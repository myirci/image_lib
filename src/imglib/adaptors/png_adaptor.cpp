
#include <imglib/adaptors/png_adaptor.hpp>
#include <imglib/utility/utility.hpp>

namespace imglib::png
{
	namespace 
	{
		bool IsPng(FILE* pFile, int numBytesToRead) 
		{
			auto buf = std::make_unique<unsigned char[]>(numBytesToRead);

			if (fread(buf.get(), 1, numBytesToRead, pFile) != numBytesToRead)
				throw std::runtime_error("File cannot be read");

			return png_sig_cmp(buf.get(), 0, numBytesToRead) == 0;
		}

		void CloseRead(FILE* pFile, png_struct* pReadStruct = nullptr, png_info* pInfoStruct = nullptr, png_byte** ppRowPointers = nullptr, size_t numRows = 0)
		{
			if (pFile)
				fclose(pFile);

			if (pReadStruct && pInfoStruct)
				png_destroy_read_struct(&pReadStruct, &pInfoStruct, nullptr);
			else if (pReadStruct)
				png_destroy_read_struct(&pReadStruct, nullptr, nullptr);
			else if (pInfoStruct)
				png_destroy_read_struct(nullptr, &pInfoStruct, nullptr);			

			if (ppRowPointers && numRows > 0)
				for (size_t i = 0; i < numRows; i++)
					if (ppRowPointers[i])
						delete[] ppRowPointers[i];
		}

		void CloseWrite(FILE* pFile, png_struct* pWriteStruct = nullptr, png_info* pInfoStruct = nullptr, png_byte** ppRowPointers = nullptr, size_t numRows = 0)
		{
			if (pFile)
				fclose(pFile);

			if (pWriteStruct && pInfoStruct)
				png_destroy_write_struct(&pWriteStruct, &pInfoStruct);
			else if (pWriteStruct)
				png_destroy_write_struct(&pWriteStruct, nullptr);
			else if (pInfoStruct)
				png_destroy_write_struct(nullptr, &pInfoStruct);

			if (ppRowPointers && numRows > 0)
				for (size_t i = 0; i < numRows; i++)
					if (ppRowPointers[i])
						delete[] ppRowPointers[i];
		}

		std::pair<ColorSpace, int> GetColorSpaceAndNumChannels(int cspace) 
		{
			switch (cspace) 
			{
				case PNG_COLOR_TYPE_GRAY:
					return { ColorSpace::GrayScale, 1 };
				case PNG_COLOR_TYPE_GRAY_ALPHA:
					return { ColorSpace::GrayScaleAlpha, 2 };
				case PNG_COLOR_TYPE_RGB:
				case PNG_COLOR_TYPE_PALETTE:
					return { ColorSpace::RGB, 3 };
				case PNG_COLOR_TYPE_RGB_ALPHA:
					return { ColorSpace::RGBA, 4 };
				default:
					return{ ColorSpace::Unspecified, 0 };
			}
		}

		int GetPngColorSpace(ColorSpace cs)
		{
			switch (cs)
			{
			case imglib::ColorSpace::GrayScale:
				return PNG_COLOR_TYPE_GRAY;
			case imglib::ColorSpace::GrayScaleAlpha:
				return PNG_COLOR_TYPE_GRAY_ALPHA;
			case imglib::ColorSpace::RGB:
				return PNG_COLOR_TYPE_RGB;
			case imglib::ColorSpace::RGBA:
				return PNG_COLOR_TYPE_RGB_ALPHA;
			default:
				throw std::logic_error("Invalid color space for a png file");
			}
		}
	}

	PngImg Read(std::wstring_view fileName)
	{
		// Open the image file for reading.
		auto pFile = OpenFile(fileName, Mode::Read);

		int numBytesToCheck{ 8 };
		if (!IsPng(pFile, numBytesToCheck))
		{
			CloseRead(pFile);
			throw std::runtime_error("File is not a png file.");
		}

		// Create and initialize the png_struct.
		auto my_png_read_struct = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (!my_png_read_struct)
		{
			CloseRead(pFile);
			throw std::runtime_error("PNG read struct could not be created.");
		}

		// Allocate / initialize the memory for image information.
		auto my_png_info_struct = png_create_info_struct(my_png_read_struct);
		if (!my_png_info_struct)
		{
			CloseRead(pFile, my_png_read_struct);
			throw std::runtime_error("PNG image info struct could not be created.");
		}

		if (setjmp(png_jmpbuf(my_png_read_struct)))
		{
			// libpng jumps here if it encounters an error.
			CloseRead(pFile, my_png_read_struct, my_png_info_struct);
			throw std::runtime_error("PNG error!");
		}

		// Initialize the input/output for the PNG file to the default functions.
		png_init_io(my_png_read_struct, pFile);

		// Let the libpng know how many bytes are read to verify that the given file is a png file.
		png_set_sig_bytes(my_png_read_struct, numBytesToCheck);

		// Read the information before the actual image data.
		png_read_info(my_png_read_struct, my_png_info_struct);

		// Read image information.
		PngInfo inImgProp{};
		png_get_IHDR(my_png_read_struct, my_png_info_struct, &inImgProp.width, &inImgProp.height, &inImgProp.bit_depth,
			&inImgProp.color_type, &inImgProp.interlace_type, &inImgProp.compression_type, &inImgProp.filter_method);

		// Color types: 
		// PNG_COLOR_TYPE_GRAY		 -> bit depths 1, 2, 4, 8, 16
		// PNG_COLOR_TYPE_GRAY_ALPHA -> bit depths 8, 16
		// PNG_COLOR_TYPE_PALETTE	 -> bit depths 1, 2, 4, 8
		// PNG_COLOR_TYPE_RGB		 -> bit_depths 8, 16
		// PNG_COLOR_TYPE_RGB_ALPHA  -> bit_depths 8, 16

		// Set-up the transformations so that each color component is either 8-bit or 16-bit depth.

		// Grayscale images with bith-depth less than 8, will be read into a single byte.
		if (inImgProp.color_type == PNG_COLOR_TYPE_GRAY && inImgProp.bit_depth < 8)
			png_set_expand_gray_1_2_4_to_8(my_png_read_struct);

		// Palette images are read as RGB images.
		if (inImgProp.color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(my_png_read_struct);

		// If there exists a transparency chunk (tRNS chunk), convert it to an alpha channel.
		if (png_get_valid(my_png_read_struct, my_png_info_struct, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(my_png_read_struct);

		// Update the info structure to reflect the applied transformations.
		png_read_update_info(my_png_read_struct, my_png_info_struct);

		PngImg img;

		// Allocate memory for the image.
		png_byte** row_pointers{ nullptr };
		try
		{
			row_pointers = new png_byte * [inImgProp.height];
			for (size_t i = 0; i < inImgProp.height; i++)
			{
				void* row_bytes = png_malloc(my_png_read_struct, png_get_rowbytes(my_png_read_struct, my_png_info_struct));
				row_pointers[i] = static_cast<png_byte*>(row_bytes);
			}
		}
		catch (...)
		{
			CloseRead(pFile, my_png_read_struct, my_png_info_struct, row_pointers, inImgProp.height);
			throw std::runtime_error("Something went wrong when allocating memory for reading the image.");
		}

		// Read image data into the allocated memory.
		png_read_image(my_png_read_struct, row_pointers);

		// End reading the image data.
		png_read_end(my_png_read_struct, nullptr);

		// Create the image
		auto [cspace, numChannels] = GetColorSpaceAndNumChannels(inImgProp.color_type);
		if (cspace == ColorSpace::Unspecified || numChannels == 0) 
		{
			CloseRead(pFile, my_png_read_struct, my_png_info_struct, row_pointers, inImgProp.height);
			throw std::runtime_error("Color space or number of channels cannot be detected.");
		}
			
		if (inImgProp.bit_depth <= 8)
		{
			auto im = ImgBitDepth8(inImgProp.height, inImgProp.width, cspace, numChannels);
			for (size_t i = 0; i < inImgProp.height; i++)
			{
				size_t byte_index{ 0 };
				for (size_t j = 0; j < inImgProp.width; j++)
					for (size_t k = 0; k < numChannels; k++)
						im(k)(i, j) = row_pointers[i][byte_index++];
			}

			img = std::move(im);
		}
		else if (inImgProp.bit_depth <= 16)
		{
			auto im = ImgBitDepth16(inImgProp.height, inImgProp.width, cspace, numChannels);
			for (size_t i = 0; i < inImgProp.height; i++)
			{
				size_t byte_index{ 0 };
				for (size_t j = 0; j < inImgProp.width; j++)
				{
					for (size_t k = 0; k < numChannels; k++)
					{
						std::uint16_t val{ row_pointers[i][byte_index++] };
						val <<= 8;
						val += row_pointers[i][byte_index++];
						im(k)(i, j) = val;
					}
				}
			}

			img = std::move(im);
		}
		else 
		{
			CloseRead(pFile, my_png_read_struct, my_png_info_struct, row_pointers, inImgProp.height);
			throw std::runtime_error("Bit-depth error, add support for bith-depths greater than 16.");
		}
		
		// do the memory clean-up and return the image.
		CloseRead(pFile, my_png_read_struct, my_png_info_struct, row_pointers, inImgProp.height);
		return img;
	}

	void Write(const PngImg& img, std::wstring_view fileName) 
	{
		const ImgBitDepth8* pImg8 = std::get_if<ImgBitDepth8>(&img);
		const ImgBitDepth16* pImg16 = std::get_if<ImgBitDepth16>(&img);

		if (pImg8 == nullptr && pImg16 == nullptr)
			throw std::logic_error("Image type error.");

		// Open the image file for writing.
		auto pFile = OpenFile(fileName, Mode::Write);

		// Create and initialize the png_struct.
		auto my_png_write_struct = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (!my_png_write_struct)
		{
			CloseWrite(pFile);
			throw std::runtime_error("PNG write struct could not be created.");
		}

		// Allocate / initialize the memory for image information.
		auto my_png_info_struct = png_create_info_struct(my_png_write_struct);
		if (!my_png_info_struct)
		{
			CloseWrite(pFile, my_png_write_struct);
			throw std::runtime_error("PNG image info struct could not be created.");
		}

		if (setjmp(png_jmpbuf(my_png_write_struct)))
		{
			// libpng jumps here if it encounters an error.
			CloseWrite(pFile, my_png_write_struct, my_png_info_struct);
			throw std::runtime_error("PNG error!");
		}

		// Initialize the input/output for the PNG file to the default functions.
		png_init_io(my_png_write_struct, pFile);

		png_uint_32 width{ 0 }, height{ 0 };
		int bit_depth{ 0 };
		int color_type{ -1 };
		size_t maxHeight{ 0 };
		bool validSize{ false };
		size_t rowDataSize{ 0 };
		size_t numChannels{ 0 };

		if (pImg8) 
		{
			width = static_cast<png_uint_32>(pImg8->width());
			height = static_cast<png_uint_32>(pImg8->height());
			bit_depth = 8;
			color_type = GetPngColorSpace(pImg8->color_space());
			validSize = pImg8->data_size() <= PNG_SIZE_MAX;
			numChannels = pImg8->num_channels();
			rowDataSize = width * numChannels;
		}
		else 
		{
			width = static_cast<png_uint_32>(pImg16->width());
			height = static_cast<png_uint_32>(pImg16->height());
			bit_depth = 16;
			color_type = GetPngColorSpace(pImg16->color_space());
			validSize = pImg16->data_size() <= PNG_SIZE_MAX;
			numChannels = pImg16->num_channels();
			rowDataSize = width * numChannels * 2;
		}

		if (!validSize)
			throw std::logic_error("Image size is too big");

		// Set image info.
		png_set_IHDR(my_png_write_struct, my_png_info_struct, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

		// Write the file header information.
		png_write_info(my_png_write_struct, my_png_info_struct);

		// Set transformations if needed - no transformations.
		
		// Allocate memory for the image.
		png_byte** row_pointers{ nullptr };
		try
		{
			row_pointers = new png_byte * [height];
			for (size_t i = 0; i < height; i++)
				row_pointers[i] = new png_byte[rowDataSize];
		}
		catch (...)
		{
			CloseWrite(pFile, my_png_write_struct, my_png_info_struct, row_pointers, height);
			throw std::runtime_error("Something went wrong when allocating memory for writing the image.");
		}

		// Fill the allocated memory with image data.

		if (pImg8) 
		{
			for (size_t i = 0; i < height; i++)
			{
				size_t byte_index{ 0 };
				for (size_t j = 0; j < width; j++)
					for (size_t k = 0; k < numChannels; k++) 
						row_pointers[i][byte_index++] = (*pImg8)(k)(i, j);
			}
		}
		else 
		{
			for (size_t i = 0; i < height; i++)
			{
				size_t byte_index{ 0 };
				for (size_t j = 0; j < width; j++) 
				{
					for (size_t k = 0; k < numChannels; k++) 
					{
						std::uint16_t val = (*pImg16)(k)(i, j);
						row_pointers[i][byte_index++] = (val >> 8);
						row_pointers[i][byte_index++] = static_cast<uint8_t>(val);
					}
				}
			}
		}

		// write the image.
		png_write_image(my_png_write_struct, row_pointers);

		// Finish the writing.
		png_write_end(my_png_write_struct, my_png_info_struct);

		// Do the memory clean-up.
		CloseWrite(pFile, my_png_write_struct, my_png_info_struct, row_pointers, height);
	}
}