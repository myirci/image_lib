#pragma once

#include <imglib/image/image.hpp>

#include <algorithm>
#include <limits>
#include <vector>

namespace imglib::algorithm
{
    template <typename T>
    void contrast(Image<T>& image, double val)
    {
        for (size_t i{ 0 }; i < image.num_channels(); ++i)
            std::for_each(image(i).begin(), image(i).end(), [val](T& intensity) 
                {
                    double newIntensity{ intensity * val };
                    intensity = newIntensity > std::numeric_limits<T>::max() ? std::numeric_limits<T>::max() : static_cast<T>(newIntensity);
                });
    }

    template <typename T>
    void invert(Image<T>& image)
    {
        for (size_t i = 0; i < image.num_channels(); ++i)
            std::for_each(image(i).begin(), image(i).end(), [](T& val) { val = std::numeric_limits<T>::max() - val; });
    }
}