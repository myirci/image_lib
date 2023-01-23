#pragma once

#include <memory>
#include <vector>

#include <imglib/image/channel.hpp>
#include <imglib/color/color.hpp>

namespace imglib
{
    template <typename T>
    class Image
    {
    public:
       
        // Default constructor
        Image() noexcept = default;

        // Constructor
        Image(size_t height, size_t width, ColorSpace colorSpace, size_t numChannels, T val = T()) :
            m_height{ height },
            m_width{ width },
            m_colorSpace{ colorSpace }, 
            m_numChannels{ numChannels } 
        {
            if (m_numChannels == 0)
                throw std::logic_error("Number of channels must be greater than zero.");

            if (m_colorSpace == ColorSpace::GrayScale) 
            {
                if(m_numChannels != 1)
                    throw std::logic_error("Number of channels for a grayscale image must be one.");
            }
            else if (m_colorSpace != ColorSpace::Unspecified)
            {
                if (m_numChannels != 3)
                    throw std::logic_error("Number of channels for must be three for this color space.");
            }

            for (auto i = 0; i < m_numChannels; i++)
                m_channels.emplace_back(std::make_unique<Channel<T>>(height, width, val));
        }

        // Copy constructor
        Image(const Image<T>& other) :
            m_height{ other.m_height },
            m_width{ other.m_width },
            m_colorSpace{ other.m_colorSpace },
            m_numChannels{ other.m_numChannels }
        {
            for (auto i = 0; i < m_numChannels; i++)
                m_channels.emplace_back(std::make_unique<Channel<T>>(*other.m_channels[i]));
        }

        // Move constructor
        Image(Image<T>&& other) noexcept
        {
            *this = std::move(other);
        }
        
        // Copy assignment operator
        Image<T>& operator= (const Image<T>& other) 
        {
            if (this == &other)
                return *this;

            Image<T> temp = other;
            *this = std::move(temp);
            return *this;
        }

        // Move assignment operator
        Image<T>& operator= (Image<T>&& other) noexcept
        {
            if (this == &other)
                return *this;

            m_height = other.m_height;
            m_width = other.m_width;
            m_numChannels = other.m_numChannels;
            m_colorSpace = other.m_colorSpace;
            m_channels = std::move(other.m_channels);
            other.clear();

            return *this;
        }
        
        size_t width() const noexcept { return m_width; }

        size_t height() const noexcept { return m_height; }

        size_t num_channels() const noexcept { return m_numChannels; }

        ColorSpace color_space() const noexcept { return m_colorSpace; }

        size_t size() const noexcept { return m_width * m_height; }

        size_t data_size() const noexcept { return size() * m_numChannels; }

        std::unique_ptr<T[]> data() const
        {
            auto bufferSize{ data_size() };
            if (bufferSize == 0)
                return nullptr;

            auto buffer = std::make_unique<T[]>(bufferSize);

            size_t k{ 0 }, t{ 0 };
            for (size_t i = 0; i < bufferSize; i += m_numChannels, k++)
                for (size_t j = 0; j < m_numChannels; ++j)
                    buffer[t++] = (*m_channels[j])(k);

            return buffer;
        }

        Channel<T>& operator()(size_t index) { return *m_channels[index]; }

        Channel<T> const& operator()(size_t index) const { return *m_channels[index]; }

        void append_channel(const Channel<T>& ch) 
        {
            if (ch.num_rows() != m_height || ch.num_columns() != m_width || m_colorSpace != ColorSpace::Unspecified)
                throw std::logic_error("Image - channel property mismatch");

            m_channels.emplace_back(std::make_unique<Channel<T>>(ch));
            m_numChannels++;
        }

        void delete_channel(size_t position) { }

        void replace_channel(size_t position, const Channel<T>& ch) { }

        void reorder_channels(std::vector<size_t>& new_positions) { }

        void clear() noexcept
        {
            m_channels.clear();
            m_height = 0;
            m_width = 0;
            m_numChannels = 0;
            m_colorSpace = ColorSpace::Unspecified;
        }

    private:

        using ChannelPtr = typename std::unique_ptr<Channel<T>>;

        size_t m_height{ 0 };
        size_t m_width{ 0 };
        size_t m_numChannels{ 0 };
        ColorSpace m_colorSpace{ ColorSpace::Unspecified };
        std::vector<ChannelPtr> m_channels;
    };

    // global functions:

    template<typename T, template<typename> typename colorT>
    inline void set_color(Image<T>& im, const colorT<T>& color)
    {
        do_set_color(im, color, typename color_traits<T, colorT>::color_space_category());
    }

    template<typename T, template<typename> typename colorT>
    inline void do_set_color(Image<T>& im, const colorT<T>& color, color_rgb_tag)
    {
        im(0) = color.r;
        im(1) = color.g;
        im(2) = color.b;
    }

    template<typename T, template<typename> typename colorT>
    inline void do_set_color(Image<T>& im, const colorT<T>& color, color_mono_tag)
    {
        im(0) = color.v;
    }
}
