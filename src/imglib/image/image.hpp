#pragma once

#include <vector>
#include <string>

#include <imglib/image/channel.hpp>
#include <imglib/color/color.hpp>

namespace imglib
{
    template <typename T>
    class Image
    {
    public:
        using iterator       = typename std::vector<Channel<T> >::iterator;
        using const_iterator = typename std::vector<Channel<T> >::const_iterator;

        // Default constructor
        Image() { }

        // Constructor
        Image(int height, int width, ColorSpace colorSpace, int numChannels, T val = 0) : 
            m_height{ height }, 
            m_width{ width }, 
            m_colorSpace{ colorSpace }, 
            m_numChannels{ numChannels }, 
            m_image(numChannels, Channel<T>(height, width, val)) { }
        
        // Copy constructor
        Image(const Image<T>& other);

        // Move constructor
        Image(Image<T>&& other);
        
        // Copy assignment operator
        Image<T>& operator= (const Image<T>& rhs);
        
        // Move assignment operator
        Image<T>& operator= (Image<T>&& rhs);
        
        Channel<T>& operator()(int index) { return m_image[index]; }
        
        Channel<T> const& operator()(int index) const { return m_image[index]; }
        
        iterator begin() { return m_image.begin(); }
        
        const_iterator begin() const { return m_image.begin(); }
        
        iterator end() { return m_image.end(); }
        
        const_iterator end() const { return m_image.end(); }

        int get_width() const noexcept { return m_width; }

        int get_height() const noexcept { return m_height; }

        int get_num_channels() const noexcept { return m_numChannels; }

        ColorSpace get_color_space() const noexcept { return m_colorSpace; }

        int get_buffer_size() const noexcept { return m_height * m_width * m_numChannels; }

        std::unique_ptr<T[]> get_buffer() const;
        
        void update(int height, int width, int numChannels, ColorSpace colorSpace, const std::unique_ptr<T[]>& buffer);
        
        bool insert_channel(const Channel<T>& ch);
        
        void clear();

    private:
        int m_height{ 0 };
        int m_width{ 0 };
        int m_numChannels{ 0 };
        ColorSpace m_colorSpace{ ColorSpace::Unspecified };
        std::vector<Channel<T>> m_image;
    };

    template <typename T>
    inline Image<T>::Image(const Image<T>& other) 
    {
        m_height = other.m_height;
        m_width = other.m_width;
        m_numChannels = other.m_numChannels;
        m_colorSpace = other.m_colorSpace;
        m_image = other.m_image;
    }

    template <typename T>
    inline Image<T>::Image(Image<T>&& other) 
    {
        m_height = other.m_height;
        m_width = other.m_width;
        m_numChannels = other.m_numChannels;
        m_colorSpace = other.m_colorSpace;
        m_image = std::move(other.m_image);
    }

    template <typename T>
    inline Image<T>& Image<T>::operator=(const Image<T>& rhs) 
    {
        if (this != &rhs) 
        {
            m_height = rhs.m_height;
            m_width = rhs.m_width;
            m_numChannels = rhs.m_numChannels;
            m_colorSpace = rhs.m_colorSpace;
            m_image = rhs.m_image;
        }
        return *this;
    }

    template <typename T>
    inline Image<T>& Image<T>::operator=(Image<T>&& rhs) 
    {
        m_height = rhs.m_height;
        m_width = rhs.m_width;
        m_numChannels = rhs.m_numChannels;
        m_colorSpace = rhs.m_colorSpace;
        m_image = std::move(rhs.m_image);
    }

    template <typename T>
    inline std::unique_ptr<T[]> Image<T>::get_buffer() const
    {
        int bufferSize { get_buffer_size() };
        auto buffer = std::make_unique<T[]>(bufferSize);

        int k{ 0 }, t{ 0 };
        for (int i{ 0 }; i < bufferSize; i += m_numChannels, k++)
            for (int j{ 0 }; j < m_numChannels; ++j)
                buffer[t++] = m_image[j](k); 

        return buffer;
    }

    template <typename T>
    inline void Image<T>::update(int height, int width, int numChannels, ColorSpace colorSpace, const std::unique_ptr<T[]>& buffer)
    {
        clear();
        m_height = height;
        m_width = width;
        m_numChannels = numChannels;
        m_colorSpace = colorSpace;

        for (int i{ 0 }; i < m_numChannels; ++i)
            m_image.push_back(Channel<T>(m_height, m_width));
        
        int k{ 0 }, bufferSize{ m_height * m_width * m_numChannels };
        for (int i{ 0 }; i < bufferSize; ++i)
        {
            for (int j{ 0 }; j < m_numChannels; ++j)
            {
                if (i % m_numChannels == j)
                {
                    m_image[j](k) = buffer[i];
                    break;
                }
            }
            if ((i % m_numChannels) == (m_numChannels - 1))
                k++;
        }
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
        m_image.clear();
        m_height = 0;
        m_width = 0;
        m_numChannels = 0;
        m_colorSpace = ColorSpace::Unspecified;
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
