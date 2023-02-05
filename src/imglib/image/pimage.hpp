#pragma once

#include <vector>

namespace imglib 
{
	template <typename T, int NumChannels>
	struct Pixel
	{
		T p[NumChannels];
	};

	template <typename T, int NumChannels>
	class PImage 
	{
	private:
		std::vector<Pixel<T, NumChannels>> m_pixels;
	};
}