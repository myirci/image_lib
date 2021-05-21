#pragma once

#include "image.hpp"
#include <algorithm>
#include <limits>

template <typename T>
struct func_contrast {
    func_contrast(double c_) : c(c_), r(0) { }
    void operator()(T& val) {
        r = val * c;
        if(r > std::numeric_limits<T>::max()) {
            val = std::numeric_limits<T>::max();
        }
        else {
            val = static_cast<T>(r);
        }
    }
    double c;
    double r;
};

template <typename T>
void contrast(Image<T>& image, double val) {
    func_contrast<T> fobj(val);
    for(int i = 0; i < image.num_channels(); ++i) {
        std::for_each(image(i).begin(), image(i).end(), fobj);
    }
}