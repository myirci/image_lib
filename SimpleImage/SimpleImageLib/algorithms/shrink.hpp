#pragma once

#include <Image/Image.hpp>

#include <iostream>
#include <algorithm>
#include <numeric>

namespace imglib
{
    namespace algorithm 
    {
        template<typename T>
        Image<T> shrink(const Image<T>& inImg, int amount)
        {
            if (inImg(0).empty())
            {
                std::cerr << "Input image size is 0" << std::endl;
                exit(1);
            }

            if (inImg.get_height() / amount == 0 || inImg.get_width() / amount == 0)
            {
                std::cerr << "Shrink amount should not bigger than then the width or height of the image" << std::endl;
                exit(2);
            }

            if (amount == 1) return inImg;

            Image<T> outImg(inImg.get_height() / amount, inImg.get_width() / amount, inImg.get_color_space(), inImg.get_num_channels());
            int divisor{ amount * amount };

            for (int t{ 0 }; t < outImg.get_num_channels(); ++t)
            {
                int inImgRow{ 0 };
                for (int i{ 0 }; i < outImg.get_height(); ++i, inImgRow += amount)
                {
                    int inImgCol{ 0 };
                    for (int j{ 0 }; j < outImg.get_width(); ++j, inImgCol += amount)
                    {
                        double sum{ 0 };
                        for (int k{ inImgRow }; k < inImgRow + amount; ++k)
                            sum = std::accumulate(inImg(t).row_iterator(k, inImgCol), inImg(t).row_iterator(k, inImgCol + amount), sum);

                        outImg(t)(i, j) = static_cast<T>(sum / divisor);
                    }
                }
            }
            return outImg;
        }
    }
}

