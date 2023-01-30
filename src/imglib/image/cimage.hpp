#pragma once

#include <memory>

#include <imglib/color/color.hpp>

namespace imglib 
{
	template <typename T>
	class CImage 
	{
	public:

		// Default constructor
		CImage() noexcept = default;

		// Constructor
		CImage(size_t height, size_t width, size_t numChannels, ColorSpace colorSpace, T val = T()) :
			m_height{ height },
			m_width{ width },
			m_numChannels{ numChannels },
			m_colorSpace{ colorSpace }
		{
			if (m_height == 0 || m_width == 0 || m_numChannels == 0 || !IsValid(m_colorSpace, m_numChannels))
				throw std::invalid_argument("At least one of the arguments is invalid.");

			m_data = std::make_unique<T[]>(m_height * m_width * m_numChannels);
			std::fill(m_data.get(), m_data.get() + numRows * numCols, val);
		}

		// Copy constructor

		// Move constructor

		// Copy assignment

		// Move assignment

	private:
		size_t m_height{ 0 };
		size_t m_width{ 0 };
		size_t m_numChannels{ 0 };
		ColorSpace m_colorSpace{ ColorSpace::Unspecified };
		std::unique_ptr<T[]> m_data{ nullptr };
	};
}