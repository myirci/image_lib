#pragma once

#include <vector>
#include <limits>
#include <imglib/image/image.hpp>
#include <imglib/color/color.hpp>
#include <imglib/utility/utility.hpp>

namespace imglib::algorithm 
{
	template<typename T>
	std::vector<size_t> get_histogram(const Channel<T>& ch, size_t numBins)
	{
		std::vector<size_t> histogram(numBins, 0);

		auto data_range = std::numeric_limits<T>::max() - std::numeric_limits<T>::min();
		double step_size = (double)data_range / (double)numBins;

		for (auto it = ch.cbegin(); it != ch.cend(); it++)
			histogram[(size_t)(*it / step_size)]++;

		return histogram;
	}

	template<typename T, size_t NumChannels>
		requires (NumChannels == 1 || NumChannels == 3)
	struct HistogramImageSettings 
	{
		Color<T, NumChannels> front;
		Color<T, NumChannels> back;
		size_t num_bins{ 10 };
		size_t bin_width{ 1 };
		size_t max_bin_height{ 488 };
		size_t padding{ 12 };
	};

	template<typename T1, typename T2, size_t NumChannels>
	Image<T2> get_histogram_image(const Channel<T1>& ch, const HistogramImageSettings<T2, NumChannels>& settings)
	{
		auto hist = get_histogram(ch, settings.num_bins);
		auto normalized_hist = normalize(hist);
		constexpr auto maxRangeVal = std::numeric_limits<T2>::max();

		size_t height = settings.max_bin_height + 2 * settings.padding;
		size_t width = settings.bin_width * settings.num_bins + 2 * settings.padding;

		// Create the histogram image
		auto histogramImg = Image<T2>{ height, width, settings.front.color_space(), NumChannels};

		// Set the background color
		histogramImg = settings.back;

		// Create the bins using the front color
		for (size_t i = 0; i < normalized_hist.size(); i++)
		{
			size_t bin_height = normalized_hist[i] * settings.max_bin_height;

			if (bin_height == 0)
				continue;

			size_t col_start = settings.padding + i * settings.bin_width + 1;
			size_t col_end = col_start + settings.bin_width;

			size_t row_start = histogramImg.height() - settings.padding - 1;
			size_t row_end = row_start - bin_height;

			for (size_t col = col_start; col < col_end; col++)
				for (size_t row = row_start; row > row_end; row--)
					histogramImg.set_pixel(row, col, settings.front);		
		}

		return histogramImg;
	}
}