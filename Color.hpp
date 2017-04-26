#ifndef COLOR_HPP
#define COLOR_HPP

#include "TraitsClasses.hpp"
#include "jpeglib.h"

typedef enum {
        UNSPECIFIED = JCS_UNKNOWN,          // error/unspecified
        GRAYSCALE   = JCS_GRAYSCALE,        // monochrome
        RGB         = JCS_RGB,              // red/green/blue
        YCbCr       = JCS_YCbCr,            // Y/Cb/Cr (also known as YUV)
        CMYK        = JCS_CMYK,             // C/M/Y/K
        YCCK        = JCS_YCCK              // Y/Cb/Cr/K
} IMG_COLOR_SPACE;

template <typename T>
T enum_convert(int val);

template<>
IMG_COLOR_SPACE enum_convert<IMG_COLOR_SPACE>(int val) {
    switch(val) {
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
}

template<>
J_COLOR_SPACE enum_convert<J_COLOR_SPACE>(int val) {
    switch(val) {
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

template <typename T>
struct color_rgb {                  // for RGB images
    typedef color_rgb_tag color_space_category;
    color_rgb(T r_, T g_, T b_) : r(r_), g(g_), b(b_) { }
    T r, g, b;
};

template <typename T>
struct color_mono {                 // for grayscale images
    typedef color_mono_tag color_space_category;
    color_mono(T v_) : v(v_) { }
    T v;
};

#endif // COLOR_HPP
