#pragma once

#include <cstdio>
#include <iostream>
#include <new>
#include <memory>
#include <string>
#include <cstdlib>
#include "jpeglib.h"

#pragma warning( disable : 26812)

namespace SmpImgLib 
{
    namespace SimpleJpeg
    {
        using data_type = JSAMPLE;

        class JpegAdapter
        {
        public:
            int height;
            int width;
            int color_components;
            JSAMPLE* buffer;
            J_COLOR_SPACE color_space;

            JpegAdapter();
            JpegAdapter(int w, int h, int cc, J_COLOR_SPACE cs);
            
            JpegAdapter(const JpegAdapter& im_data);

            JpegAdapter& operator=(const JpegAdapter& rhs);

            virtual ~JpegAdapter();

            void fill_buffer(unsigned int val);
            void fill_buffer(unsigned int r, unsigned int g, unsigned int b);
            void update(int w, int h, int cc, J_COLOR_SPACE cs);
            void clear();

        private:

            void update_parameters(int w, int h, int cc, J_COLOR_SPACE cs);
            void allocate_memory();
            void fill_buffer(const JSAMPLE* const _buff);
            void delete_buffer();
        };

        // stand-alone utiliy functions for import/export
        void export_to_jpeg(std::string filename, int quality, const JpegAdapter& im_data);
        void import_from_jpeg(std::string filename, JpegAdapter& im_data);
        void export_to_binary(std::string filename, const JpegAdapter& im_data);
        void import_to_binary(std::string filename, JpegAdapter& im_data);
    }
}