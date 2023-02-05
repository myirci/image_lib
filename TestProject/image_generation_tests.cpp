#include "image_generation_tests.hpp"

#include <imglib/algorithms/image_generation.hpp>
#include <imglib/adaptors/jpeg_adaptor.hpp>
#include "test_config.hpp"

using namespace imglib;

void GenerateBarsInGrayscale()
{
    Image<jpeg::data_type> img(600, 600, ColorSpace::GrayScale, 1);
    color_mono<jpeg::data_type> first_color(0);
    unsigned char increment = 24;
    int width = 64;
    algorithm::bars(img, first_color, increment, width);
    std::string outImgPath{ output_img_path };
    outImgPath += "/bars_grayscale.jpg";
    jpeg::Export(outImgPath, 60, img);
}

void GenerateBarsInRGB()
{
    Image<jpeg::data_type> img(600, 600, ColorSpace::RGB, 3);
    color_rgb<jpeg::data_type> first_color(50, 100, 150);
    unsigned char increment = 20;
    int width = 20;
    algorithm::bars(img, first_color, increment, width);
    std::string outImgPath{ output_img_path };
    outImgPath += "/bars_rgb.jpg";
    jpeg::Export(outImgPath, 60, img);
}

void GenerateBlocksInGrayscale()
{
    int width{ 600 }, height{ 600 };
    int oneThird{ width / 3 };
    int top{ oneThird }, bottom{ height - oneThird }, left{ oneThird }, right{ width - oneThird };
    color_mono<jpeg::data_type> boxColor{ 127 };
    
    Image<jpeg::data_type> img(height, width, ColorSpace::GrayScale, 1, 50);
    algorithm::block(img, top, left, bottom, right, boxColor);

    std::string outImgPath{ output_img_path };
    outImgPath += "/block_grayscale.jpg";

    jpeg::Export(outImgPath, 60, img);
}

void GenerateBlocksInRGB()
{
    int width{ 600 }, height{ 600 };
    int oneThird{ width / 3 };
    int top{ oneThird }, bottom{ height - oneThird }, left{ oneThird }, right{ width - oneThird };
    color_rgb<jpeg::data_type> boxColor(127, 0, 0);

    Image<jpeg::data_type> img(height, width, ColorSpace::RGB, 3);
    jpeg::data_type cl[3] = {100, 0, 0};
    img.set_channels(cl[0], cl[1], cl[2]);
    algorithm::block(img, top, left, bottom, right, boxColor);

    std::string outImgPath{ output_img_path };
    outImgPath += "/block_rgb.jpg";

    jpeg::Export(outImgPath, 60, img);
}