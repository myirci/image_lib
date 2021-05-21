#include "jpeg_adapter.hpp"

namespace SimpleJpeg
{
    void export_to_jpeg(std::string filename, int quality, const JpegAdapter& im_data)
    {
        // allocate and initialize JPEG compression object  
        struct jpeg_error_mgr jerr;
        struct jpeg_compress_struct cinfo;
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_compress(&cinfo);

        cinfo.image_width = im_data.width;
        cinfo.image_height = im_data.height;
        cinfo.input_components = im_data.color_components;
        cinfo.in_color_space = im_data.color_space;
        jpeg_set_defaults(&cinfo);  // set default compression parameters.
        jpeg_set_quality(&cinfo, quality, TRUE); // set any non-default parameters you wish to (in this ex: quality)

        FILE* outfile;
        if ((outfile = fopen(filename.c_str(), "wb")) == NULL)
        {
            std::cerr << "Can't open " << filename << std::endl;
            exit(1);
        }
        jpeg_stdio_dest(&cinfo, outfile);

        // start compressor
        jpeg_start_compress(&cinfo, TRUE);

        int row_stride = im_data.width * cinfo.input_components; // physical row width in image buffer
        JSAMPROW row_pointer[1];
        while (cinfo.next_scanline < cinfo.image_height)
        {
            row_pointer[0] = &(im_data.buffer[cinfo.next_scanline * row_stride]);
            (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
        }

        // finish compression
        jpeg_finish_compress(&cinfo);

        // close the file and release the JPEG compression object
        fclose(outfile);
        jpeg_destroy_compress(&cinfo);
    }

    void import_from_jpeg(std::string filename, JpegAdapter& im_data)
    {
        struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr jerr;
        FILE* infile;
        if ((infile = fopen(filename.c_str(), "rb")) == NULL)
        {
            std::cerr << "Can't open " << filename << std::endl;
            exit(1);
        }

        // allocate and initialize JPEG decompression object
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&cinfo);

        // specify data source (eg, a file)
        jpeg_stdio_src(&cinfo, infile);

        // read file parameters with jpeg_read_header()
        (void)jpeg_read_header(&cinfo, true);

        // start decompressor
        (void)jpeg_start_decompress(&cinfo);
        int row_stride = cinfo.output_width * cinfo.output_components;
        JSAMPARRAY buffer;
        buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);
        im_data.update(cinfo.output_width, cinfo.output_height, cinfo.output_components, cinfo.out_color_space);

        int j = 0;
        while (cinfo.output_scanline < cinfo.output_height)
        {
            (void)jpeg_read_scanlines(&cinfo, buffer, 1);
            for (int i = 0; i < row_stride; ++i)
                im_data.buffer[j++] = buffer[0][i];
        }

        // finish decompression, release JPEG decompression object and close the file
        (void)jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
    }

    void export_to_binary(std::string filename, const JpegAdapter& im_data)
    {

    }

    void import_to_binary(std::string filename, JpegAdapter& im_data)
    {

    }
}