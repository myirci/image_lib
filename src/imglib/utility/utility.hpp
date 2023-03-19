#pragma once

#include <imglib/color/color.hpp>
#include <string_view>

namespace imglib 
{
	bool IsValid(ColorSpace cSpace, size_t numChannels);

	enum class Mode
	{
		Read,
		Write
	};

	FILE* OpenFile(std::wstring_view fileName, Mode mode);
}