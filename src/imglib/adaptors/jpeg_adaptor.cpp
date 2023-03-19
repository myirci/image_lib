#include <imglib/adaptors/jpeg_adaptor.hpp>

#include <stdexcept>
#include <format>

namespace imglib::jpeg 
{
    namespace 
    {
        J_COLOR_SPACE Convert(ColorSpace cs)
        {
            switch (cs)
            {
            case ColorSpace::Unspecified:
                return J_COLOR_SPACE::JCS_UNKNOWN;
            case ColorSpace::GrayScale:
                return J_COLOR_SPACE::JCS_GRAYSCALE;
            case ColorSpace::RGB:
                return J_COLOR_SPACE::JCS_RGB;
            case ColorSpace::YCbCr:
                return J_COLOR_SPACE::JCS_YCbCr;
            case ColorSpace::CMYK:
                return J_COLOR_SPACE::JCS_CMYK;
            case ColorSpace::YCCK:
                return J_COLOR_SPACE::JCS_YCCK;
            default:
                return J_COLOR_SPACE::JCS_UNKNOWN;
            }
        }

        ColorSpace Convert(J_COLOR_SPACE cs)
        {
            switch (cs)
            {
            case J_COLOR_SPACE::JCS_UNKNOWN:
                return ColorSpace::Unspecified;
            case J_COLOR_SPACE::JCS_GRAYSCALE:
                return ColorSpace::GrayScale;
            case J_COLOR_SPACE::JCS_RGB:
                return ColorSpace::RGB;
            case J_COLOR_SPACE::JCS_YCbCr:
                return ColorSpace::YCbCr;
            case J_COLOR_SPACE::JCS_CMYK:
                return ColorSpace::CMYK;
            case J_COLOR_SPACE::JCS_YCCK:
                return ColorSpace::YCCK;
            default:
                return ColorSpace::Unspecified;
            }
        }
    }
    
    void Write(const std::string& fileName, int quality, const Image<data_type>& img)
    {
        // Open the file
        FILE* outFile{ nullptr };
        if (outFile = fopen(fileName.c_str(), "wb"); outFile == nullptr)
            throw std::runtime_error(std::format("File cannot be opened: {}", fileName));

        bool success{ false };
        jpeg_compress_struct cinfo{};

        try 
        {
            // Allocate and initialize JPEG compression object
            jpeg_error_mgr jerr{};
            cinfo.err = jpeg_std_error(&jerr);
            jpeg_create_compress(&cinfo);
            cinfo.image_height = static_cast<int>(img.height());
            cinfo.image_width = static_cast<int>(img.width());
            cinfo.input_components = static_cast<int>(img.num_channels());
            cinfo.in_color_space = Convert(img.color_space());

            jpeg_set_defaults(&cinfo);
            jpeg_set_quality(&cinfo, quality, TRUE);
            jpeg_stdio_dest(&cinfo, outFile);
            jpeg_start_compress(&cinfo, TRUE);

            int rowStride = cinfo.image_width * cinfo.input_components;
            data_type* rowPointer[1];
            auto buffer = img.data();
            while (cinfo.next_scanline < cinfo.image_height)
            {
                rowPointer[0] = &(buffer[cinfo.next_scanline * rowStride]);
                jpeg_write_scanlines(&cinfo, rowPointer, 1);
            }

            jpeg_finish_compress(&cinfo);

            // Close the file and release the JPEG compression object
            fclose(outFile);
            outFile = nullptr;

            jpeg_destroy_compress(&cinfo);
            success = true;
        }
        catch (...) 
        {
            if (!success)
            {
                if (outFile)
                    fclose(outFile);
                
                jpeg_destroy_compress(&cinfo);
            }

            throw std::runtime_error("Something went wrong while writing the image to a jpeg file.");
        }
    }

    Image<data_type> Read(const std::string& fileName)
    {
        // Open the file
        FILE* inFile;
        if (inFile = fopen(fileName.c_str(), "rb"); inFile == nullptr)
            throw std::runtime_error(std::format("File cannot be opened: {}", fileName));

        bool success{ false };

        jpeg_decompress_struct cinfo{};

        try 
        {
            jpeg_error_mgr jerr{};
            cinfo.err = jpeg_std_error(&jerr);
            jpeg_create_decompress(&cinfo);
            jpeg_stdio_src(&cinfo, inFile);
            jpeg_read_header(&cinfo, true);
            jpeg_start_decompress(&cinfo);

            auto rowStride = cinfo.output_width * cinfo.output_components;   
            data_type** buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr)&cinfo, JPOOL_IMAGE, rowStride, 1);

            auto img = Image<data_type>(cinfo.output_height, cinfo.output_width, Convert(cinfo.out_color_space), cinfo.output_components);
            
            size_t rowNo{ 0 };
            while (cinfo.output_scanline < cinfo.output_height)
            {
                jpeg_read_scanlines(&cinfo, buffer, 1);

                for (auto i = 0; i < cinfo.out_color_components; i++) 
                {
                    size_t pixel{ cinfo.output_width * rowNo };
                    for (unsigned int j = i; j < rowStride; j += cinfo.out_color_components)
                        img(i)(pixel++) = buffer[0][j];
                }                  
                rowNo++;
            }

            jpeg_finish_decompress(&cinfo);
            
            fclose(inFile);
            inFile = nullptr;

            jpeg_destroy_decompress(&cinfo);
            success = true;

            return img;
        }
        catch (...) 
        {
            if (!success)
            {
                if (inFile)
                    fclose(inFile);

                jpeg_finish_decompress(&cinfo);
            }

            throw std::runtime_error("Something went wrong while reading the image from a jpeg file.");
        }
    }
}
