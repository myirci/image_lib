#pragma once

#include <vector>
#include <iostream>
#include <iomanip>

namespace imglib
{
    template<typename T>
    class Channel
    {
    private:
        int m_numCols;
        int m_numRows;
        std::vector<T> m_data;

    public:
        using iterator = typename std::vector<T>::iterator;
        using const_iterator = typename std::vector<T>::const_iterator;
        using reference = typename std::vector<T>::reference;
        using const_reference = typename std::vector<T>::const_reference;

        Channel();
        Channel(int numRows, int numCols, T val = T());

        reference operator()(int row, int col);
        reference operator()(int index);
        const_reference operator()(int row, int col) const;
        const_reference operator()(int index) const;
        void operator=(T value);

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;

        iterator row_begin(int row);
        const_iterator row_begin(int row) const;
        iterator row_end(int row);
        const_iterator row_end(int row) const;

        iterator row_iterator(int row, int col);
        const_iterator row_iterator(int row, int col) const;

        void clear();
        bool empty() const;

        int num_columns() const;
        int num_rows() const;

        void resize(int numRows, int numCols);
       
    private:
        typename std::vector<T>::size_type convert(int row, int col) const;
    };

    template <typename T>
    inline Channel<T>::Channel() : m_numCols{ 0 }, m_numRows{ 0 } { }

    template <typename T>
    inline Channel<T>::Channel(int numRows, int numCols, T val) : m_numRows{ numRows }, m_numCols{ numCols }, m_data(numRows * numCols, val) { }

    template <typename T>
    inline void Channel<T>::operator=(T value) { m_data.assign(m_data.size(), value); }

    template <typename T>
    inline typename Channel<T>::reference Channel<T>::operator()(int row, int col) { return m_data[convert(row, col)]; }

    template <typename T>
    inline typename Channel<T>::const_reference Channel<T>::operator()(int row, int col) const { return m_data[convert(row, col)]; }

    template <typename T>
    inline typename Channel<T>::reference Channel<T>::operator()(int index) { return m_data[index]; }

    template <typename T>
    inline typename Channel<T>::const_reference Channel<T>::operator()(int index) const { return m_data[index]; }

    template <typename T>
    inline typename Channel<T>::iterator Channel<T>::begin() { return m_data.begin(); }

    template <typename T>
    inline typename Channel<T>::const_iterator Channel<T>::begin() const { return m_data.begin(); }

    template <typename T>
    inline typename Channel<T>::iterator Channel<T>::end() { return m_data.end(); }

    template <typename T>
    inline typename Channel<T>::const_iterator Channel<T>::end() const { return m_data.end(); }

    template <typename T>
    inline typename Channel<T>::iterator Channel<T>::row_begin(int row) { return m_data.begin() + row * m_numCols; }

    template <typename T>
    inline typename Channel<T>::const_iterator Channel<T>::row_begin(int row) const { return m_data.begin() + row * m_numCols; }

    template <typename T>
    inline typename Channel<T>::iterator Channel<T>::row_end(int row) { return row_begin(row + 1); }

    template <typename T>
    inline typename Channel<T>::const_iterator Channel<T>::row_end(int row) const { return row_begin(row + 1); }

    template <typename T>
    inline typename Channel<T>::iterator Channel<T>::row_iterator(int row, int col) { return row_begin(row) + col; }

    template <typename T>
    inline typename Channel<T>::const_iterator Channel<T>::row_iterator(int row, int col) const { return row_begin(row) + col; }
   
    template <typename T>
    inline void Channel<T>::clear()
    {
        m_data.clear();
        m_numRows = 0;
        m_numCols = 0;
    }

    template <typename T>
    inline int Channel<T>::num_columns() const { return m_numCols; }

    template <typename T>
    inline int Channel<T>::num_rows() const { return m_numRows; }

    template <typename T>
    inline typename std::vector<T>::size_type Channel<T>::convert(int row, int col) const { return row * m_numCols + col; }

    template <typename T>
    inline bool Channel<T>::empty() const { return m_data.empty(); }

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
