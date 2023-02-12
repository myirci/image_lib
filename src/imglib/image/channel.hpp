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
    class ValueIterator
    {
    public:

        using iterator_category = std::contiguous_iterator_tag;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;
        using difference_type   = std::ptrdiff_t;
        using self              = ValueIterator<T>;

        // Default constructible -> required by the std::forward_iterator concept
        ValueIterator() noexcept = default;

        // Constructor
        explicit ValueIterator(pointer ptr) noexcept : m_ptr{ ptr } { }

        // Dereferencable -> required by the std::input_or_output_iterator concept 
        // Note that although the pointee can be modified using the returned reference, m_ptr is not modified, therefore marked with const specifier. 
        reference operator*() const { return *m_ptr; }

        // Pre-incrementable -> required by the std::input_or_output_iterator concept 
        self& operator++() noexcept
        {
            ++m_ptr;
            return *this;
        }

        // Post-incrementable -> required by the std::input_or_output_iterator concept 
        self operator++(int)
        {
            auto copy = *this;
            ++m_ptr;
            return copy;
        }

        // Pre-decrementable -> required by the std::bidirectional_iterator concept 
        self& operator--() noexcept
        {
            --m_ptr;
            return *this;
        }

        // Post-decrementable -> required by the std::bidirectional_iterator concept 
        self operator--(int)
        {
            auto copy = *this;
            --m_ptr;
            return copy;
        }

        // Equality -> required by the std::input_iterator concept 
        bool operator==(const self& other) const noexcept { return m_ptr == other.m_ptr; }

        // Inequality
        bool operator!=(const self& other) const noexcept { return m_ptr != other.m_ptr; }

        // Compound addition assignment -> required by the std::random_access_iterator concept
        self& operator+=(const difference_type n) noexcept
        {
            m_ptr += n;
            return *this;
        }

        // Compound substraction assignment -> required by the std::random_access_iterator concept
        self& operator-=(const difference_type n) noexcept
        {
            m_ptr -= n;
            return *this;
        }

        // Addition, substraction -> required by the std::random_access_iterator concept
        self operator+(const difference_type n) const noexcept { return self{ m_ptr + n }; }
        self operator-(const difference_type n) const noexcept { return self{ m_ptr - n }; }

        friend self operator+(const difference_type n, const self& it) noexcept { return it + n; }
        friend difference_type operator-(const self& it1, const self& it2) noexcept { return it1.m_ptr - it2.m_ptr; }

        // Subscripting -> required by the std::random_access_iterator concept
        reference operator[](const difference_type n) const { return *(m_ptr + n); }

        // Less than, less than or equal to, greather than, greater than or equal to -> required by the std::random_access_iterator concept
        bool operator<(const self& other) const noexcept { return m_ptr < other.m_ptr; }
        bool operator<=(const self& other) const noexcept { return m_ptr <= other.m_ptr; }
        bool operator>(const self& other) const noexcept { return m_ptr > other.m_ptr; }
        bool operator>=(const self& other) const noexcept { return m_ptr >= other.m_ptr; }

        // required by the std::contiguous_iterator concept
        const pointer operator->() const noexcept { return m_ptr; }

    private:
        pointer m_ptr{ nullptr };
    };
    
    template<typename T>
    class Channel
    {
    private:
        size_t m_numCols{ 0 };
        size_t m_numRows{ 0 };
        std::unique_ptr<T[]> m_data{ nullptr };

    public:
        using iterator               = typename ValueIterator<T>;
        using const_iterator         = typename ValueIterator<const T>;
        using reverse_iterator       = typename std::reverse_iterator<iterator>;
        using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

        Channel() noexcept = default;

        Channel(size_t numRows, size_t numCols, T val = T()) : m_numRows{ numRows }, m_numCols{ numCols }, m_data{ std::make_unique<T[]>(numRows * numCols) }
        {
            if (m_numRows < 1 || m_numCols < 1)
                throw std::invalid_argument("Number of rows/columns should be greater than 0.");

            std::fill(m_data.get(), m_data.get() + numRows * numCols, val);
        }

        Channel(const Channel<T>& other) : m_numRows{ other.m_numRows }, m_numCols{ other.m_numCols }, m_data{ std::make_unique<T[]>(other.size()) }
        {
            memcpy(static_cast<void*>(m_data.get()), static_cast<void const*>(other.m_data.get()), other.size() * sizeof(T));
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
            if (this == &other)
                return *this;

            m_numRows = other.m_numRows;
            m_numCols = other.m_numCols;
            m_data = std::move(other.m_data);
            other.clear();
            return *this;
        }

        T& operator()(size_t row, size_t col) { return m_data[to_index(row, col)]; }

        T& operator()(size_t index) { return m_data[index]; }

        T const& operator()(size_t row, size_t col) const { return m_data[to_index(row, col)]; }

        T const& operator()(size_t index) const { return m_data[index]; }

        void operator=(T value) { std::fill(m_data.get(), m_data.get() + size(), value); }

        // Iterators
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

        // Generic iterator 
        iterator git(size_t row, size_t col) noexcept { return iterator{ m_data.get() + to_index(row, col) }; }
        const_iterator git(size_t row, size_t col) const noexcept { return cgit(row, col); }
        const_iterator cgit(size_t row, size_t col) const noexcept { return const_iterator{ m_data.get() + to_index(row, col) }; }

        // Row iterators
        iterator row_begin(size_t row) noexcept { return iterator{ m_data.get() + row * m_numCols }; }
        const_iterator row_begin(size_t row) const noexcept { return crow_begin(row); }
        const_iterator crow_begin(size_t row) const noexcept { return const_iterator{ m_data.get() + row * m_numCols }; }

        iterator row_end(size_t row) noexcept { return row_begin(row + 1); }
        const_iterator row_end(size_t row) const noexcept { return crow_begin(row + 1); }
        const_iterator crow_end(size_t row) const noexcept { return crow_begin(row + 1); }

        // Reverse row iterators
        reverse_iterator rrow_begin(size_t row) noexcept { return reverse_iterator{ row_end(row) }; }
        const_reverse_iterator rrow_begin(size_t row) const noexcept { return crrow_begin(row); }
        const_reverse_iterator crrow_begin(size_t row) const noexcept { return const_reverse_iterator{ crow_end(row) }; }

        reverse_iterator rrow_end(size_t row) noexcept { return reverse_iterator{ row_begin(row) }; }
        const_reverse_iterator rrow_end(size_t row) const noexcept { return crrow_end(row); }
        const_reverse_iterator crrow_end(size_t row) const noexcept { return const_reverse_iterator{ crow_begin(row) }; }

        void clear() noexcept
        {
            m_data.reset();
            m_numRows = 0;
            m_numCols = 0;
        }

        T const* data() const noexcept { return m_data.get(); }
        
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

            m_numRows = numRows;
            m_numCols = numCols;
            m_data = std::move(buffer);

            return true;
        }

        void copy(const Channel<T>& other)
        {
            if (m_numRows != other.m_numRows || m_numCols != other.m_numCols)
                throw std::invalid_argument("Size mismatch.");

            memcpy(static_cast<void*>(m_data.get()), static_cast<void*>(other.m_data.get()), other.size() * sizeof(T));
        }

    private:

        auto to_index(size_t row, size_t col) const noexcept { return row * m_numCols + col; }
    };
}
