#include "convolution_tests.hpp"
#include "homogeneous_point_operations_tests.hpp"
#include "shrink_tests.hpp"

#include <imglib/image/channel.hpp>
#include <iterator>
#include <concepts>

#include <imglib/utility/logger.hpp>

int main(void) 
{
	Logger log("C:/Users/myirc/source/repos/github/image_lib/data/log/log1.txt");
	log.WriteLine(1, 2);
	log.WriteLine("Test", "Dost", 34);

	GenerateAveragedImages();
	GenerateEdgeDetectedImages();
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

