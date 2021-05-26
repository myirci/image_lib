#pragma once

#include <algorithm>
#include "image.hpp"


namespace SmpImgLib
{
    template <typename T>
    void invert(Image<T>& image)
    {
        for (int i = 0; i < image.get_num_channels(); ++i)
            std::for_each(image(i).begin(), image(i).end(), [](T& val) { val = std::numeric_limits<T>::max() - val; });
    }
}