#pragma once

#include <vector>
#include <concepts>

namespace imglib
{
    enum class ColorSpace : unsigned char
    {
        Unspecified,
        GrayScale,
        GrayScaleAlpha, // Grayscale with alpha channel 
        RGB,            // Additive color model, 3 channels - Red, Green, Blue
        RGBA,           // RGB with alpha channel
        YCbCr,
        CMYK,           // Substractive color model, 4 channels - Cyan, Magenta, Yellow, Key (black)
        YCCK
    };

    template<typename T> class Image;

    template <typename T>
    T interpolate(T start, T end, double ratio) { return static_cast<T>(start * (1.0 - ratio) + end * ratio); }

    template <typename T, size_t NumChannels>
    class Color
    {
    public:

        Color() = default;

        template <std::convertible_to<T>... U>
           requires (sizeof...(U) == NumChannels)
        Color(ColorSpace colorSpace, U... args) : m_colorSpace{ colorSpace }
        {
            size_t i = 0;
            for (T arg : { args... })
                m_values[i++] = arg;
        }

        T& operator()(size_t i) { return m_values[i]; }

        T const& operator()(size_t i) const { return m_values[i]; }

        size_t num_channels() const noexcept { return NumChannels; }

        ColorSpace color_space() const noexcept { return m_colorSpace; }

    private:
        T m_values[NumChannels];
        ColorSpace m_colorSpace;
    };

    template <typename T, size_t NumChannels>
    Color<T, NumChannels> GetColor(const Color<T, NumChannels>& start, const Color<T, NumChannels>& end, double ratio)
    {
        Color<T, NumChannels> clr;
        for (size_t i = 0; i < NumChannels; i++)
            clr(i) = interpolate(start(i), end(i), ratio);

        return clr;
    }

    template <typename T, size_t NumChannels>
    std::vector<Color<T, NumChannels>> GetColorStops(const Color<T, NumChannels>& start, const Color<T, NumChannels>& end, size_t numColors)
    {
        std::vector<Color<T, NumChannels>> colors = { start };

        double step = 1.0 / (numColors - 1);
        double r = step;

        for (size_t i = 0; i < numColors - 2; i++, r += step) 
            colors.emplace_back(GetColor(start, end, r));

        colors.push_back(end);

        return colors;
    }

    template<typename T>
    Image<T> rgb_to_grayscale(const Image<T>& rgbImg)
    {
        Image<T> grayImage{ rgbImg.height(), rgbImg.width(), ColorSpace::GrayScale, 1 };

        for (size_t i = 0; i < rgbImg.size(); ++i)
            grayImage(0)(i) = (rgbImg(0)(i) + rgbImg(1)(i) + rgbImg(2)(i)) / 3;

        return grayImage;
    }
}


