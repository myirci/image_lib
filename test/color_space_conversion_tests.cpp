#include "color_space_conversion_tests.hpp"
#include "color_space_conversion.hpp"
#include "jpeg_adapter.hpp"

using namespace SmpImgLib;

void generate_grayscale_from_rgb_image()
{
    Image<JpegAdapter::data_type> img_rgb;
    JpegAdapter::import_from_jpeg("../data/petit_prince.jpg", img_rgb);
    Image<JpegAdapter::data_type> img_grayscale(img_rgb.get_height(), img_rgb.get_width(), ColorSpace::GrayScale, 1);   
    convert_rgb_to_grayscale(img_rgb, img_grayscale);
    JpegAdapter::export_to_jpeg("petit_prince_grayscale.jpg", 90, img_grayscale);    
}