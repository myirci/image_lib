#pragma once

#include <image/image.hpp>

#include <algorithm>
#include <limits>
#include <vector>

namespace SmpImgLib
{
    namespace Algorithm 
    {
        template <typename T>
        void contrast(Image<T>& image, double val)
        {
            for (int i{ 0 }; i < image.get_num_channels(); ++i)
                std::for_each(image(i).begin(), image(i).end(), [val](T& intensity)
                    {
                        double newIntensity{ intensity * val };
                        intensity = newIntensity > std::numeric_limits<T>::max() ? std::numeric_limits<T>::max() : static_cast<T>(newIntensity);
                    });
        }

        template <typename T>
        bool auto_contrast(Image<T>& image)
        {
            T lowVal, highVal;
            int auto_contrast_count = 0;
            for (int i{ 0 }; i < image.get_num_channels(); ++i)
            {
                lowVal = *std::min_element(image(i).begin(), image(i).end());
                highVal = *std::max_element(image(i).begin(), image(i).end());
                if (lowVal == highVal || (lowVal == std::numeric_limits<T>::min() && highVal == std::numeric_limits<T>::max()))
                {
                    auto_contrast_count++;
                    continue;
                }
                std::for_each(image(i).begin(), image(i).end(), [lowVal, highVal](T& val)
                    {
                        val = std::numeric_limits<T>::min() + (val - lowVal) * ((std::numeric_limits<T>::max() - std::numeric_limits<T>::min()) / (highVal - lowVal));
                    });
            }
            return auto_contrast_count != image.get_num_channels();
        }

        template <typename T>
        void modified_auto_contrast(Image<T>& img, double quantileLow, double quantileHigh)
        {
            int numPixels = img.get_width() * img.get_height();
            int lowerCount = static_cast<int>(numPixels * quantileLow);
            int upperCount = static_cast<int>(numPixels * (1 - quantileHigh));

            std::vector<int> histogram(std::numeric_limits<T>::max() + 1, 0);
            std::vector<int> cumulativeHis(std::numeric_limits<T>::max() + 1, 0);
            typename Channel<T>::const_iterator it;
            for (int i{ 0 }; i < img.get_num_channels(); ++i)
            {
                // histogram
                for (it = img(i).begin(); it != img(i).end(); ++it)
                    histogram[static_cast<int>(*it)]++;

                // cumulative histogram
                cumulativeHis[0] = histogram[0];
                for (int j{ 1 }; j < std::numeric_limits<T>::max() + 1; ++j)
                    cumulativeHis[j] = cumulativeHis[j - 1] + histogram[j];

                // detemine lower limit
                T valLow{ 0 };
                for (int k{ 0 }; k < cumulativeHis.size(); ++k)
                {
                    if (cumulativeHis[k] >= lowerCount)
                    {
                        valLow = k;
                        break;
                    }
                }

                // determine upper limit
                T valHigh{ 0 };
                for (int t{ static_cast<int>(cumulativeHis.size()) - 1 }; t >= 0; --t)
                {
                    if (cumulativeHis[t] <= upperCount)
                    {
                        valHigh = t;
                        break;
                    }
                }

                std::for_each(img(i).begin(), img(i).end(), [valLow, valHigh](T& val)
                    {
                        val = std::numeric_limits<T>::min() + (val - valLow) * ((std::numeric_limits<T>::max() - std::numeric_limits<T>::min()) / (valHigh - valLow));
                    });

                histogram.clear();
                histogram.resize(std::numeric_limits<T>::max() + 1, 0);
                cumulativeHis.clear();
                cumulativeHis.resize(std::numeric_limits<T>::max() + 1, 0);
            }
        }

        template <typename T>
        void invert(Image<T>& image)
        {
            for (int i = 0; i < image.get_num_channels(); ++i)
                std::for_each(image(i).begin(), image(i).end(), [](T& val) { val = std::numeric_limits<T>::max() - val; });
        }
    }
}