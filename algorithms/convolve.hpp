#include "image.hpp"
#include <vector>
#include <limits>
#include <iomanip>
#include <cmath>

namespace SmpImgLib
{
    struct filter_kernel
    {
    public:
        typedef std::vector<int>::reference reference;
        typedef std::vector<int>::const_reference const_reference;
        // filter_kernel(int rows, int columns, double fac = 1.0) : r(rows), c(columns), factor(fac), kernel(rows*columns, 0) { }
        filter_kernel(int rows, int columns, double fac = 1.0, int ival = 0) : r(rows), c(columns), factor(fac), kernel(rows* columns, ival) { }
        filter_kernel(const std::vector<int>& filter, int rows, int columns, double fac = 1.0) :
            kernel(filter.begin(), filter.end()), r(rows), c(columns), factor(fac) { }
        reference operator()(int i, int j) {
            return kernel[convert(i, j)];
        }
        const_reference operator()(int i, int j) const {
            return kernel[convert(i, j)];
        }
        bool is_valid() const {
            if ((((r - 1) % 2) == 0) && (((r - 1) % 2) == 0)) {
                return true;
            }
            return false;
        }
        void print_kernel() const {
            std::cout << "-------------------------------------" << std::endl;
            for (int j = -((r - 1) / 2); j <= ((r - 1) / 2); ++j) {
                for (int i = -((c - 1) / 2); i <= ((c - 1) / 2); ++i) {
                    std::cout << std::setw(3) << kernel[convert(i, j)] << " ";
                }
                std::cout << std::endl;
            }
            std::cout << "-------------------------------------" << std::endl;
        }
        int rows() const {
            return r;
        }
        int columns() const {
            return c;
        }
        double get_factor() const {
            return factor;
        }
    private:
        int convert(int i, int j) const { // i: column (horizontal) index, j: row (vertical) index
            return (j + (r - 1) / 2) * c + i + (c - 1) / 2;
        }
        int r;
        int c;
        double factor;
        std::vector<int> kernel;
    };

    template <typename T>
    void apply_linear_filter(Image<T>& in_img, const filter_kernel& kernel, Image<T>& out_img) {
        // check if the filter is valid or not
        if (!kernel.is_valid()) {
            std::cerr << "The number of filter rows and columns must be an odd number" << std::endl;
            return;
        }
        else if (kernel.rows() > in_img.num_rows() || kernel.columns() > in_img.num_columns()) {
            std::cerr << "The filter must be smaller than the image" << std::endl;
            return;
        }

        // clear the output image and modify its parameters
        out_img.clear();
        out_img.set_rows(in_img.num_rows());
        out_img.set_columns(in_img.num_columns());
        out_img.set_channels(in_img.num_channels());
        out_img.get_buffer_size();
        out_img.set_color_space(in_img.get_color_space());
        for (int i = 0; i < in_img.num_channels(); ++i) {
            out_img.insert_channel(Channel<T>(out_img.num_rows(), out_img.num_columns()));
        }

        int row_start = (kernel.rows() - 1) / 2;
        int row_end = in_img.num_rows() - row_start;
        int col_start = (kernel.columns() - 1) / 2;
        int col_end = in_img.num_columns() - col_start;

        double sum;
        for (int t = 0; t < in_img.num_channels(); ++t) {
            for (int u = row_start; u < row_end; ++u) {
                for (int v = col_start; v < col_end; ++v) {
                    sum = 0;
                    for (int j = -row_start; j <= row_start; ++j) {
                        for (int i = -col_start; i <= col_start; ++i) {
                            // std::cerr << "i: " << i << " j: " << j << " u: " << u << " v: " << v;
                            sum = sum + kernel(i, j) * in_img(t)(u + j, v + i);
                        }
                    }
                    sum *= kernel.get_factor();
                    if (sum > std::numeric_limits<T>::max()) {
                        out_img(t)(u, v) = std::numeric_limits<T>::max();
                    }
                    else if (sum < std::numeric_limits<T>::min()) {
                        out_img(t)(u, v) = std::numeric_limits<T>::min();
                    }
                    else {
                        out_img(t)(u, v) = static_cast<T>(std::floor(sum + 0.5));
                    }
                }
            }
        }
    }
}