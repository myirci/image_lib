#pragma once

#include "image.hpp"
#include <algorithm>
#include <limits>

template <typename T>
struct func_auto_contrast {
    func_auto_contrast(T lp, T hp) : low_p(lp), high_p(hp) { }
    void operator()(T& val) {
        val = std::numeric_limits<T>::min() +
                (val - low_p) * ((std::numeric_limits<T>::max() - std::numeric_limits<T>::min())/( high_p - low_p));
    }
    T low_p;
    T high_p;
};

template <typename T>
bool auto_contrast(Image<T>& image) {
    T low_p, high_p;
    int auto_contrast_count = 0;
    for(int i = 0; i < image.num_channels(); ++i) {
        low_p = *std::min_element(image(i).begin(), image(i).end());
        high_p = *std::max_element(image(i).begin(), image(i).end());
        if(low_p == high_p || (low_p == std::numeric_limits<T>::min() && high_p == std::numeric_limits<T>::max())) {
            auto_contrast_count++;
            continue;
        }
        func_auto_contrast<T> fobj(low_p, high_p);
        std::for_each(image(i).begin(), image(i).end(), fobj);
    }
    if(auto_contrast_count == image.num_channels()) {
        return false;
    }
    else {
        return true;
    }
}
