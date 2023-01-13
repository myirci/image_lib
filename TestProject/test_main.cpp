
#include "image_generation_tests.hpp"
#include "color_space_conversion_tests.hpp"
#include "convolution_tests.hpp"
#include "homogeneous_point_operations_tests.hpp"
#include "shrink_tests.hpp"


#include <imglib/image/channel.hpp>
#include <iterator>
#include <concepts>

int main(void) 
{
	static_assert(std::contiguous_iterator<imglib::ChannelIterator<int>>);

	GenerateBlocksInGrayscale();
	GenerateBlocksInRGB();
	GenerateBarsInGrayscale();
	GenerateBarsInRGB();

	GenerateGrayscaleImageFromRGBImage();

	GenerateEdgeDetectedImages();
	GenerateAveragedImages();
	GenerateGradientImages();

	GenerateIncreasedContrastImages();
	GenerateDecreasedContrastImages();
	GenerateAutoContrastImages();
	GenerateModifiedAutoContrastImages();
	GenerateInvertedImages();

	GenerateShrinkedImageGrayscale();
	GenerateShrinkedImageRGB();

	return 0;
}

