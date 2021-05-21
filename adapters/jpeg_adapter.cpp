#include "jpeg_adapter.hpp"

namespace SmpImgLib 
{
    namespace SimpleJpeg
    {
        JpegAdapter::JpegAdapter() : m_buffer{ nullptr }, m_height{ 0 }, m_width{ 0 }, m_numChannels{ 0 }, m_colorSpace{ color_space::JCS_UNKNOWN } { }

        JpegAdapter::JpegAdapter(int w, int h, int cc, color_space cs) : m_width{ w }, m_height{ h }, m_numChannels{ cc }, m_colorSpace{ cs }
        {
            allocate_memory();
        }

        JpegAdapter::JpegAdapter(const JpegAdapter& other) : m_width{ other.m_width }, m_height{ other.m_height }, m_numChannels{ other.m_numChannels }, m_colorSpace{ other.m_colorSpace }
        {
            allocate_memory();
            fill_buffer(other.m_buffer);
        }

        JpegAdapter& JpegAdapter::operator=(const JpegAdapter& rhs)
        {
            if (this != &rhs)
            {
                update(rhs.m_width, rhs.m_height, rhs.m_numChannels, rhs.m_colorSpace);
                fill_buffer(rhs.m_buffer);
            }
            return *this;
        }

        JpegAdapter::~JpegAdapter()
        {
            delete_buffer();
        }

        void JpegAdapter::fill_buffer(unsigned int val)
        {
            if (val > 255)
                val = 255;

            std::raw_storage_iterator<data_type*, data_type> raw_it(m_buffer);
            for (int i = 0; i < m_height * m_width * m_numChannels; ++i, ++raw_it)
                *raw_it = val;
        }

        void JpegAdapter::fill_buffer(unsigned int r, unsigned int g, unsigned int b)
        {
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;

            std::raw_storage_iterator<data_type*, data_type> raw_it(m_buffer);
            for (int i = 0; i < m_height * m_width * m_numChannels; ++i, ++raw_it)
            {
                if (i % 3 == 0) *raw_it = r;
                else if (i % 3 == 1) *raw_it = g;
                else *raw_it = b;
            }
        }

        void JpegAdapter::clear() 
        {
            delete_buffer();
            m_height = 0;
            m_width = 0;
            m_numChannels = 0;
            m_colorSpace = color_space::JCS_UNKNOWN;
        }

        void JpegAdapter::update(int w, int h, int cc, color_space cs)
        {
            delete_buffer();
            update_parameters(w, h, cc, cs);
            allocate_memory();
        }

        void JpegAdapter::update_parameters(int w, int h, int cc, color_space cs)
        {
            m_width = w;
            m_height = h;
            m_numChannels = cc;
            m_colorSpace = cs;
        }

        void JpegAdapter::allocate_memory()
        {
            m_buffer = (JSAMPLE*) operator new(sizeof(JSAMPLE) * m_height * m_width * m_numChannels, std::nothrow);
            if (!m_buffer)
            {
                std::cerr << "Memory allocation error!" << std::endl;
                exit(1);
            }
        }

        void JpegAdapter::fill_buffer(const data_type* const _buff)
        {
            for (int i = 0; i < m_height * m_width * m_numChannels; ++i)
                m_buffer[i] = _buff[i];
        }

        void JpegAdapter::delete_buffer()
        {
            if (m_buffer)
            {
                delete m_buffer;
                m_buffer = nullptr;
            }
        }


        void export_to_jpeg(std::string filename, int quality, const JpegAdapter& im_data)
        {
            // allocate and initialize JPEG compression object  
            struct jpeg_error_mgr jerr;
            struct jpeg_compress_struct cinfo;
            cinfo.err = jpeg_std_error(&jerr);
            jpeg_create_compress(&cinfo);

            cinfo.image_width = im_data.m_width;
            cinfo.image_height = im_data.m_height;
            cinfo.input_components = im_data.m_numChannels;
            cinfo.in_color_space = im_data.m_colorSpace;
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

            int row_stride = im_data.m_width * cinfo.input_components; // physical row width in image buffer
            JSAMPROW row_pointer[1];
            while (cinfo.next_scanline < cinfo.image_height)
            {
                row_pointer[0] = &(im_data.m_buffer[cinfo.next_scanline * row_stride]);
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
                    im_data.m_buffer[j++] = buffer[0][i];
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
}
