#pragma once

#include <vector>
#include <iostream>
#include <iomanip>

namespace SmpImgLib
{
    template<typename T>
    class Channel
    {
    private:
        int m_cols;
        int m_rows;
        std::vector<T> m_data;

    public:
        using iterator = typename std::vector<T>::iterator;
        using const_iterator = typename std::vector<T>::const_iterator;
        using reference = typename std::vector<T>::reference;
        using const_reference = typename std::vector<T>::const_reference;

        Channel();
        Channel(int rows, int columns, T initial_value = T());

        reference operator()(int row, int column);
        reference operator()(int index);
        const_reference operator()(int row, int column) const;
        const_reference operator()(int index) const;
        void operator=(T value);

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;

        iterator row_begin(int row);
        const_iterator  row_begin(int row) const;
        iterator row_end(int row);
        const_iterator row_end(int row) const;

        iterator row_begin(int row, int column);
        const_iterator row_begin(int row, int column) const;
        iterator row_end(int row, int column);
        const_iterator row_end(int row, int column) const;

        void clear();
        bool empty() const;

        int num_columns() const;
        int num_rows() const;

        void resize(int the_rows, int the_columns);
        void print_channel() const;

    private:
        typename std::vector<T>::size_type convert(int row, int column) const;
    };

    template <typename T>
    inline Channel<T>::Channel() : m_cols{ 0 }, m_rows{ 0 } { }

    template <typename T>
    inline Channel<T>::Channel(int r, int c, T initial_value) : m_rows{ r }, m_cols{ c }, m_data(r * c, initial_value) { }

    template <typename T>
    inline void Channel<T>::operator=(T value)
    {
        m_data.assign(m_data.size(), value);
    }

    template <typename T>
    inline typename Channel<T>::reference Channel<T>::operator()(int row, int column)
    {
        return m_data[convert(row, column)];
    }

    template <typename T>
    inline typename Channel<T>::const_reference Channel<T>::operator()(int row, int column) const
    {
        return m_data[convert(row, column)];
    }

    template <typename T>
    inline typename Channel<T>::reference Channel<T>::operator()(int index)
    {
        return m_data[index];
    }

    template <typename T>
    inline typename Channel<T>::const_reference Channel<T>::operator()(int index) const
    {
        return m_data[index];
    }

    template <typename T>
    inline typename Channel<T>::iterator Channel<T>::begin()
    {
        return m_data.begin();
    }

    template <typename T>
    inline typename Channel<T>::const_iterator Channel<T>::begin() const
    {
        return m_data.begin();
    }

    template <typename T>
    inline typename Channel<T>::iterator Channel<T>::end()
    {
        return m_data.end();
    }

    template <typename T>
    inline typename Channel<T>::const_iterator Channel<T>::end() const
    {
        return m_data.end();
    }

    template <typename T>
    inline typename Channel<T>::iterator Channel<T>::row_begin(int row)
    {
        return m_data.begin() + row * num_columns();
    }

    template <typename T>
    inline typename Channel<T>::const_iterator Channel<T>::row_begin(int row) const
    {
        return m_data.begin() + row * num_columns();
    }

    template <typename T>
    inline typename Channel<T>::iterator Channel<T>::row_end(int row)
    {
        return row_begin(row + 1);
    }

    template <typename T>
    inline typename Channel<T>::const_iterator Channel<T>::row_end(int row) const
    {
        return row_begin(row + 1);
    }

    template <typename T>
    inline typename Channel<T>::iterator Channel<T>::row_begin(int row, int column)
    {
        return row_begin(row) + column;
    }

    template <typename T>
    inline typename Channel<T>::const_iterator Channel<T>::row_begin(int row, int column) const
    {
        return row_begin(row) + column;
    }

    template <typename T>
    inline typename Channel<T>::iterator Channel<T>::row_end(int row, int column)
    {
        return row_begin(row, column) + 1;
    }

    template <typename T>
    inline typename Channel<T>::const_iterator Channel<T>::row_end(int row, int column) const
    {
        return row_begin(row, column) + 1;
    }

    template <typename T>
    inline void Channel<T>::clear()
    {
        m_data.clear();
        m_rows = 0;
        m_cols = 0;
    }

    template <typename T>
    inline int Channel<T>::num_columns() const
    {
        return m_cols;
    }

    template <typename T>
    inline int Channel<T>::num_rows() const
    {
        return m_rows;
    }

    template <typename T>
    inline typename std::vector<T>::size_type Channel<T>::convert(int row, int column) const
    {
        return row * num_columns() + column;
    }

    template <typename T>
    inline bool Channel<T>::empty() const
    {
        return m_data.empty();
    }

    template <typename T>
    void Channel<T>::resize(int the_rows, int the_columns)
    {
        if (the_rows == m_rows && the_columns == m_cols)
            return;

        m_data.clear();
        m_data.resize(the_rows * the_columns);
        m_rows = the_rows;
        m_cols = the_columns;
    }

    template <typename T>
    void Channel<T>::print_channel() const
    {
        std::cout << "-------------------------------------" << std::endl;
        for (int i = 0; i < m_rows; ++i)
        {
            for (int j = 0; j < m_cols; ++j)
                std::cout << std::setw(3) << static_cast<int>(m_data[convert(i, j)]) << " ";
            std::cout << std::endl;
        }
        std::cout << "-------------------------------------" << std::endl;
    }
}


