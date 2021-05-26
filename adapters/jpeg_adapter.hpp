#pragma once

#include <cstdio>
#include <iostream>
#include <new>
#include <memory>
#include <string>
#include <cstdlib>

#include "image.hpp"
#include "color.hpp"
#include "jpeglib.h"

#pragma warning( disable : 26812)

namespace SmpImgLib 
{
    namespace JpegAdapter
    {
        using data_type = JSAMPLE;
        using color_space = J_COLOR_SPACE;
        
        color_space convert(ColorSpace cs);
        void export_to_jpeg(const std::string& fileName, int quality, const Image<data_type>& img);
        void import_from_jpeg(const std::string& fileName, Image<data_type>& img);

        //  void export_to_binary(std::string filename, const JpegWrapper& im_data);
        //  void import_to_binary(std::string filename, JpegWrapper& im_data);
    };
}