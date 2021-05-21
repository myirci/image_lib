#pragma once

#include <cstdio>
#include <iostream>
#include <new>
#include <memory>
#include <string>
#include <cstdlib>

#include "color.hpp"
#include "jpeglib.h"

#pragma warning( disable : 26812)

namespace SmpImgLib 
{
    namespace SimpleJpeg
    {
        using data_type = JSAMPLE;
        using color_space = J_COLOR_SPACE;

        class JpegAdapter
        {
        public:
            JpegAdapter();
            JpegAdapter(int w, int h, int cc, color_space cs);
       
            JpegAdapter(const JpegAdapter& other);

            JpegAdapter& operator=(const JpegAdapter& rhs);

            virtual ~JpegAdapter();

            void fill_buffer(unsigned int val);
            void fill_buffer(unsigned int r, unsigned int g, unsigned int b);
            void update(int w, int h, int cc, color_space cs);
            void clear();

        private:
            int m_height;
            int m_width;
            int m_numChannels;
            data_type* m_buffer;
            color_space m_colorSpace;

            void update_parameters(int w, int h, int cc, color_space cs);
            void allocate_memory();
            void fill_buffer(const data_type* const _buff);
            void delete_buffer();

            friend void export_to_jpeg(std::string, int, const JpegAdapter&);
            friend void import_from_jpeg(std::string, JpegAdapter&);
        };

        // stand-alone utiliy functions for import/export
        void export_to_jpeg(std::string filename, int quality, const JpegAdapter& im_data);
        void import_from_jpeg(std::string filename, JpegAdapter& im_data);
        void export_to_binary(std::string filename, const JpegAdapter& im_data);
        void import_to_binary(std::string filename, JpegAdapter& im_data);
        
       /* ColorSpace get_color_space(int val)
        {
            switch (val)
            {
            case 0:
                return UNSPECIFIED;
            case 1:
                return GRAYSCALE;
            case 2:
                return RGB;
            case 3:
                return YCbCr;
            case 4:
                return CMYK;
            case 5:
                return YCCK;
            default:
                return UNSPECIFIED;
            }
        }*/
    }
}