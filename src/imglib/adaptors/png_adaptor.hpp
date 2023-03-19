#pragma once

#include <png.h>
#include <variant>

#include <imglib/image/image.hpp>

namespace imglib::png 
{
	using ImgBitDepth8 = Image<std::uint8_t>;
	using ImgBitDepth16 = Image<std::uint16_t>;
	using PngImg = std::variant<std::monostate, ImgBitDepth8, ImgBitDepth16>;

	struct PngInfo
	{
		png_uint_32 width{ 0 };
		png_uint_32 height{ 0 };
		int color_type{ -1 };
		int bit_depth{ -1 };        
		int interlace_type{ -1 };   
		int compression_type{ -1 }; 
		int filter_method{ -1 };
	};

	PngImg Read(std::wstring_view fileName);

	void Write(const PngImg& img, std::wstring_view fileName);
}