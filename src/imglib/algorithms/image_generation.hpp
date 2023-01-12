#pragma once

#include <algorithm>
#include <imglib/image/image.hpp>

namespace imglib::algorithm
{
    template<typename T, template<typename> class colorT>
    void do_block(Image<T>& image, int top, int left, int bottom, int right, colorT<T> color, color_rgb_tag)
    {
        for (int i = top; i <= bottom; ++i)
        {
            std::fill(image(0).row_iterator(i, left), image(0).row_iterator(i, right + 1), color.r);
            std::fill(image(1).row_iterator(i, left), image(1).row_iterator(i, right + 1), color.b);
            std::fill(image(2).row_iterator(i, left), image(2).row_iterator(i, right + 1), color.g);
        }
    }

    template<typename T, template<typename> class colorT>
    void do_block(Image<T>& image, int top, int left, int bottom, int right, colorT<T> color, color_mono_tag)
    {
        for (int i = top; i <= bottom; ++i)
            std::fill(image(0).row_iterator(i, left), image(0).row_iterator(i, right), color.v);
    }

    // Draw a rectangle on the image with the given color
    template<typename T, template<typename> class colorT>
    void block(Image<T>& image, int top, int left, int bottom, int right, colorT<T> color)
    {
        do_block(image, top, left, bottom, right, color, typename color_traits<T, colorT>::color_space_category());
    }

    template<typename T, template<typename> class colorT>
    void do_bars(Image<T>& image, colorT<T> first_color, T increment, int bar_width, color_mono_tag)
    {
        // compute the first row
        T value = first_color.v - increment;
        for (int i = 0; i < image.get_width(); ++i)
        {
            if (i % bar_width == 0)
            {
                value += increment;
                if (value > std::numeric_limits<T>::max())
                {
                    value = first_color.v;
                }
            }
            image(0)(0, i) = value;
        }
        // copy the top row into all the others
        for (int i = 1; i < image.get_height(); ++i)
        {
            std::copy(image(0).row_begin(0), image(0).row_end(0), image(0).row_begin(i));
        }
    }

    template<typename T, template<typename> class colorT>
    void do_bars(Image<T>& image, colorT<T> first_color, T increment, int bar_width, color_rgb_tag)
    {
        // compute the first row
        T value_r = first_color.r - increment;
        T value_g = first_color.g - increment;
        T value_b = first_color.b - increment;
        for (int i = 0; i < image.get_width(); ++i)
        {
            if (i % bar_width == 0)
            {
                value_r += increment;
                value_g += increment;
                value_b += increment;
                if (value_r > std::numeric_limits<T>::max())
                {
                    value_r = first_color.r;
                }
                if (value_g > std::numeric_limits<T>::max())
                {
                    value_g = first_color.g;
                }
                if (value_b > std::numeric_limits<T>::max())
                {
                    value_b = first_color.b;
                }
            }
            image(0)(0, i) = value_r;
            image(1)(0, i) = value_g;
            image(2)(0, i) = value_b;
        }
        // copy the top row into all the others
        for (int i = 1; i < image.get_height(); ++i)
        {
            std::copy(image(0).row_begin(0), image(0).row_end(0), image(0).row_begin(i));
            std::copy(image(1).row_begin(0), image(1).row_end(0), image(1).row_begin(i));
            std::copy(image(2).row_begin(0), image(2).row_end(0), image(2).row_begin(i));
        }
    }

    template<typename T, template<typename> class colorT>
    void bars(Image<T>& image, colorT<T> firstColor, T increment, int barWidth)
    {
        do_bars(image, firstColor, increment, barWidth, typename color_traits<T, colorT>::color_space_category());
    }
}
