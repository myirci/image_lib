#include "image.hpp"

namespace SmpImgLib
{
    template<typename T>
    void convert_rgb_to_grayscale(Image<T>& im_rgb, Image<T>& im_gray) 
    {
        int num_pixels = im_rgb.num_columns() * im_rgb.num_rows();
        for (int i = 0; i < num_pixels; ++i)
            im_gray(0)(i) = (im_rgb(0)(i) + im_rgb(1)(i) + im_rgb(2)(i)) / 3;
    }
}



