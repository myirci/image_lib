#ifndef MODIFIEDAUTOCONTRAST_HPP
#define MODIFIEDAUTOCONTRAST_HPP

#include "../Image.hpp"
#include <vector>
#include <algorithm>
#include <limits>

template <typename T>
struct func_modified_auto_contrast {
    func_modified_auto_contrast(T lp, T hp) : low_p(lp), high_p(hp) { }
    void operator()(T& val) {
        val = std::numeric_limits<T>::min() +
                (val - low_p) * ((std::numeric_limits<T>::max() - std::numeric_limits<T>::min())/( high_p - low_p));
    }
    T low_p;
    T high_p;
};

template <typename T>
void modified_auto_contrast(Image<T>& image, double q_low, double q_high) {
    std::cout << "This function only supports JSAMPLE for now." << std::endl;
}

template < >
void modified_auto_contrast(Image<JSAMPLE>& image, double q_low, double q_high) {

    // need to determine lower and upper limits
    int num_pixels = image.num_columns() * image.num_rows();
    int lower_count = static_cast<int>(num_pixels * q_low);
    int upper_count = static_cast<int>(num_pixels * (1 - q_high));

    std::vector<int> histogram(std::numeric_limits<JSAMPLE>::max() + 1, 0);
    std::vector<int> cumulative_his(std::numeric_limits<JSAMPLE>::max() + 1, 0);
    ImageChannel<JSAMPLE>::const_iterator it;
    for(int i = 0; i < image.num_channels(); ++i) {
        // calculate histogram
        for(it = image(i).begin(); it != image(i).end(); ++it) {
            histogram[static_cast<int>(*it)]++;
        }
        // cumulative histogram
        cumulative_his[0] = histogram[0];
        for(int j = 1; j < std::numeric_limits<JSAMPLE>::max() + 1; ++j) {
            cumulative_his[j] = cumulative_his[j-1] + histogram[j];
        }
        // detemine lower limit
        JSAMPLE alow = 0;
        for(int k = 0; k < cumulative_his.size(); ++k) {
            if(cumulative_his[k] >= lower_count) {
                alow = k;
                break;
            }
        }
        // determine upper limit
        JSAMPLE ahigh = 0;
        for(int t = cumulative_his.size() - 1; t >= 0; --t) {
            if(cumulative_his[t] <= upper_count) {
                ahigh = t;
                break;
            }
        }
        func_modified_auto_contrast<JSAMPLE> fobj(alow, ahigh);
        std::for_each(image(i).begin(), image(i).end(), fobj);
        histogram.clear();
        histogram.resize(std::numeric_limits<JSAMPLE>::max() + 1, 0);
        cumulative_his.clear();
        cumulative_his.resize(std::numeric_limits<JSAMPLE>::max() + 1, 0);
    }
}

#endif // MODIFIEDAUTOCONTRAST_HPP
