#pragma once

#include "image.hpp"

// draw a rectangle of one color in the image

namespace SmpImgLib 
{
    template<typename T, template<typename> class colorT>
    void block(Image<T>& image, int top, int left, int bottom, int right, colorT<T> color) {
        do_block(image, top, left, bottom, right, color, typename color_traits<T, colorT>::color_space_category());
    }

    template<typename T, template<typename> class colorT>
    void do_block(Image<T>& image, int top, int left, int bottom, int right, colorT<T> color, color_rgb_tag) {
        for (int i = top; i <= bottom; ++i) {
            std::fill(image(0).row_begin(i, left), image(0).row_end(i, right), color.r);
            std::fill(image(1).row_begin(i, left), image(1).row_end(i, right), color.b);
            std::fill(image(2).row_begin(i, left), image(2).row_end(i, right), color.g);
        }
    }

    template<typename T, template<typename> class colorT>
    void do_block(Image<T>& image, int top, int left, int bottom, int right, colorT<T> color, color_mono_tag) {
        for (int i = top; i <= bottom; ++i) {
            std::fill(image(0).row_begin(i, left), image(0).row_end(i, right), color.v);
        }
    }
}


