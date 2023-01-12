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
    
    void Export(const std::string& fileName, int quality, const Image<data_type>& img)
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
            cinfo.image_height = img.get_height();
            cinfo.image_width = img.get_width();
            cinfo.input_components = img.get_num_channels();
            cinfo.in_color_space = Convert(img.get_color_space());

            jpeg_set_defaults(&cinfo);
            jpeg_set_quality(&cinfo, quality, TRUE);
            jpeg_stdio_dest(&cinfo, outFile);
            jpeg_start_compress(&cinfo, TRUE);

            int rowStride = cinfo.image_width * cinfo.input_components;
            data_type* rowPointer[1];
            auto buffer = img.get_buffer();
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

            throw std::runtime_error("Something went wrong while exporting the image to a jpeg file.");
        }
    }

    void Import(const std::string& fileName, Image<data_type>& img)
    {
        // Open the file
        FILE* inFile;
        if (inFile = fopen(fileName.c_str(), "rb"); inFile == nullptr)
            throw std::runtime_error(std::format("File cannot be opened: {}", fileName));

        bool success{ false };

        jpeg_decompress_struct cinfo{};

        try 
        {
            jpeg_error_mgr jerr {};
            cinfo.err = jpeg_std_error(&jerr);
            jpeg_create_decompress(&cinfo);
            jpeg_stdio_src(&cinfo, inFile);
            jpeg_read_header(&cinfo, true);
            jpeg_start_decompress(&cinfo);

            int rowStride = cinfo.output_width * cinfo.output_components;
            data_type** buffer;
            buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr)&cinfo, JPOOL_IMAGE, rowStride, 1);

            auto size = cinfo.output_height * cinfo.output_width * cinfo.output_components;
            auto buff = std::make_unique<data_type[]>(size);
            int j{ 0 };
            while (cinfo.output_scanline < cinfo.output_height)
            {
                jpeg_read_scanlines(&cinfo, buffer, 1);
                for (int i{ 0 }; i < rowStride; ++i)
                    buff[j++] = buffer[0][i];
            }

            img.update(cinfo.output_height, cinfo.output_width, cinfo.output_components, Convert(cinfo.out_color_space), buff);

            jpeg_finish_decompress(&cinfo);
            
            fclose(inFile);
            inFile = nullptr;

            jpeg_destroy_decompress(&cinfo);
            success = true;
        }
        catch (...) 
        {
            if (!success)
            {
                if (inFile)
                    fclose(inFile);

                jpeg_finish_decompress(&cinfo);
            }

            throw std::runtime_error("Something went wrong while importing the image from a jpeg file.");
        }
    }
}
