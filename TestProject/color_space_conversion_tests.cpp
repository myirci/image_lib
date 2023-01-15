#include "color_space_conversion_tests.hpp"
#include <imglib/algorithms/color_space_conversion.hpp>
#include <imglib/adaptors/jpeg_adaptor.hpp>
#include "test_config.hpp"

using namespace imglib;

void GenerateGrayscaleImageFromRGBImage()
{
    Image<jpeg::data_type> img_rgb;

    std::string inimgpath{ input_img_path };
    inimgpath += "/petit_prince.jpg";
    
    jpeg::Import(inimgpath, img_rgb);
    
    Image<jpeg::data_type> img_grayscale(img_rgb.get_height(), img_rgb.get_width(), ColorSpace::GrayScale, 1);   
    algorithm::convert_rgb_to_grayscale(img_rgb, img_grayscale);

    std::string outimgpath{ output_img_path };
    outimgpath += "/petit_prince_grayscale.jpg";
    jpeg::Export(outimgpath, 90, img_grayscale);
}