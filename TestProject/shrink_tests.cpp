#include "shrink_tests.hpp"
#include <imglib/algorithms/shrink.hpp>
#include <imglib/adaptors/jpeg_adaptor.hpp>
#include "test_config.hpp"

using namespace imglib;

void GenerateShrinkedImageGrayscale()
{
    Image<jpeg::data_type> imgGrayscale;

    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    jpeg::Import(inImgPath1, imgGrayscale);
    Image<jpeg::data_type> imgShrinkedGrayscale = algorithm::shrink(imgGrayscale, 2);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_shrinked.jpg";
    jpeg::Export(outImgPath1, 80, imgShrinkedGrayscale);
}

void GenerateShrinkedImageRGB()
{
    Image<jpeg::data_type> imgRgb;
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince.jpg";
    jpeg::Import(inImgPath1, imgRgb);
    Image<jpeg::data_type> imgShrinkedRgb = algorithm::shrink(imgRgb, 8);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_shrinked.jpg";
    jpeg::Export(outImgPath1, 80, imgShrinkedRgb);
}