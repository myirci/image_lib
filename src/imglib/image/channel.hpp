#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <memory>
#include <algorithm>

#include <iterator>

namespace imglib
{
    template <typename T>
    class ChannelIterator 
    {
    public:
        
        using iterator_category = std::contiguous_iterator_tag;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;
        using difference_type   = std::ptrdiff_t;

        // Default constructible -> required by the std::forward_iterator concept
        ChannelIterator() noexcept = default;

        // Constructor
        explicit ChannelIterator(T* ptr) noexcept : m_ptr{ ptr } { }

        // Dereferencable -> required by the std::input_or_output_iterator concept 
        // Note that although the pointee can be modified using the returned reference, the data member of this class is the pointer and this method does not modify it, therefore marked with const specifier. 
        reference operator*() const { return *m_ptr; }

        // Pre-incrementable -> required by the std::input_or_output_iterator concept 
        ChannelIterator<T>& operator++() noexcept
        {
            ++m_ptr;
            return *this;
        }
        
        // Post-incrementable -> required by the std::input_or_output_iterator concept 
        ChannelIterator<T> operator++(int) 
        {
            auto copy = *this;
            ++m_ptr;
            return copy;
        }

        // Pre-decrementable -> required by the std::bidirectional_iterator concept 
        ChannelIterator<T>& operator--() noexcept
        {
            --m_ptr;
            return *this;
        }

        // Post-decrementable -> required by the std::bidirectional_iterator concept 
        ChannelIterator<T> operator--(int)
        {
            auto copy = *this;
            --m_ptr;
            return copy;
        }

        // Equality -> required by the std::input_iterator concept 
        bool operator==(const ChannelIterator<T>& other) const noexcept { return m_ptr == other.m_ptr; }

        // Inequality
        bool operator!=(const ChannelIterator<T>& other) const noexcept { return m_ptr != other.m_ptr; }

        // Compound addition assignment -> required by the std::random_access_iterator concept
        ChannelIterator<T>& operator+=(const difference_type n) noexcept
        {
            m_ptr += n;
            return *this;
        }

        // Compound substraction assignment -> required by the std::random_access_iterator concept
        ChannelIterator<T>& operator-=(const difference_type n) noexcept
        {
            m_ptr -= n;
            return *this;
        }

        // Addition, substraction -> required by the std::random_access_iterator concept
        ChannelIterator<T> operator+(const difference_type n) const noexcept { return ChannelIterator<T>(m_ptr + n); }
        ChannelIterator<T> operator-(const difference_type n) const noexcept { return ChannelIterator<T>(m_ptr - n); }

        friend ChannelIterator<T> operator+(const difference_type n, const ChannelIterator<T>& it) noexcept     { return it + n; }
        friend difference_type operator-(const ChannelIterator<T>& it1, const ChannelIterator<T>& it2) noexcept { return it1.m_ptr - it2.m_pt2; }

        // Subscripting -> required by the std::random_access_iterator concept
        reference operator[](const difference_type n) const { return *(m_ptr + n); }

        // Less than, less than or equal to, greather than, greater than or equal to -> required by the std::random_access_iterator concept
        bool operator<(const ChannelIterator<T>& other) const noexcept  { return m_ptr < other.m_ptr;  }
        bool operator<=(const ChannelIterator<T>& other) const noexcept { return m_ptr <= other.m_ptr; }
        bool operator>(const ChannelIterator<T>& other) const noexcept  { return m_ptr > other.m_ptr;  }
        bool operator>=(const ChannelIterator<T>& other) const noexcept { return m_ptr >= other.m_ptr; }

        // required by the std::contiguous_iterator concept
        const pointer operator->() const noexcept { return m_ptr; }

    private:
        pointer m_ptr{ nullptr };
    };

    template<typename T>
    class Channel
    {
    private:
        int m_numCols{ 0 };
        int m_numRows{ 0 };
        // std::unique_ptr<T[]> m_pixels{ nullptr };
        std::vector<T> m_data;

    public:
        using iterator        = typename std::vector<T>::iterator;
        using const_iterator  = typename std::vector<T>::const_iterator;

        Channel() { }
        
        Channel(int numRows, int numCols, T val = T()) :
            m_numRows{ numRows }, 
            m_numCols{ numCols },
            // m_pixels{ std::make_unique<T[]>(numRows * numCols) },
            m_data(numRows * numCols, val) 
        {
            //std::fill(*m_pixels[0])
        }

        T& operator()(int row, int col) { return m_data[to_index(row, col)]; }
        
        T& operator()(int index) { return m_data[index]; }
        
        T const& operator()(int row, int col) const { return m_data[to_index(row, col)]; }
        
        T const& operator()(int index) const { return m_data[index]; }
        
        void operator=(T value) { m_data.assign(m_data.size(), value); }

        iterator begin() { return m_data.begin(); }

        const_iterator begin() const { return m_data.begin(); }

        iterator end() { return m_data.end(); }

        const_iterator end() const { return m_data.end(); }

        iterator row_begin(int row) { return m_data.begin() + row * m_numCols; }

        const_iterator row_begin(int row) const { return m_data.begin() + row * m_numCols; }

        iterator row_end(int row) { return row_begin(row + 1); }

        const_iterator row_end(int row) const { return row_begin(row + 1); }

        iterator row_iterator(int row, int col) { return row_begin(row) + col; }

        const_iterator row_iterator(int row, int col) const { return row_begin(row) + col; }

        void clear();
        
        bool empty() const { return m_data.empty(); }

        int num_columns() const noexcept { return m_numCols; }
        
        int num_rows() const noexcept { return m_numRows; }

        void resize(int numRows, int numCols);
       
    private:
        int to_index(int row, int col) const { return row * m_numCols + col; }
    };
   
    template <typename T>
    void Channel<T>::clear()
    {
        m_data.clear();
        m_numRows = 0;
        m_numCols = 0;
    }

    template <typename T>
    void Channel<T>::resize(int numRows, int numCols)
    {
        if (numRows == m_numRows && numCols == m_numCols)
            return;

        m_data.clear();
        m_data.resize(numRows * numCols);
        m_numRows = numRows;
        m_numCols = numCols;
    }
}
