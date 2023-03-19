#pragma once

#include <cstdio>
#include <iostream>
#include <new>
#include <memory>
#include <string>
#include <cstdlib>
#include <string_view>

#include <jpeglib.h>

#include <imglib/image/image.hpp>
#include <imglib/color/color.hpp>

namespace imglib::jpeg
{
    using data_type = JSAMPLE;
    using JpegImg = Image<jpeg::data_type>;

    // Saves the given image to a jpeg file
    void Write(std::wstring_view fileName, int quality, const Image<data_type>& img);
    
    // Reads the given jpeg file
    Image<data_type> Read(std::wstring_view fileName);
}