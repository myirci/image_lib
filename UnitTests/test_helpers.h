#pragma once

#include <string_view>

namespace helpers 
{
	template <typename T>
	bool AllPixelsEqualTo(T const* data, size_t size, T expectedVal)
	{
		for (auto i = 0; i < size; i++)
			if (data[i] != expectedVal)
				return false;
		return true;
	}

	inline constexpr std::wstring_view input_img_path = L"C:/Users/myirc/source/repos/github/image_lib/data/input/";
	inline constexpr std::wstring_view output_img_path = L"C:/Users/myirc/source/repos/github/image_lib/data/output/";
}