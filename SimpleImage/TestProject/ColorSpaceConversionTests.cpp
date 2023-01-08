#include "ColorSpaceConversionTests.hpp"
#include <algorithms/ColorSpaceConversion.hpp>
#include <adaptors/JPEGAdaptor.hpp>
#include "TestConfig.hpp"

using namespace imglib;

void generate_grayscale_from_rgb_image()
{
    Image<jpeg_adaptor::data_type> img_rgb;

    std::string inImgPath{ input_img_path };
    inImgPath += "/petit_prince.jpg";
    
    jpeg_adaptor::import_from_jpeg(inImgPath, img_rgb);
    
    Image<jpeg_adaptor::data_type> img_grayscale(img_rgb.get_height(), img_rgb.get_width(), ColorSpace::GrayScale, 1);   
    algorithm::convert_rgb_to_grayscale(img_rgb, img_grayscale);

    std::string outImgPath{ output_img_path };
    outImgPath += "/petit_prince_grayscale.jpg";
    jpeg_adaptor::export_to_jpeg(outImgPath, 90, img_grayscale);
}