#pragma once

#include <imglib/image/image.hpp>

#include <iostream>
#include <algorithm>
#include <numeric>

namespace imglib::algorithm
{
    template<typename T>
    Image<T> shrink(const Image<T>& inImg, int amount)
    {
        if (inImg(0).empty())
        {
            std::cerr << "Input image size is 0" << std::endl;
            exit(1);
        }

        if (inImg.height() / amount == 0 || inImg.width() / amount == 0)
        {
            std::cerr << "Shrink amount should not bigger than then the width or height of the image" << std::endl;
            exit(2);
        }

        if (amount == 1) return inImg;

        Image<T> outImg(inImg.height() / amount, inImg.width() / amount, inImg.color_space(), inImg.num_channels());
        int divisor{ amount * amount };

        for (int t{ 0 }; t < outImg.num_channels(); ++t)
        {
            int inImgRow{ 0 };
            for (int i{ 0 }; i < outImg.height(); ++i, inImgRow += amount)
            {
                int inImgCol{ 0 };
                for (int j{ 0 }; j < outImg.width(); ++j, inImgCol += amount)
                {
                    double sum{ 0 };
                    for (int k{ inImgRow }; k < inImgRow + amount; ++k)
                        sum = std::accumulate(inImg(t).get_iterator(k, inImgCol), inImg(t).get_iterator(k, inImgCol + amount), sum);

                    outImg(t)(i, j) = static_cast<T>(sum / divisor);
                }
            }
        }
        return outImg;
    }
}

