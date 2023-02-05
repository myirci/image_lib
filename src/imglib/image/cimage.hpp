#pragma once

#include <memory>
#include <algorithm>

#include <imglib/color/color.hpp>
#include <imglib/utility/utility.hpp>

namespace imglib 
{
	template <typename T, size_t NumChannels>
		requires (NumChannels >= 1)
	class CImage 
	{
	public:

		// Default constructor
		CImage() noexcept = default;

		// Constructor
		CImage(size_t height, size_t width, ColorSpace colorSpace, T val = T()) :
			m_height{ height },
			m_width{ width },
			m_colorSpace{ colorSpace }
		{
			if (m_height < 1 || m_width < 1 || !IsValid(m_colorSpace, NumChannels))
				throw std::invalid_argument("At least one of the arguments is invalid.");

			auto sz = data_size();
			m_data = std::make_unique<T[]>(sz);
			std::fill(m_data.get(), m_data.get() + sz, val);
		}

		// Copy constructor
		CImage(const CImage<T, NumChannels>& other) : m_height{ other.m_height }, m_width{ other.m_width }, m_colorSpace{ other.m_colorSpace }, m_data{ std::make_unique<T[]>(other.data_size()) }
		{
			memcpy(static_cast<void*>(m_data.get()), static_cast<void*>(other.m_data.get()), other.data_size() * sizeof(T));
		}

		// Move constructor
		CImage(CImage<T, NumChannels>&& other) noexcept
		{
			*this = std::move(other);
		}

		// Copy assignment
		CImage<T, NumChannels>& operator=(const CImage<T, NumChannels>& other) 
		{
			if (this == &other)
				return *this;

			auto temp = other;
			*this = std::move(temp);
			return *this;
		}

		// Move assignment
		CImage<T, NumChannels>& operator=(CImage<T, NumChannels>&& other) noexcept
		{
			 if (this == &other)
				return *this;

			m_height = other.m_height;
			m_width = other.m_width;
			m_colorSpace = other.m_colorSpace;
			m_data = std::move(other.m_data);
			other.clear();

			return *this;
		}

		T& operator()(size_t pixelIndex, size_t ch) { return m_data[pixelIndex * NumChannels + ch]; }

		T const& operator()(size_t pixelIndex, size_t ch) const { return m_data[pixelIndex * NumChannels + ch]; }

		T& operator()(size_t row, size_t col, size_t ch) { return m_data[row * m_width + col + ch]; }

		T const& operator()(size_t row, size_t col, size_t ch) const { return m_data[row * m_width + col + ch]; }

		void operator=(T value) { std::fill(m_data.get(), m_data.get() + data_size(), value); }

		size_t width() const noexcept { return m_width; }

		size_t height() const noexcept { return m_height; }

		constexpr size_t num_channels() const noexcept { return NumChannels; }

		ColorSpace color_space() const noexcept { return m_colorSpace; }

		size_t size() const noexcept { return m_width * m_height; }

		size_t data_size() const noexcept { return size() * NumChannels; }

		T const* data() const noexcept { return m_data.get(); }

		void clear() 
		{
			m_height = 0;
			m_width = 0;
			m_data.reset();
		}

		std::unique_ptr<T[]> get_channel(size_t pos) const 
		{
			auto sz = size();
			if (sz < 1)
				return nullptr;

			auto ch = std::make_unique<T[]>(sz);

			size_t j{ 0 };
			for (size_t i = pos; i < data_size(); i += NumChannels)
				ch[j++] = m_data[i];

			return ch;
		}

		template <std::same_as<T> ...U>
			requires (sizeof...(U) >= 1 && sizeof...(U) <= NumChannels)
		void set_channels(U... args)
		{
			size_t i{ 0 };
			for (T arg : { args... })
				set_channel(i++, arg);
		}

		void set_channel(size_t pos, T val) 
		{
			for (size_t i = pos; i < data_size(); i += NumChannels)
				m_data[i] = val;
		}

		bool resize(size_t height, size_t width) 
		{
			if (height == m_height && width == m_width)
				return false;

			std::unique_ptr<T[]> buffer{ nullptr };

			try
			{
				buffer = std::make_unique<T[]>(height * width * NumChannels);
			}
			catch (...)
			{
				return false;
			}

			m_data = std::move(buffer);
			m_height = height;
			m_width = width;

			return true;
		}

		template <std::same_as<T> ...U>
			requires (sizeof...(U) == NumChannels)
		void set_pixel(size_t pixelIndex, U... args)
		{
			auto i = pixelIndex * NumChannels;
			for (T arg : { args... })
				m_data[i++] = arg;
		}

		template <std::same_as<T> ...U>
			requires (sizeof...(U) == NumChannels)
		void set_pixel(size_t row, size_t col, U... args)
		{
			set_pixel(row * m_width + col, args...);
		}

	private:
		size_t m_height{ 0 };
		size_t m_width{ 0 };
		ColorSpace m_colorSpace{ ColorSpace::Unspecified };
		std::unique_ptr<T[]> m_data{ nullptr };
	};
}