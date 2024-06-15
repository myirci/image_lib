#include "convolution_tests.hpp"

#include <imglib/image/channel.hpp>
#include <iterator>
#include <concepts>

#include <imglib/utility/logger.hpp>

#include <imglib/adaptors/png_adaptor.hpp>
#include <imglib/adaptors/jpeg_adaptor.hpp>
#include <imglib/algorithms/histogram.hpp>

int main(void) 
{
	auto img = imglib::jpeg::Read(L"C:/Users/myirc/source/repos/github/image_lib/data/input/petit_prince_grayscale.jpg");
	
	imglib::algorithm::HistogramImageSettings<std::uint8_t, 1> histogramSettings{};
	histogramSettings.back = imglib::Color<std::uint8_t, 1>{ imglib::ColorSpace::GrayScale, 255 };
	histogramSettings.front = imglib::Color<std::uint8_t, 1>{ imglib::ColorSpace::GrayScale, 0 };
	histogramSettings.bin_width = 30;
	histogramSettings.padding = 20;
	histogramSettings.num_bins = 15;
	histogramSettings.max_bin_height = 512;

	auto histogramImg = imglib::algorithm::get_histogram_image<imglib::jpeg::data_type, std::uint8_t, 1>(img(0), histogramSettings);
	imglib::png::Write(histogramImg, L"C:/Users/myirc/source/repos/github/image_lib/data/output/petit_prince_graysacele_histogram1.png");

	// GenerateAveragedImages();
	// GenerateEdgeDetectedImages();
	// GenerateGradientImages();
	return 0;
}

