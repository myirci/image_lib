#include "shrink_tests.hpp"
#include <imglib/algorithms/shrink.hpp>
#include <imglib/adaptors/jpeg_adaptor.hpp>
#include "test_config.hpp"

using namespace imglib;

void generate_shrinked_image_from_grayscale()
{
    Image<jpeg_adaptor::data_type> imgGrayscale;

    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    jpeg_adaptor::import_from_jpeg(inImgPath1, imgGrayscale);
    Image<jpeg_adaptor::data_type> imgShrinkedGrayscale = algorithm::shrink(imgGrayscale, 2);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_shrinked.jpg";
    jpeg_adaptor::export_to_jpeg(outImgPath1, 80, imgShrinkedGrayscale);
}

void generate_shrinked_image_from_rgb()
{
    Image<jpeg_adaptor::data_type> imgRgb;
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince.jpg";
    jpeg_adaptor::import_from_jpeg(inImgPath1, imgRgb);
    Image<jpeg_adaptor::data_type> imgShrinkedRgb = algorithm::shrink(imgRgb, 8);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_shrinked.jpg";
    jpeg_adaptor::export_to_jpeg(outImgPath1, 80, imgShrinkedRgb);
}
