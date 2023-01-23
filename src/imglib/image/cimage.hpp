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