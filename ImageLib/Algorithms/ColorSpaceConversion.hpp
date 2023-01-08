#include <Image/Image.hpp>

namespace imglib
{
    namespace algorithm
    {
        template<typename T>
        void convert_rgb_to_grayscale(const Image<T>& img_rgb, Image<T>& img_gray)
        {
            int num_pixels = img_rgb.get_width() * img_rgb.get_height();
            for (int i = 0; i < num_pixels; ++i)
                img_gray(0)(i) = (img_rgb(0)(i) + img_rgb(1)(i) + img_rgb(2)(i)) / 3;
        }
    }
}



