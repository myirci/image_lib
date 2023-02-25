#pragma once

#include <algorithm>
#include <imglib/image/image.hpp>
#include <imglib/image/cimage.hpp>
#include <imglib/image/pimage.hpp>
#include <imglib/utility/simple_geometry.hpp>
#include <imglib/color/color.hpp>

namespace imglib::algorithm
{
    template <typename T, std::convertible_to<T>... U>
        requires (sizeof...(U) >= 1)
    void block(Image<T>& img, const Rectangle2D<size_t>& box, U... vals)
    {
        if (img.num_channels() != sizeof...(U))
            throw std::invalid_argument("Channel number mismatch.");

        size_t i{ 0 };
        for (auto val : { vals... }) 
        {
            for (size_t j = box.top_left()(0); j <= box.bottom_right()(0); j++)
                std::fill(img(i).git(j, box.top_left()(1)), img(i).git(j, box.bottom_right()(1) + 1), val);

            i++;
        }
    }

    template <typename T, std::convertible_to<T>... U>
        requires (sizeof...(U) >= 1)
    void bars(Image<T>& img, size_t width, T increment, U... vals)
    {
        if (img.num_channels() != sizeof...(U))
            throw std::invalid_argument("Channel number mismatch.");

        if (width == 0)
            throw std::invalid_argument("Bar width should be greater than 0.");

        size_t i{ 0 };
        for (auto val : { vals... })
        {
            // compute first row
            T v = val;
            for (size_t j = 0; j < img.width(); ++j) 
            {
                if (j != 0 && j % width == 0)
                {
                    if (std::numeric_limits<T>::max() - v > increment)
                        v += increment;
                    else
                        v = val;
                }

                img(i)(0, j) = v;
            }

            // copy first row to the other rows
            for (size_t j = 1; j < img.height(); ++j)
                std::copy(img(i).row_begin(0), img(i).row_end(0), img(i).row_begin(j));

            i++;
        }
    }

    template <typename T, size_t NumChannels>
    Image<T> horizontal_linear_gradient(size_t imgSize, size_t sectionSize, size_t numStops, const Color<T, NumChannels>& start, const Color<T, NumChannels>& end)
    {
        if (start.color_space() != end.color_space())
            throw std::invalid_argument("Color space mismatch");
        
        auto colors = GetColorStops(start, end, numStops);

        size_t width = sectionSize * numStops;
        auto img = Image<T>{ imgSize, width, start.color_space(), NumChannels };

        // set the initial row
        size_t k{ 0 };
        for (size_t i = 0; i < numStops; i++)
            for (size_t j = 0; j < sectionSize; j++)
                img.set_pixel(k++, colors[i]);

        // set the rest of the rows
        for (size_t i = 0; i < NumChannels; i++)
            for (size_t j = 1; j < imgSize; j++)
                std::copy(img(i).row_begin(0), img(i).row_end(0), img(i).row_begin(j));

        return img;
    }
}
