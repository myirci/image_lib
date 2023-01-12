#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <memory>
#include <algorithm>

namespace imglib
{
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
