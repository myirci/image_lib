#include <Image/Image.hpp>

#include <vector>
#include <limits>
#include <iomanip>
#include <cmath>

namespace imglib
{
    class Filter
    {
    public:
        
        using reference = std::vector<int>::reference;
        using const_reference = std::vector<int>::const_reference;
        
        Filter(int rows, int columns, double fac = 1.0, int initialVal = 0) :
            m_rows{ rows }, m_columns{ columns }, m_scaleFactor{ fac }, m_filterMatrix(rows * columns, initialVal) { }
        
        Filter(const std::vector<int>& filter, int rows, int columns, double fac = 1.0) :
            m_filterMatrix(filter.begin(), filter.end()), m_rows{ rows }, m_columns{ columns }, m_scaleFactor{ fac } { }
        
        reference operator()(int i, int j) { return m_filterMatrix[convert(i, j)]; }
        
        const_reference operator()(int i, int j) const { return m_filterMatrix[convert(i, j)]; }
        
        int rows() const { return m_rows; }

        int columns() const { return m_columns; }

        double get_factor() const { return m_scaleFactor; }

        bool is_valid() const { return (((m_rows - 1) % 2) == 0) && (((m_columns - 1) % 2) == 0); }

        void print_kernel() const 
        {
            std::cout << "-------------------------------------" << std::endl;
            for (int j = -((m_rows - 1) / 2); j <= ((m_rows - 1) / 2); ++j)
            {
                for (int i = -((m_columns - 1) / 2); i <= ((m_columns - 1) / 2); ++i)
                    std::cout << std::setw(3) << m_filterMatrix[convert(i, j)] << " ";
                
                std::cout << std::endl;
            }
            std::cout << "-------------------------------------" << std::endl;
        }

    private:
        int convert(int i, int j) const // i: column (horizontal) index, j: row (vertical) index
        { 
            return (j + (m_rows - 1) / 2) * m_columns + i + (m_columns - 1) / 2;
        }
        int m_rows;
        int m_columns;
        double m_scaleFactor;
        std::vector<int> m_filterMatrix;
    };

    template <typename T>
    Image<T> apply_linear_filter(const Image<T>& inImg, const Filter& kernel)
    {
        // check if the filter is valid or not
        if (!kernel.is_valid())
        {
            std::cerr << "The number of filter rows and columns must be an odd number" << std::endl;
            exit(1);
        }
        else if (kernel.rows() > inImg.get_height() || kernel.columns() > inImg.get_width()) 
        {
            std::cerr << "The filter must be smaller than the image" << std::endl;
            exit(1);
        }

        Image<T> outImg(inImg);

        int row_start = (kernel.rows() - 1) / 2;
        int row_end = inImg.get_height() - row_start;
        int col_start = (kernel.columns() - 1) / 2;
        int col_end = inImg.get_width() - col_start;

        double sum;
        for (int t{ 0 }; t < inImg.get_num_channels(); ++t)
        {
            for (int u{ row_start }; u < row_end; ++u)
            {
                for (int v{ col_start } ; v < col_end; ++v)
                {
                    sum = 0;
                    for (int j{ -row_start }; j <= row_start; ++j)
                    {
                        for (int i{ -col_start }; i <= col_start; ++i)
                        {
                            // std::cerr << "i: " << i << " j: " << j << " u: " << u << " v: " << v;
                            sum = sum + kernel(i, j) * inImg(t)(u + j, v + i);
                        }
                    }
                    sum *= kernel.get_factor();
                    if (sum > std::numeric_limits<T>::max()) 
                    {
                        outImg(t)(u, v) = std::numeric_limits<T>::max();
                    }
                    else if (sum < std::numeric_limits<T>::min()) 
                    {
                        outImg(t)(u, v) = std::numeric_limits<T>::min();
                    }
                    else
                    {
                        outImg(t)(u, v) = static_cast<T>(std::floor(sum + 0.5));
                    }
                }
            }
        }

        return std::move(outImg);
    }
}