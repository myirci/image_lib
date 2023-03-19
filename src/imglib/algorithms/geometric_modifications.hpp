#pragma once

#include <imglib/image/image.hpp>

#include <iostream>
#include <algorithm>
#include <numeric>

namespace imglib::algorithm
{
    template<typename T>
    Image<T> Shrink(const Image<T>& img, size_t amount)
    {
        if (img.size() == 0 || amount > img.height() || amount > img.width())
            throw std::invalid_argument("At least one of the arguments is invalid.");
       
        if (amount == 1) 
            return img;

        Image<T> outImg(img.height() / amount, img.width() / amount, img.color_space(), img.num_channels());
        size_t divisor{ amount * amount };

        for (size_t t = 0; t < outImg.num_channels(); ++t)
        {
            size_t imgRow{ 0 };
            for (size_t i = 0; i < outImg.height(); ++i, imgRow += amount)
            {
                size_t imgCol{ 0 };
                for (size_t j = 0; j < outImg.width(); ++j, imgCol += amount)
                {
                    double sum{ 0 };
                    for (size_t k = imgRow; k < imgRow + amount; ++k)
                        sum = std::accumulate(img(t).git(k, imgCol), img(t).git(k, imgCol + amount), sum);

                    outImg(t)(i, j) = static_cast<T>(sum / divisor);
                }
            }
        }
        return outImg;
    }

    template<typename T>
    Image<T> Shrink2(const Image<T>& img, double amount)
    {
        if (img.size() == 0 || amount > 1.0 || amount < img.width())
            throw std::invalid_argument("At least one of the arguments is invalid.");

        if (amount == 1)
            return img;

        Image<T> outImg(img.height() / amount, img.width() / amount, img.color_space(), img.num_channels());
        size_t divisor{ amount * amount };

        for (size_t t = 0; t < outImg.num_channels(); ++t)
        {
            size_t imgRow{ 0 };
            for (size_t i = 0; i < outImg.height(); ++i, imgRow += amount)
            {
                size_t imgCol{ 0 };
                for (size_t j = 0; j < outImg.width(); ++j, imgCol += amount)
                {
                    double sum{ 0 };
                    for (size_t k = imgRow; k < imgRow + amount; ++k)
                        sum = std::accumulate(img(t).git(k, imgCol), img(t).git(k, imgCol + amount), sum);

                    outImg(t)(i, j) = static_cast<T>(sum / divisor);
                }
            }
        }
        return outImg;
    }
}

