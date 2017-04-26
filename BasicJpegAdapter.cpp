#include "BasicJpegAdapter.hpp"

void write_jpeg_file(std::string filename, int quality, const image_data& im_data) {
    struct jpeg_compress_struct cinfo;          // this struct contains the jpeg compression parameters
    struct jpeg_error_mgr jerr;                 // this struct represents a jpeg error handler
    /// Step 1: allocate and initialize JPEG compression object
    cinfo.err = jpeg_std_error(&jerr);          // Setup the error handler first, incase the initialization step fails.
    jpeg_create_compress(&cinfo);               // Now we can initialize the JPEG compression object.
    /// Step 2: specify data destination (eg, a file)
    FILE* outfile;
    if((outfile = fopen(filename.c_str(), "wb")) == NULL) {
        std::cerr << "Can't open " << filename << std::endl;
        exit(1);
    }
    jpeg_stdio_dest(&cinfo, outfile);
    /// Step 3: set parameters for compression
    cinfo.image_width = im_data.width;                      // image width and height, in pixels
    cinfo.image_height = im_data.height;
    cinfo.input_components = im_data.color_components;      // number of color components per pixel
    cinfo.in_color_space = im_data.color_space;             // colorspace of input image
    jpeg_set_defaults(&cinfo);                              // set default compression parameters.
    jpeg_set_quality(&cinfo, quality, TRUE);                // set any non-default parameters you wish to (in this ex: quality)
    /// Step 4: Start compressor
    jpeg_start_compress(&cinfo, TRUE);
    /// Step 5: while (scan lines remain to be written)
    int row_stride = im_data.width * cinfo.input_components;// physical row width in image buffer
    JSAMPROW row_pointer[1];                                // pointer to JSAMPLE row[s]
    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &(im_data.buff[cinfo.next_scanline * row_stride]);
        (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    /// Step 6: Finish compression
    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    /// Step 7: release JPEG compression object
    jpeg_destroy_compress(&cinfo);
}

void read_JPEG_file (std::string filename, image_data& im_data) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE* infile;
    if ((infile = fopen(filename.c_str(), "rb")) == NULL) {
        std::cerr << "Can't open " << filename << std::endl;
        exit(1);
    }
    /// Step 1: allocate and initialize JPEG decompression object
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    /// Step 2: specify data source (eg, a file)
    jpeg_stdio_src(&cinfo, infile);
    /// Step 3: read file parameters with jpeg_read_header()
    (void) jpeg_read_header(&cinfo, true);
    /// Step 4: set parameters for decompression
    // we don't need to change any of the defaults set by
    /// Step 5: Start decompressor
    (void) jpeg_start_decompress(&cinfo);
    int row_stride = cinfo.output_width * cinfo.output_components;
    JSAMPARRAY buffer;
    buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    im_data.update(cinfo.output_width, cinfo.output_height, cinfo.output_components, cinfo.out_color_space);
    /// Step 6: while (scan lines remain to be read)
    int j = 0;
    while(cinfo.output_scanline < cinfo.output_height) {
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        for(int i = 0; i < row_stride; ++i) {
            im_data.buff[j++] = buffer[0][i];
        }
    }
    /// Step 7: Finish decompression
    (void) jpeg_finish_decompress(&cinfo);
    /// Step 8: Release JPEG decompression object
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
}

