#pragma once

#include "channel.hpp"
#include "jpeg_adapter.hpp"
#include "color.hpp"
#include <string>

namespace SmpImgLib
{
    template <typename T>
    class Image
    {
    private:
        int m_numChannels;
        int m_height;
        int m_width;
        ColorSpace m_colorSpace;
        std::vector<Channel<T>> m_image;

        void read_jpeg_file_and_update_data(std::string filename);

    public:
        using iterator = typename std::vector<Channel<T> >::iterator;
        using const_iterator = typename std::vector<Channel<T> >::const_iterator;
        using reference = typename std::vector<Channel<T> >::reference;
        using const_reference = typename std::vector<Channel<T> >::const_reference;

        Image();
        Image(int height, int widrg, ColorSpace colorSpace, int numChannels, T val = 0);
        Image(std::string filename);

        reference operator()(int index);
        const_reference operator()(int index) const;

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;

        int num_columns() const;
        int num_rows() const;
        int num_channels() const;
        ColorSpace get_color_space() const;

        void set_columns(int c);
        void set_rows(int r);
        void set_channels(int ch);
        void set_color_space(ColorSpace cs);

        int get_buffer_size() const;
        bool insert_channel(const Channel<T>& ch);
        void clear();

        void load_jpeg(std::string filename);
        void save_jpeg(std::string filename, int quality);
    };

    template <typename T>
    inline Image<T>::Image() : m_height{ 0 }, m_width{ 0 }, m_numChannels{ 0 }, m_colorSpace{ ColorSpace::Unspecified }, m_image(0, Channel<T>(0, 0)) { }

    template <typename T>
    inline Image<T>::Image(int height, int width, ColorSpace colorSpace, int numChannels, T val) : m_height{ height }, m_width{ width }, m_colorSpace{ colorSpace }, m_numChannels{ numChannels }, m_image(numChannels, Channel<T>(height, width, val)) { }

    template < >
    void Image<SimpleJpeg::data_type>::read_jpeg_file_and_update_data(std::string filename)
    {
        /*
        if (!image.empty())
            image.clear();                        
        
        SimpleJpeg::JpegAdapter in_img;
        SimpleJpeg::import_from_jpeg(filename, in_img);

        channels = in_img.color_components;
        rows = in_img.height;
        columns = in_img.width;
        // color_space = enum_convert<IMG_COLOR_SPACE>(in_img.color_space);
        buffer_size = rows * columns * channels;

        for (int i = 0; i < channels; ++i)
        {      // create each channel layer
            image.push_back(Channel<SimpleJpeg::data_type>(rows, columns));
        }

        int k = 0;                                  // transfer data from buffer to layers
        for (int i = 0; i < buffer_size; ++i)
        {
            for (int j = 0; j < channels; ++j)
            {
                if (i % channels == j)
                {
                    image[j](k) = in_img.buffer[i];
                    break;
                }
            }
            if ((i % channels) == (channels - 1))
                k++;
        }
        */
    }

    template < >
    Image<SimpleJpeg::data_type>::Image(std::string filename)
    {
        read_jpeg_file_and_update_data(filename);
    }

    template < >
    void Image<SimpleJpeg::data_type>::load_jpeg(std::string filename)
    {
        read_jpeg_file_and_update_data(filename);
    }

    template < >
    void Image<SimpleJpeg::data_type>::save_jpeg(std::string filename, int quality)
    {
        /*
        // initialize a buffer for stroing the image data
        SimpleJpeg::JpegAdapter out_img(columns, rows, channels, enum_convert<J_COLOR_SPACE>(color_space));
        int k = 0;
        int t = 0;
        // write data to the buffer
        for (int i = 0; i < buffer_size; i += channels, k++)
        {
            for (int j = 0; j < channels; ++j)
            {
                out_img.buffer[t++] = image[j](k);
            }
        }
        SimpleJpeg::export_to_jpeg(filename, quality, out_img);
        */
    }

    template <typename T>
    inline typename Image<T>::reference Image<T>::operator()(int index)
    {
        return m_image[index];
    }

    template <typename T>
    inline typename Image<T>::const_reference Image<T>::operator()(int index) const
    {
        return m_image[index];
    }

    template <typename T>
    inline typename Image<T>::iterator Image<T>::begin()
    {
        return m_image.begin();
    }

    template <typename T>
    inline typename Image<T>::const_iterator Image<T>::begin() const
    {
        return m_image.begin();
    }

    template <typename T>
    inline typename Image<T>::iterator Image<T>::end()
    {
        return m_image.end();
    }

    template <typename T>
    inline typename Image<T>::const_iterator Image<T>::end() const
    {
        return m_image.end();
    }

    template <typename T>
    inline int Image<T>::num_columns() const
    {
        return m_width;
    }

    template <typename T>
    inline int Image<T>::num_rows() const
    {
        return m_height;
    }

    template <typename T>
    inline int Image<T>::num_channels() const
    {
        return m_numChannels;
    }

    template <typename T>
    inline ColorSpace Image<T>::get_color_space() const
    {
        return m_colorSpace;
    }

    template <typename T>
    inline void Image<T>::set_rows(int r)
    {
        m_height = r;
    }

    template <typename T>
    inline void Image<T>::set_columns(int c)
    {
        m_width = c;
    }

    template <typename T>
    inline void Image<T>::set_channels(int ch)
    {
        m_numChannels = ch;
    }

    template <typename T>
    inline void Image<T>::set_color_space(ColorSpace cs)
    {
        m_colorSpace = cs;
    }

    template <typename T>
    inline int Image<T>::get_buffer_size() const
    {
        return m_height * m_width * m_numChannels;
    }

    template <typename T>
    inline bool Image<T>::insert_channel(const Channel<T>& ch)
    {
        if (ch.num_rows() == m_height && ch.num_columns() == m_width)
        {
            m_image.push_back(ch);
            return true;
        }
        return false;
    }

    template <typename T>
    inline void Image<T>::clear()
    {
        for (int i = 0; i < m_numChannels; ++i)
        {
            m_image[i].clear();
        }
        m_image.clear();
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
}


