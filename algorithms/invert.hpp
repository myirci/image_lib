#pragma once

#include "image.hpp"

namespace SmpImgLib
{
    template <typename T>
    struct func_invert {
        void operator()(T& val) {
            val = std::numeric_limits<T>::max() - val;
        }
    };

    template <typename T>
    void invert(Image<T>& image) {
        func_invert<T> fobj;
        for (int i = 0; i < image.num_channels(); ++i) {
            std::for_each(image(i).begin(), image(i).end(), fobj);
        }
    }
}