#ifndef IMAGECHANNEL_HPP
#define IMAGECHANNEL_HPP

#include <vector>
#include <iostream>
#include <iomanip>

template<typename T>
class ImageChannel {
public:
    // iterators and refrences
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;
    typedef typename std::vector<T>::reference reference;
    typedef typename std::vector<T>::const_reference const_reference;

    // default constructor
    ImageChannel();

    // constructor
    ImageChannel(int rows, int columns, T initial_value = T());

    // operator overloadings
    reference operator()(int row, int column);
    reference operator()(int index);
    const_reference operator()(int row, int column) const;
    const_reference operator()(int index) const;
    void operator=(T value);

    // iterator functions
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
    int columns;
    int rows;
    std::vector<T> pixels;
};

template <typename T>
inline ImageChannel<T>::ImageChannel() : columns(0), rows(0), pixels() { }

template <typename T>
inline ImageChannel<T>::ImageChannel(int r, int c, T initial_value)
    : rows(r), columns(c), pixels(rows * columns, initial_value) { }

template <typename T>
inline void ImageChannel<T>::operator=(T value) {
    pixels.assign(pixels.size(), value);
}

template <typename T>
inline typename ImageChannel<T>::reference ImageChannel<T>::operator()(int row, int column) {
    return pixels[convert(row, column)];
}

template <typename T>
inline typename ImageChannel<T>::const_reference ImageChannel<T>::operator()(int row, int column) const{
    return pixels[convert(row, column)];
}

template <typename T>
inline typename ImageChannel<T>::reference ImageChannel<T>::operator()(int index) {
    return pixels[index];
}

template <typename T>
inline typename ImageChannel<T>::const_reference ImageChannel<T>::operator()(int index) const{
    return pixels[index];
}

template <typename T>
inline typename ImageChannel<T>::iterator ImageChannel<T>::begin() {
    return pixels.begin();
}

template <typename T>
inline typename ImageChannel<T>::const_iterator ImageChannel<T>::begin() const {
    return pixels.begin();
}

template <typename T>
inline typename ImageChannel<T>::iterator ImageChannel<T>::end() {
    return pixels.end();
}

template <typename T>
inline typename ImageChannel<T>::const_iterator ImageChannel<T>::end() const {
    return pixels.end();
}

template <typename T>
inline typename ImageChannel<T>::iterator ImageChannel<T>::row_begin(int row) {
    return pixels.begin() + row * num_columns();
}

template <typename T>
inline typename ImageChannel<T>::const_iterator ImageChannel<T>::row_begin(int row) const{
    return pixels.begin() + row * num_columns();
}

template <typename T>
inline typename ImageChannel<T>::iterator ImageChannel<T>::row_end(int row) {
    return row_begin(row + 1);
}

template <typename T>
inline typename ImageChannel<T>::const_iterator ImageChannel<T>::row_end(int row) const{
    return row_begin(row + 1);
}

template <typename T>
inline typename ImageChannel<T>::iterator ImageChannel<T>::row_begin(int row, int column) {
    return row_begin(row) + column;
}

template <typename T>
inline typename ImageChannel<T>::const_iterator ImageChannel<T>::row_begin(int row, int column) const {
    return row_begin(row) + column;
}

template <typename T>
inline typename ImageChannel<T>::iterator ImageChannel<T>::row_end(int row, int column) {
    return row_begin(row, column) + 1;
}

template <typename T>
inline typename ImageChannel<T>::const_iterator ImageChannel<T>::row_end(int row, int column) const {
    return row_begin(row, column) + 1;
}

template <typename T>
inline void ImageChannel<T>::clear() {
    pixels.clear();
    rows = 0;
    columns = 0;
}

template <typename T>
inline int ImageChannel<T>::num_columns() const {
    return columns;
}

template <typename T>
inline int ImageChannel<T>::num_rows() const {
    return rows;
}

template <typename T>
inline typename std::vector<T>::size_type ImageChannel<T>::convert(int row, int column) const {
    return row * num_columns() + column;
}

template <typename T>
inline bool ImageChannel<T>::empty() const {
    return pixels.empty();
}

template <typename T>
void ImageChannel<T>::resize(int the_rows, int the_columns) {
    if(the_rows == rows && the_columns == columns) {
        return;
    }
    pixels.clear();
    pixels.resize(the_rows * the_columns);
    rows = the_rows;
    columns = the_columns;
}

template <typename T>
void ImageChannel<T>::print_channel() const {
    std::cout << "-------------------------------------" << std::endl;
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < columns; ++j) {
            std::cout << std::setw(3) << static_cast<int>(pixels[convert(i,j)]) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-------------------------------------" << std::endl;
}

#endif
