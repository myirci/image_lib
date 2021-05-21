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

            JpegAdapter() : buffer{ nullptr }, height{ 0 }, width{ 0 }, color_components{ 0 }, color_space{ J_COLOR_SPACE::JCS_UNKNOWN } { }

            JpegAdapter(int w, int h, int cc, J_COLOR_SPACE cs) : width{ w }, height{ h }, color_components{ cc }, color_space{ cs }
            {
                allocate_memory();
            }

            JpegAdapter(const JpegAdapter& im_data) : width{ im_data.width }, height{ im_data.height }, color_components{ im_data.color_components }, color_space{ im_data.color_space }
            {
                allocate_memory();
                fill_buffer(im_data.buffer);
            }

            JpegAdapter& operator=(const JpegAdapter& rhs)
            {
                if (this != &rhs)
                {
                    update(rhs.width, rhs.height, rhs.color_components, rhs.color_space);
                    fill_buffer(rhs.buffer);
                }
                return *this;
            }

            virtual ~JpegAdapter()
            {
                delete_buffer();
            }

            void fill_buffer(unsigned int val)
            {
                if (val > 255)
                    val = 255;

                std::raw_storage_iterator<JSAMPLE*, JSAMPLE> raw_it(buffer);
                for (int i = 0; i < height * width * color_components; ++i, ++raw_it)
                    *raw_it = val;
            }

            void fill_buffer(unsigned int r, unsigned int g, unsigned int b)
            {
                if (r > 255) r = 255;
                if (g > 255) g = 255;
                if (b > 255) b = 255;

                std::raw_storage_iterator<JSAMPLE*, JSAMPLE> raw_it(buffer);
                for (int i = 0; i < height * width * color_components; ++i, ++raw_it)
                {
                    if (i % 3 == 0) *raw_it = r;
                    else if (i % 3 == 1) *raw_it = g;
                    else *raw_it = b;
                }
            }

            void update(int w, int h, int cc, J_COLOR_SPACE cs)
            {
                delete_buffer();
                update_parameters(w, h, cc, cs);
                allocate_memory();
            }

            void clear()
            {
                delete_buffer();
            }

            void print_buffer()
            {
                for (int i = 0; i < height * width * color_components; ++i)
                    std::cout << (int)buffer[i] << " ";
                std::cout << std::endl;
            }

        private:

            void update_parameters(int w, int h, int cc, J_COLOR_SPACE cs)
            {
                width = w;
                height = h;
                color_components = cc;
                color_space = cs;
            }

            void allocate_memory()
            {
                buffer = (JSAMPLE*) operator new(sizeof(JSAMPLE) * height * width * color_components, std::nothrow);
                if (!buffer)
                {
                    std::cerr << "Memory allocation error!" << std::endl;
                    exit(1);
                }
            }

            void fill_buffer(const JSAMPLE* const _buff)
            {
                for (int i = 0; i < height * width * color_components; ++i)
                    buffer[i] = _buff[i];
            }

            void delete_buffer()
            {
                if (buffer)
                {
                    delete buffer;
                    buffer = nullptr;
                }
            }
        };

        void export_to_jpeg(std::string filename, int quality, const JpegAdapter& im_data);
        void import_from_jpeg(std::string filename, JpegAdapter& im_data);
        void export_to_binary(std::string filename, const JpegAdapter& im_data);
        void import_to_binary(std::string filename, JpegAdapter& im_data);
    }
}