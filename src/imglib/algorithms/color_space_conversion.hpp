#include <imglib/image/image.hpp>

namespace imglib::algorithm
{
    template<typename T>
    void convert_rgb_to_grayscale(const Image<T>& img_rgb, Image<T>& img_gray)
    {
        size_t num_pixels = img_rgb.width() * img_rgb.height();
        for (size_t i = 0; i < num_pixels; ++i)
            img_gray(0)(i) = (img_rgb(0)(i) + img_rgb(1)(i) + img_rgb(2)(i)) / 3;
    }
}



