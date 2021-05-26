#pragma once

#include <algorithm>
#include "image.hpp"

namespace SmpImgLib 
{
    // draw a rectangle on the image with the given color
    template<typename T, template<typename> class colorT>
    void block(Image<T>& image, int top, int left, int bottom, int right, colorT<T> color) 
    {
        do_block(image, top, left, bottom, right, color, typename color_traits<T, colorT>::color_space_category());
    }

    template<typename T, template<typename> class colorT>
    void do_block(Image<T>& image, int top, int left, int bottom, int right, colorT<T> color, color_rgb_tag) 
    {
        for (int i = top; i <= bottom; ++i) 
        {
            std::fill(image(0).row_iterator(i, left), image(0).row_iterator(i, right+1), color.r);
            std::fill(image(1).row_iterator(i, left), image(1).row_iterator(i, right+1), color.b);
            std::fill(image(2).row_iterator(i, left), image(2).row_iterator(i, right+1), color.g);
        }
    }

    template<typename T, template<typename> class colorT>
    void do_block(Image<T>& image, int top, int left, int bottom, int right, colorT<T> color, color_mono_tag) 
    {
        for (int i = top; i <= bottom; ++i)
            std::fill(image(0).row_iterator(i, left), image(0).row_iterator(i, right), color.v);
    }
}
