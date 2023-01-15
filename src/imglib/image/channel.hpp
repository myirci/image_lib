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
    class ChPxIt
    {
    public:

        using iterator_category = std::contiguous_iterator_tag;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;
        using difference_type   = std::ptrdiff_t;

        // Default constructible -> required by the std::forward_iterator concept
        ChPxIt() noexcept = default;

        // Constructor
        explicit ChPxIt(T* ptr) noexcept : m_ptr{ ptr } { }

        // Dereferencable -> required by the std::input_or_output_iterator concept 
        // Note that although the pointee can be modified using the returned reference, m_ptr is not modified, therefore marked with const specifier. 
        reference operator*() const { return *m_ptr; }

        // Pre-incrementable -> required by the std::input_or_output_iterator concept 
        ChPxIt<T>& operator++() noexcept
        {
            ++m_ptr;
            return *this;
        }

        // Post-incrementable -> required by the std::input_or_output_iterator concept 
        ChPxIt<T> operator++(int)
        {
            auto copy = *this;
            ++m_ptr;
            return copy;
        }

        // Pre-decrementable -> required by the std::bidirectional_iterator concept 
        ChPxIt<T>& operator--() noexcept
        {
            --m_ptr;
            return *this;
        }

        // Post-decrementable -> required by the std::bidirectional_iterator concept 
        ChPxIt<T> operator--(int)
        {
            auto copy = *this;
            --m_ptr;
            return copy;
        }

        // Equality -> required by the std::input_iterator concept 
        bool operator==(const ChPxIt<T>& other) const noexcept { return m_ptr == other.m_ptr; }

        // Inequality
        bool operator!=(const ChPxIt<T>& other) const noexcept { return m_ptr != other.m_ptr; }

        // Compound addition assignment -> required by the std::random_access_iterator concept
        ChPxIt<T>& operator+=(const difference_type n) noexcept
        {
            m_ptr += n;
            return *this;
        }

        // Compound substraction assignment -> required by the std::random_access_iterator concept
        ChPxIt<T>& operator-=(const difference_type n) noexcept
        {
            m_ptr -= n;
            return *this;
        }

        // Addition, substraction -> required by the std::random_access_iterator concept
        ChPxIt<T> operator+(const difference_type n) const noexcept { return ChPxIt<T>(m_ptr + n); }
        ChPxIt<T> operator-(const difference_type n) const noexcept { return ChPxIt<T>(m_ptr - n); }

        friend ChPxIt<T> operator+(const difference_type n, const ChPxIt<T>& it) noexcept { return it + n; }
        friend difference_type operator-(const ChPxIt<T>& it1, const ChPxIt<T>& it2) noexcept { return it1.m_ptr - it2.m_ptr; }

        // Subscripting -> required by the std::random_access_iterator concept
        reference operator[](const difference_type n) const { return *(m_ptr + n); }

        // Less than, less than or equal to, greather than, greater than or equal to -> required by the std::random_access_iterator concept
        bool operator<(const ChPxIt<T>& other) const noexcept { return m_ptr < other.m_ptr; }
        bool operator<=(const ChPxIt<T>& other) const noexcept { return m_ptr <= other.m_ptr; }
        bool operator>(const ChPxIt<T>& other) const noexcept { return m_ptr > other.m_ptr; }
        bool operator>=(const ChPxIt<T>& other) const noexcept { return m_ptr >= other.m_ptr; }

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
        using iterator       = typename ChPxIt<T>;
        using const_iterator = typename ChPxIt<const T>;

        Channel() { }
        
        Channel(size_t numRows, size_t numCols, T val = T()) : m_numRows{ numRows }, m_numCols{ numCols }, m_data{ std::make_unique<T[]>(numRows * numCols) }
        {
            std::fill(m_data.get(), m_data.get() + numRows * numCols, val);
        }

        Channel(const Channel<T>& other) : m_numRows{ other.m_numRows }, m_numCols{ other.m_numCols }, m_data{ std::make_unique<T[]>(other.size()) }
        {
            // std::copy(other.begin(), other.end(), this->begin());
            memcpy(static_cast<void*>(m_data.get()), static_cast<void*>(other.m_data.get()), other.size() * sizeof(T));
        }

        Channel(Channel<T>&& other) noexcept
        {
            *this = std::move(other);
        }

        Channel<T>& operator=(const Channel<T>& other) 
        {
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

        iterator begin() noexcept { return iterator{ m_data.get() }; }

        const_iterator begin() const noexcept { return cbegin(); }

        const_iterator cbegin() const noexcept { return const_iterator{ m_data.get() }; }

        iterator end() noexcept { return iterator{ m_data.get() + size() }; }

        const_iterator end() const noexcept { return cend(); }

        const_iterator cend() const noexcept { return const_iterator{ m_data.get() + size() }; }

        iterator row_begin(size_t row) { return iterator{ m_data.get() + row * m_numCols }; }

        const_iterator row_begin(size_t row) const { return const_iterator{ m_data.get() + row * m_numCols }; }

        iterator row_end(size_t row) { return row_begin(row + 1); }

        const_iterator row_end(size_t row) const { return row_begin(row + 1); }

        iterator row_iterator(size_t row, size_t col) { return iterator{ m_data.get() + to_index(row, col) }; }

        const_iterator row_iterator(size_t row, size_t col) const { return const_iterator{ m_data.get() + to_index(row, col) }; }

        void clear() 
        {
            m_data.reset();
            m_numRows = 0;
            m_numCols = 0;
        }
        
        bool empty() const noexcept { return size() == 0; }

        auto num_columns() const noexcept { return m_numCols; }
        
        auto num_rows() const noexcept { return m_numRows; }

        auto size() const noexcept { return m_numRows * m_numCols; }

        bool resize(size_t numRows, size_t numCols) noexcept 
        {
            if (numRows == m_numRows && numCols == m_numCols)
                return;

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

            m_data.clear();
            m_data = std::move(buffer);

            return true;
        }

    private:

        auto to_index(size_t row, size_t col) const noexcept { return row * m_numCols + col; }
    };
}
