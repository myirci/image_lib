#pragma once

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
}