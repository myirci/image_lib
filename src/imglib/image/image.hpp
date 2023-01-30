#pragma once

#include <memory>
#include <vector>

#include <imglib/image/channel.hpp>
#include <imglib/color/color.hpp>
#include <imglib/utility/utility.hpp>

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
            if (m_height == 0 || m_width == 0 || m_numChannels == 0 || !IsValid(m_colorSpace, m_numChannels))
                throw std::invalid_argument("At least one of the arguments is invalid.");

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
        
        void operator=(T value) 
        { 
            for (auto& ch : m_channels)
                *ch = value;
        }

        template <std::same_as<T> ...U>
            requires (sizeof...(U) >= 1)
        void set_channels(U... args) 
        {
            if (sizeof...(args) != m_numChannels)
                throw std::invalid_argument("Number of parameters error.");

            size_t i{ 0 };
            for (T arg : { args... })
                *m_channels[i++] = arg;
        }

        template <std::same_as<T> ...U>
            requires (sizeof...(U) >= 1)
        void set_pixel(size_t row, size_t col, U... args) 
        {
            set_pixel(row * m_width + col, args...);
        }

        template <std::same_as<T> ...U>
            requires (sizeof...(U) >= 1)
        void set_pixel(size_t index, U... args)
        {
            if (sizeof...(args) != m_numChannels)
                throw std::invalid_argument("Number of parameters error.");

            size_t i{ 0 };
            for (T arg : { args... })
                (*m_channels[i++])(index) = arg;
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

        template <typename ChannelType>
            requires std::same_as<typename std::remove_cvref_t<ChannelType>, Channel<T>>
        void append_channel(ChannelType&& ch)
        {
            if (ch.num_rows() != m_height || ch.num_columns() != m_width || m_colorSpace != ColorSpace::Unspecified)
                throw std::invalid_argument("Image - channel property mismatch.");

            m_channels.emplace_back(std::make_unique<Channel<T>>(std::forward<ChannelType>(ch)));
            m_numChannels++;
        }

        void delete_channel(size_t pos) 
        {
            if (m_numChannels == 0 || pos > m_numChannels - 1 || m_colorSpace != ColorSpace::Unspecified)
                throw std::invalid_argument("Position error.");

            auto it = m_channels.begin();
            it += pos;
            m_channels.erase(it);
            m_numChannels--;
        }
        
        template <typename ChannelType>
            requires std::same_as<typename std::remove_cvref_t<ChannelType>, Channel<T>>
        void replace_channel(size_t pos, ChannelType&& ch) 
        {
            if (m_numChannels == 0 || pos > m_numChannels - 1)
                throw std::invalid_argument("Position error.");

            if (ch.num_rows() != m_height || ch.num_columns() != m_width)
                throw std::invalid_argument("Image - channel property mismatch.");

            m_channels[pos] = std::make_unique<Channel<T>>(std::forward<ChannelType>(ch));
        }

        void exchange_channel(size_t pos1, size_t pos2) 
        {
            if (m_numChannels < 2 || pos1 > m_numChannels - 1 || pos2 > m_numChannels - 1)
                throw std::invalid_argument("Position error.");

            if (pos1 != pos2)
                m_channels[pos1].swap(m_channels[pos2]);
        }

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
