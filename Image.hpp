#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "ImageChannel.hpp"
#include "BasicJpegAdapter.hpp"
#include "Color.hpp"
#include <string>

template <typename T>
class Image 
{
public:

    // iterators and refrences
    using iterator = typename std::vector<ImageChannel<T> >::iterator ;
    using const_iterator = typename std::vector<ImageChannel<T> >::const_iterator;
    using reference = typename std::vector<ImageChannel<T> >::reference;
    using const_reference = typename std::vector<ImageChannel<T> >::const_reference;

    // constructors
    Image();
    Image(int r, int c, IMG_COLOR_SPACE cs, int ch, T val = 0);
    Image(std::string filename);

    // operator overloadings
    reference operator()(int index);
    const_reference operator()(int index) const;

    // iterator functions
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    int num_columns() const;
    int num_rows() const;
    int num_channels() const;
    int get_buffer_size() const;
    IMG_COLOR_SPACE get_color_space() const;

    void set_columns(int c);
    void set_rows(int r);
    void set_channels(int ch);
    void calculate_buffer_size();
    void set_color_space(IMG_COLOR_SPACE cs);

    bool insert_channel(const ImageChannel<T>& ch);
    void clear();

    // loading and saving jpeg files
    void load_jpeg(std::string filename);               
    void save_jpeg(std::string filename, int quality);

private:
    void read_jpeg_file_and_update_data(std::string filename);
    int channels;                       
    int rows;                           
    int columns;                        
    int buffer_size; // width * height * channels
    IMG_COLOR_SPACE color_space;  // for now same as the jpeg color space
    std::vector<ImageChannel<T>> image;
};

template <typename T>
inline Image<T>::Image() : rows(0), columns(0), buffer_size(0), channels(0),
    color_space(UNSPECIFIED), image(0,ImageChannel<T>(0, 0)) { }

template <typename T>
inline Image<T>::Image(int r, int c, IMG_COLOR_SPACE cs, int ch, T val) : rows(r), columns(c),
    color_space(cs), channels(ch), buffer_size(r * c * ch),
    image(ch, ImageChannel<T>(r, c, val)) { }

template < >
void Image<JSAMPLE>::read_jpeg_file_and_update_data(std::string filename) 
{
    if(!image.empty())
    {
        image.clear();                          // clear the image if not empty
    }
    image_data in_img;
    read_JPEG_file(filename, in_img);           // read the file in to the in_img structure

    channels = in_img.color_components;      // update the parameters
    rows        = in_img.height;
    columns     = in_img.width;
    color_space = enum_convert<IMG_COLOR_SPACE>(in_img.color_space);
    buffer_size = rows*columns*channels;

    for(int i = 0; i < channels; ++i) 
    {      // create each channel layer
        image.push_back(ImageChannel<JSAMPLE>(rows, columns));
    }

    int k = 0;                                  // transfer data from buffer to layers
    for(int i = 0; i < buffer_size; ++i)
    {
        for(int j = 0; j < channels; ++j)
        {
            if(i % channels == j) 
            {
                image[j](k) = in_img.buff[i];
                break;
            }
        }
        if((i % channels) == (channels - 1) )
            k++;
    }
}

template < >
Image<JSAMPLE>::Image(std::string filename) 
{
    read_jpeg_file_and_update_data(filename);
}

template < >
void Image<JSAMPLE>::load_jpeg(std::string filename)
{
    read_jpeg_file_and_update_data(filename);
}

template < >
void Image<JSAMPLE>::save_jpeg(std::string filename, int quality)
{
    // initialize a buffer for stroing the image data
    image_data out_img(columns, rows, channels, enum_convert<J_COLOR_SPACE>(color_space));
    int k = 0;
    int t = 0;
    // write data to the buffer
    for(int i = 0; i < buffer_size; i+=channels, k++) 
    {
        for(int j = 0; j < channels; ++j)
        {
            out_img.buff[t++] = image[j](k);
        }
    }
    write_jpeg_file(filename, quality, out_img);
}

template <typename T>
inline typename Image<T>::reference Image<T>::operator()(int index)
{
    return image[index];
}

template <typename T>
inline typename Image<T>::const_reference Image<T>::operator()(int index) const
{
    return image[index];
}

template <typename T>
inline typename Image<T>::iterator Image<T>::begin() 
{
    return image.begin();
}

template <typename T>
inline typename Image<T>::const_iterator Image<T>::begin() const 
{
    return image.begin();
}

template <typename T>
inline typename Image<T>::iterator Image<T>::end()
{
    return image.end();
}

template <typename T>
inline typename Image<T>::const_iterator Image<T>::end() const
{
    return image.end();
}

template <typename T>
inline int Image<T>::num_columns() const 
{
    return columns;
}

template <typename T>
inline int Image<T>::num_rows() const 
{
    return rows;
}

template <typename T>
inline int Image<T>::num_channels() const 
{
    return channels;
}

template <typename T>
inline int Image<T>::get_buffer_size() const
{
    return buffer_size;
}

template <typename T>
inline IMG_COLOR_SPACE Image<T>::get_color_space() const
{
    return color_space;
}

template <typename T>
inline void Image<T>::set_rows(int r)
{
    rows = r;
}

template <typename T>
inline void Image<T>::set_columns(int c)
{
    columns = c;
}

template <typename T>
inline void Image<T>::set_channels(int ch)
{
    channels = ch;
}

template <typename T>
inline void Image<T>::calculate_buffer_size() 
{
    buffer_size = rows * columns * channels;
}

template <typename T>
inline void Image<T>::set_color_space(IMG_COLOR_SPACE cs)
{
    color_space = cs;
}

template <typename T>
inline bool Image<T>::insert_channel(const ImageChannel<T>& ch)
{
    if(ch.num_rows() == rows && ch.num_columns() == columns)
    {
        image.push_back(ch);
        return true;
    }
    return false;
}

template <typename T>
inline void Image<T>::clear() 
{
    for(int i = 0; i < channels; ++i) 
    {
        image[i].clear();
    }
    image.clear();
}

// global functions:

template<typename T, template<typename> class colorT>
inline void set_color(Image<T>& im, const colorT<T>& color) 
{
    do_set_color(im, color, typename color_traits<T, colorT>::color_space_category());
}

template<typename T, template<typename> class colorT>
inline void do_set_color(Image<T>& im, const colorT<T>& color, color_rgb_tag) 
{
    im(0) = color.r;
    im(1) = color.g;
    im(2) = color.b;
}

template<typename T, template<typename> class colorT>
inline void do_set_color(Image<T>& im, const colorT<T>& color, color_mono_tag)
{
    im(0) = color.v;
}

#endif // IMAGE_HPP
