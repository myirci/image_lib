#include "color_space_conversion_tests.hpp"
#include <imglib/algorithms/color_space_conversion.hpp>
#include <imglib/adaptors/jpeg_adaptor.hpp>
#include "test_config.hpp"

using namespace imglib;

void GenerateGrayscaleImageFromRGBImage()
{
    Image<jpeg::data_type> img_rgb;

    std::string inImgPath{ input_img_path };
    inImgPath += "/petit_prince.jpg";
    
    jpeg::Import(inImgPath, img_rgb);
    
    Image<jpeg::data_type> img_grayscale(img_rgb.get_height(), img_rgb.get_width(), ColorSpace::GrayScale, 1);   
    algorithm::convert_rgb_to_grayscale(img_rgb, img_grayscale);

    std::string outImgPath{ output_img_path };
    outImgPath += "/petit_prince_grayscale.jpg";
    jpeg::Export(outImgPath, 90, img_grayscale);
}