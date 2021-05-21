#pragma once

#include "image.hpp"
#include <algorithm>
#include <numeric>

namespace SmpImgLib
{
    template<typename T>
    void shrink(Image<T>& in_img, int amount, Image<T>& out_img) {
        if (in_img(0).empty()) {
            return;
        }
        else if (in_img.num_rows() / amount == 0 | in_img.num_columns() == 0) {
            std::cerr << "Shrink amount should not bigger than then the width or height of the image" << std::endl;
            return;
        }
        else if (amount == 1) {
            out_img.set_rows(in_img.num_rows());
            out_img.set_columns(in_img.num_columns());
            out_img.set_channels(in_img.num_channels());
            out_img.calculate_buffer_size();
            out_img.set_color_space(in_img.get_color_space());
            for (int i = 0; i < in_img.num_channels(); ++i) {
                if (!out_img.insert_channel(in_img(i))) {
                    std::cerr << "Channel insertion error!" << std::endl;
                    return;
                }
            }
            return;
        }

        out_img.set_rows(in_img.num_rows() / amount);
        out_img.set_columns(in_img.num_columns() / amount);
        out_img.set_channels(in_img.num_channels());
        out_img.calculate_buffer_size();
        out_img.set_color_space(in_img.get_color_space());

        for (int i = 0; i < in_img.num_channels(); ++i) {
            out_img.insert_channel(Channel<T>(out_img.num_rows(), out_img.num_columns()));
        }

        int divisor = amount * amount;
        for (int t = 0; t < out_img.num_channels(); ++t) {
            int in_img_row = 0;
            for (int i = 0; i < out_img.num_rows(); ++i, in_img_row += amount) {
                int in_img_col = 0;
                for (int j = 0; j < out_img.num_columns(); ++j, in_img_col += amount) {
                    int sum = 0;
                    for (int k = in_img_row; k < in_img_row + amount; ++k) {
                        sum += std::accumulate(in_img(t).row_begin(k, in_img_col),
                            in_img(t).row_end(k, in_img_col + amount - 1), 0);
                    }
                    out_img(t)(i, j) = static_cast<T>(sum / divisor);
                }
            }
        }
    }

}

