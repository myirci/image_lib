#include "jpeg_adapter.hpp"
#include "image.hpp"

namespace SmpImgLib 
{
    namespace JpegAdapter 
    {
        color_space convert(ColorSpace cs) 
        {
            switch (cs)
            {
                case ColorSpace::Unspecified: return color_space::JCS_UNKNOWN;
                case ColorSpace::GrayScale: return color_space::JCS_GRAYSCALE;
                case ColorSpace::RGB: return color_space::JCS_RGB;
                case ColorSpace::YCbCr: return color_space::JCS_YCbCr;
                case ColorSpace::CMYK: return color_space::JCS_CMYK;
                case ColorSpace::YCCK: return color_space::JCS_YCCK;
                default: return color_space::JCS_UNKNOWN;
            }
        }

        ColorSpace convert(color_space cs) 
        {
            switch (cs)
            {
                case color_space::JCS_UNKNOWN: return ColorSpace::Unspecified;
                case color_space::JCS_GRAYSCALE: return ColorSpace::GrayScale;
                case color_space::JCS_RGB: return ColorSpace::RGB;
                case color_space::JCS_YCbCr: return ColorSpace::YCbCr;
                case color_space::JCS_CMYK: return ColorSpace::CMYK;
                case color_space::JCS_YCCK: return ColorSpace::YCCK;
                default: return ColorSpace::Unspecified;
            }
        }

        void export_to_jpeg(const std::string& fileName, int quality, const Image<data_type>& img)
        {
            // open the file
            FILE* outfile;
            if ((outfile = fopen(fileName.c_str(), "wb")) == NULL)
            {
                std::cerr << "Can't open " << fileName << std::endl;
                exit(1);
            }

            // allocate and initialize JPEG compression object  
            struct jpeg_error_mgr jerr;
            struct jpeg_compress_struct cinfo;
            cinfo.err = jpeg_std_error(&jerr);
            jpeg_create_compress(&cinfo);
            cinfo.image_height = img.get_height();
            cinfo.image_width = img.get_width();
            cinfo.input_components = img.get_num_channels();
            cinfo.in_color_space = convert(img.get_color_space());

            jpeg_set_defaults(&cinfo);
            jpeg_set_quality(&cinfo, quality, TRUE);
            jpeg_stdio_dest(&cinfo, outfile);
            jpeg_start_compress(&cinfo, TRUE);

            int rowStride = cinfo.image_width * cinfo.input_components;
            data_type* rowPointer[1] { };
            auto buffer = img.get_buffer();
            while (cinfo.next_scanline < cinfo.image_height)
            {
                rowPointer[0] = &(buffer[cinfo.next_scanline * rowStride]);
                (void)jpeg_write_scanlines(&cinfo, rowPointer, 1);
            }

            jpeg_finish_compress(&cinfo);

            // close the file and release the JPEG compression object
            fclose(outfile);
            jpeg_destroy_compress(&cinfo);
        }

        void import_from_jpeg(const std::string& filename, Image<data_type>& img)
        {
            FILE* infile;
            if ((infile = fopen(filename.c_str(), "rb")) == NULL)
            {
                std::cerr << "Can't open " << filename << std::endl;
                exit(1);
            }

            struct jpeg_decompress_struct cinfo;
            struct jpeg_error_mgr jerr;
            cinfo.err = jpeg_std_error(&jerr);
            jpeg_create_decompress(&cinfo);
            jpeg_stdio_src(&cinfo, infile);
            (void)jpeg_read_header(&cinfo, true);
            (void)jpeg_start_decompress(&cinfo);

            int rowStride = cinfo.output_width * cinfo.output_components;
            data_type** buffer;
            buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr)&cinfo, JPOOL_IMAGE, rowStride, 1);
            
            auto buff = std::make_unique<data_type[]>(cinfo.output_height * cinfo.output_width * cinfo.output_components);
            int j{ 0 };
            while (cinfo.output_scanline < cinfo.output_height)
            {
                (void)jpeg_read_scanlines(&cinfo, buffer, 1);
                for (int i{ 0 }; i < rowStride; ++i)
                    buff[j++] = buffer[0][i];
            }

            img.update(cinfo.output_height, cinfo.output_width, cinfo.output_components, convert(cinfo.out_color_space), buff);

            (void)jpeg_finish_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);
            fclose(infile);
        }
    }
}
