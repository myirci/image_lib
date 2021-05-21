#pragma once

#include "color_traits.hpp"

namespace SmpImgLib
{
    enum class ColorSpace : unsigned char
    {
        Unspecified,
        GrayScale,
        RGB,
        YCbCr,
        CMYK,
        YCCK
    };

    template <typename T>
    struct color_rgb
    {
        typedef color_rgb_tag color_space_category;
        color_rgb(T r_, T g_, T b_) : r(r_), g(g_), b(b_) { }
        T r, g, b;
    };

    template <typename T>
    struct color_mono
    {
        typedef color_mono_tag color_space_category;
        color_mono(T v_) : v(v_) { }
        T v;
    };


    /*
    template<>
    J_COLOR_SPACE enum_convert<J_COLOR_SPACE>(int val) 
    {
        switch (val) 
        {
        case 0:
            return JCS_UNKNOWN;
        case 1:
            return JCS_GRAYSCALE;
        case 2:
            return JCS_RGB;
        case 3:
            return JCS_YCbCr;
        case 4:
            return JCS_CMYK;
        case 5:
            return JCS_YCCK;
        default:
            return JCS_UNKNOWN;
        }
    }
    */
}


