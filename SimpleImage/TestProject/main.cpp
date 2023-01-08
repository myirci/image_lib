
#include "image_generation_tests.hpp"
#include "color_space_conversion_tests.hpp"
#include "convolution_tests.hpp"
#include "homogeneous_point_operations_tests.hpp"
#include "shrink_tests.hpp"


int main(void) 
{
	generate_blocks_in_grayscale();
	generate_blocks_in_rgb();
	generate_bars_in_grayscale();
	generate_bars_in_rgb();

    generate_grayscale_from_rgb_image();

    generate_edge_detected_images();
    generate_averaged_images();
    generate_gradient_images();

	generate_increased_contrast_images();
	generate_decreased_contrast_images();
	generate_auto_contrast_images();
	generate_modified_auto_contrast_images();
	generate_inverted_images();

	generate_shrinked_image_from_grayscale();
	generate_shrinked_image_from_rgb();

	return 0;
}

