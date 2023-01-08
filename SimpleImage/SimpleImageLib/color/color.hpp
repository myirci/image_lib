#pragma once

#include <color/ColorTraits.hpp>

namespace imglib
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
}


