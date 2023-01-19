#pragma once

#include <iostream>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <iterator>

#include <imglib/config.hpp>

namespace imglib
{
    template <typename T>
    class HorizontalValueIterator
    {
    public:

        using iterator_category = std::contiguous_iterator_tag;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;
        using difference_type   = std::ptrdiff_t;

        // Default constructible -> required by the std::forward_iterator concept
        HorizontalValueIterator() noexcept = default;

        // Constructor
        explicit HorizontalValueIterator(T* ptr) noexcept : m_ptr{ ptr } { }

        // Dereferencable -> required by the std::input_or_output_iterator concept 
        // Note that although the pointee can be modified using the returned reference, m_ptr is not modified, therefore marked with const specifier. 
        reference operator*() const { return *m_ptr; }

        // Pre-incrementable -> required by the std::input_or_output_iterator concept 
        HorizontalValueIterator<T>& operator++() noexcept
        {
            ++m_ptr;
            return *this;
        }

        // Post-incrementable -> required by the std::input_or_output_iterator concept 
        HorizontalValueIterator<T> operator++(int)
        {
            auto copy = *this;
            ++m_ptr;
            return copy;
        }

        // Pre-decrementable -> required by the std::bidirectional_iterator concept 
        HorizontalValueIterator<T>& operator--() noexcept
        {
            --m_ptr;
            return *this;
        }

        // Post-decrementable -> required by the std::bidirectional_iterator concept 
        HorizontalValueIterator<T> operator--(int)
        {
            auto copy = *this;
            --m_ptr;
            return copy;
        }

        // Equality -> required by the std::input_iterator concept 
        bool operator==(const HorizontalValueIterator<T>& other) const noexcept { return m_ptr == other.m_ptr; }

        // Inequality
        bool operator!=(const HorizontalValueIterator<T>& other) const noexcept { return m_ptr != other.m_ptr; }

        // Compound addition assignment -> required by the std::random_access_iterator concept
        HorizontalValueIterator<T>& operator+=(const difference_type n) noexcept
        {
            m_ptr += n;
            return *this;
        }

        // Compound substraction assignment -> required by the std::random_access_iterator concept
        HorizontalValueIterator<T>& operator-=(const difference_type n) noexcept
        {
            m_ptr -= n;
            return *this;
        }

        // Addition, substraction -> required by the std::random_access_iterator concept
        HorizontalValueIterator<T> operator+(const difference_type n) const noexcept { return HorizontalValueIterator<T>{ m_ptr + n }; }
        HorizontalValueIterator<T> operator-(const difference_type n) const noexcept { return HorizontalValueIterator<T>{ m_ptr - n }; }

        friend HorizontalValueIterator<T> operator+(const difference_type n, const HorizontalValueIterator<T>& it) noexcept { return it + n; }
        friend difference_type operator-(const HorizontalValueIterator<T>& it1, const HorizontalValueIterator<T>& it2) noexcept { return it1.m_ptr - it2.m_ptr; }

        // Subscripting -> required by the std::random_access_iterator concept
        reference operator[](const difference_type n) const { return *(m_ptr + n); }

        // Less than, less than or equal to, greather than, greater than or equal to -> required by the std::random_access_iterator concept
        bool operator<(const HorizontalValueIterator<T>& other) const noexcept { return m_ptr < other.m_ptr; }
        bool operator<=(const HorizontalValueIterator<T>& other) const noexcept { return m_ptr <= other.m_ptr; }
        bool operator>(const HorizontalValueIterator<T>& other) const noexcept { return m_ptr > other.m_ptr; }
        bool operator>=(const HorizontalValueIterator<T>& other) const noexcept { return m_ptr >= other.m_ptr; }

        // required by the std::contiguous_iterator concept
        const pointer operator->() const noexcept { return m_ptr; }

    private:
        pointer m_ptr{ nullptr };
    };
    
    template <typename T>
    class VerticalValueIterator
    {
    public:
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;
        using difference_type   = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;

        // Default constructible -> required by the std::forward_iterator concept
        VerticalValueIterator() noexcept = default;

        // Constructor
        explicit VerticalValueIterator(T* ptr, size_t numRows) noexcept : m_ptr{ ptr }, m_numRows{ numRows } { }

        // Dereferencable -> required by the std::input_or_output_iterator concept 
       // Note that although the pointee can be modified using the returned reference, m_ptr is not modified, therefore marked with const specifier. 
        reference operator*() const { return *m_ptr; }

        // Pre-incrementable -> required by the std::input_or_output_iterator concept 
        VerticalValueIterator<T>& operator++() noexcept
        {
            m_ptr += m_numRows;
            return *this;
        }

        // Post-incrementable -> required by the std::input_or_output_iterator concept 
        VerticalValueIterator<T> operator++(int)
        {
            auto copy = *this;
            m_ptr += m_numRows;
            return copy;
        }

        // Pre-decrementable -> required by the std::bidirectional_iterator concept 
        VerticalValueIterator<T>& operator--() noexcept
        {
            m_ptr -= m_numRows;
            return *this;
        }

        // Post-decrementable -> required by the std::bidirectional_iterator concept 
        VerticalValueIterator<T> operator--(int)
        {
            auto copy = *this;
            m_ptr -= m_numRows;
            return copy;
        }

        // Equality -> required by the std::input_iterator concept 
        bool operator==(const VerticalValueIterator<T>& other) const noexcept { return m_ptr == other.m_ptr && m_numRows == other.m_numRows; }

        // Inequality
        bool operator!=(const VerticalValueIterator<T>& other) const noexcept { return m_ptr != other.m_ptr || m_numRows != other.m_numRows; }

        // Compound addition assignment -> required by the std::random_access_iterator concept
        VerticalValueIterator<T>& operator+=(const difference_type n) noexcept
        {
            m_ptr += (n * m_numRows);
            return *this;
        }

        // Compound substraction assignment -> required by the std::random_access_iterator concept
        VerticalValueIterator<T>& operator-=(const difference_type n) noexcept
        {
            m_ptr -= (n * m_numRows);
            return *this;
        }

        // Addition, substraction -> required by the std::random_access_iterator concept
        VerticalValueIterator<T> operator+(const difference_type n) const noexcept { return VerticalValueIterator<T>{ m_ptr + n * m_numRows, m_numRows }; }
        VerticalValueIterator<T> operator-(const difference_type n) const noexcept { return VerticalValueIterator<T>{ m_ptr - n * m_numRows, m_numRows }; }

        friend VerticalValueIterator<T> operator+(const difference_type n, const VerticalValueIterator<T>& it) noexcept { return it + n; }
        friend difference_type operator-(const VerticalValueIterator<T>& it1, const VerticalValueIterator<T>& it2) noexcept { return (it1.m_ptr - it2.m_ptr) / it1.m_numRows; }

        // Subscripting -> required by the std::random_access_iterator concept
        reference operator[](const difference_type n) const { return *(m_ptr + n*m_numRows); }

        // Less than, less than or equal to, greather than, greater than or equal to -> required by the std::random_access_iterator concept
        bool operator<(const VerticalValueIterator<T>& other) const noexcept { return m_ptr < other.m_ptr; }
        bool operator<=(const VerticalValueIterator<T>& other) const noexcept { return m_ptr <= other.m_ptr; }
        bool operator>(const VerticalValueIterator<T>& other) const noexcept { return m_ptr > other.m_ptr; }
        bool operator>=(const VerticalValueIterator<T>& other) const noexcept { return m_ptr >= other.m_ptr; }

    private:
        pointer m_ptr{ nullptr };
        size_t m_numRows{ 0 };
    };

    template<typename T>
    class Channel
    {
    private:
        size_t m_numCols{ 0 };
        size_t m_numRows{ 0 };
        std::unique_ptr<T[]> m_data{ nullptr };

    public:
        using iterator = typename HorizontalValueIterator<T>;
        using const_iterator = typename HorizontalValueIterator<const T>;
        using reverse_iterator = typename std::reverse_iterator<iterator>;
        using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;
        using column_iterator = typename VerticalValueIterator<T>;
        using const_column_iterator = typename VerticalValueIterator<const T>;
        using reverse_column_iterator = typename std::reverse_iterator<column_iterator>;
        using const_reverse_column_iterator = typename std::reverse_iterator<const_column_iterator>;

        Channel() { }

        Channel(size_t numRows, size_t numCols, T val = T()) : m_numRows{ numRows }, m_numCols{ numCols }, m_data{ std::make_unique<T[]>(numRows * numCols) }
        {
            if (m_numRows < 1 || m_numCols < 1)
                throw std::invalid_argument("Number of rows/columns should be greater than 0.");

            std::fill(m_data.get(), m_data.get() + numRows * numCols, val);
        }

        Channel(const Channel<T>& other) : m_numRows{ other.m_numRows }, m_numCols{ other.m_numCols }, m_data{ std::make_unique<T[]>(other.size()) }
        {
            memcpy(static_cast<void*>(m_data.get()), static_cast<void*>(other.m_data.get()), other.size() * sizeof(T));
        }

        Channel(Channel<T>&& other) noexcept
        {
            *this = std::move(other);
        }

        Channel<T>& operator=(const Channel<T>& other)
        {
            if (this == &other)
                return *this;

            Channel<T> temp = other;
            *this = std::move(temp);
            return *this;
        }

        Channel<T>& operator=(Channel<T>&& other) noexcept
        {
            m_numRows = other.m_numRows;
            m_numCols = other.m_numCols;
            m_data = std::move(other.m_data);
            return *this;
        }

        T& operator()(size_t row, size_t col) { return m_data[to_index(row, col)]; }

        T& operator()(size_t index) { return m_data[index]; }

        T const& operator()(size_t row, size_t col) const { return m_data[to_index(row, col)]; }

        T const& operator()(size_t index) const { return m_data[index]; }

        void operator=(T value) { std::fill(m_data.get(), m_data.get() + size(), value); }

        // Itearators
        iterator begin() noexcept { return iterator{ m_data.get() }; }
        const_iterator begin() const noexcept { return cbegin(); }
        const_iterator cbegin() const noexcept { return const_iterator{ m_data.get() }; }

        iterator end() noexcept { return iterator{ m_data.get() + size() }; }
        const_iterator end() const noexcept { return cend(); }
        const_iterator cend() const noexcept { return const_iterator{ m_data.get() + size() }; }

        // Reverse iterators
        reverse_iterator rbegin() noexcept { return reverse_iterator{ end() }; }
        const_reverse_iterator rbegin() const noexcept { return crbegin(); }
        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator{ cend() }; }

        reverse_iterator rend() noexcept { return reverse_iterator{ begin() }; }
        const_reverse_iterator rend() const noexcept { return crend(); }
        const_reverse_iterator crend() const noexcept { return const_reverse_iterator{ cbegin() }; }

        // Row iterators
        iterator row_begin(size_t row) noexcept { return iterator{ m_data.get() + row * m_numCols }; }
        const_iterator row_begin(size_t row) const noexcept { return const_row_begin(row); }
        const_iterator const_row_begin(size_t row) const noexcept { return const_iterator{ m_data.get() + row * m_numCols }; }

        iterator row_end(size_t row) noexcept { return row_begin(row + 1); }
        const_iterator row_end(size_t row) const noexcept { return const_row_begin(row + 1); }
        const_iterator const_row_end(size_t row) const noexcept { return const_row_begin(row + 1); }
        
        // Reverse row iterators
        reverse_iterator rrow_begin(size_t row) noexcept { return reverse_iterator{ row_end(row) }; }
        const_reverse_iterator rrow_begin(size_t row) const noexcept { return const_rrow_begin(row); }
        const_reverse_iterator const_rrow_begin(size_t row) const noexcept { return const_reverse_iterator{ const_row_end(row) }; }

        reverse_iterator rrow_end(size_t row) noexcept { return reverse_iterator{ row_begin(row) }; }
        const_reverse_iterator rrow_end(size_t row) const noexcept { return const_rrow_end(row); }
        const_reverse_iterator const_rrow_end(size_t row) const noexcept { return const_reverse_iterator{ const_row_begin(row) }; }

        // Column iterators
        column_iterator column_begin(size_t column) noexcept { return column_iterator{ m_data.get() + column, m_numRows }; }
        const_column_iterator column_begin(size_t column) const noexcept { return const_column_begin(column); }
        const_column_iterator const_column_begin(size_t column) const noexcept { return const_column_iterator{ m_data.get() + column, m_numRows }; }

        column_iterator column_end(size_t column) noexcept { return (column_begin(column) += m_numCols); }
        const_column_iterator column_end(size_t column) const noexcept { return const_column_end(column); }
        const_column_iterator const_column_end(size_t column) const noexcept { return (const_column_begin(column) += m_numCols); }

        // Reverse column iterators
        reverse_column_iterator rcolumn_begin(size_t column) noexcept { return reverse_column_iterator{ column_end(column) }; }
        const_reverse_column_iterator rcolumn_begin(size_t column) const noexcept { return const_rcolumn_begin(column); }
        const_reverse_column_iterator const_rcolumn_begin(size_t column) const noexcept { return const_reverse_column_iterator{ const_column_end(column) }; }

        reverse_column_iterator rcolumn_end(size_t column) noexcept { return reverse_column_iterator{ column_begin(column) }; }
        const_reverse_column_iterator rcolumn_end(size_t column) const noexcept { return const_rcolumn_end(column); }
        const_reverse_column_iterator const_rcolumn_end(size_t column) const noexcept { return const_reverse_column_iterator{ const_column_begin(column) }; }

        // Index iterator 
        iterator get_iterator(size_t row, size_t col) noexcept { return iterator{ m_data.get() + to_index(row, col) }; }
        const_iterator get_iterator(size_t row, size_t col) const noexcept { return get_const_iterator(row, col); }
        const_iterator get_const_iterator(size_t row, size_t col) const noexcept { return const_iterator{ m_data.get() + to_index(row, col) }; }

        void clear() 
        {
            m_data.reset();
            m_numRows = 0;
            m_numCols = 0;
        }

        T const* get_data() const noexcept { return m_data.get(); }
        
        bool empty() const noexcept { return size() == 0 && m_data == nullptr; }

        auto num_columns() const noexcept { return m_numCols; }
        
        auto num_rows() const noexcept { return m_numRows; }

        auto size() const noexcept { return m_numRows * m_numCols; }

        bool resize(size_t numRows, size_t numCols) noexcept 
        {
            if (numRows == m_numRows && numCols == m_numCols)
                return false;

            std::unique_ptr<T[]> buffer{ nullptr };

            try
            {
                buffer = std::make_unique<T[]>(numRows * numCols);
            }
            catch (...)
            {
                return false;
            }

            clear();
            m_numRows = numRows;
            m_numCols = numCols;

            m_data = std::move(buffer);

            return true;
        }

    private:

        auto to_index(size_t row, size_t col) const noexcept { return row * m_numCols + col; }
    };
}
