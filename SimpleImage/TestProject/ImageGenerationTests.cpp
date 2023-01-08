#include "ImageGenerationTests.hpp"

#include <algorithms/ImageGeneration.hpp>
#include <adaptors/JPEGAdaptor.hpp>
#include "TestConfig.hpp"

using namespace imglib;

void generate_blocks_in_grayscale()
{
    int width{ 600 }, height{ 600 };
    int oneThird{ width / 3 };
    int top{ oneThird }, bottom{ height - oneThird }, left{ oneThird }, right{ width - oneThird };
    color_mono<jpeg_adaptor::data_type> boxColor{ 127 };
    
    Image<jpeg_adaptor::data_type> img(height, width, ColorSpace::GrayScale, 1, 50);
    algorithm::block(img, top, left, bottom, right, boxColor);

    std::string outImgPath{ output_img_path };
    outImgPath += "/block_grayscale.jpg";

    jpeg_adaptor::export_to_jpeg(outImgPath, 60, img);
}

void generate_blocks_in_rgb()
{
    int width{ 600 }, height{ 600 };
    int oneThird{ width / 3 };
    int top{ oneThird }, bottom{ height - oneThird }, left{ oneThird }, right{ width - oneThird };
    color_rgb<jpeg_adaptor::data_type> boxColor(127, 0, 0);

    Image<jpeg_adaptor::data_type> img(height, width, ColorSpace::RGB, 3);
    color_rgb<jpeg_adaptor::data_type> imgColor(100, 0, 0);
    set_color(img, imgColor);
    algorithm::block(img, top, left, bottom, right, boxColor);

    std::string outImgPath{ output_img_path };
    outImgPath += "/block_rgb.jpg";

    jpeg_adaptor::export_to_jpeg(outImgPath, 60, img);
}

void generate_bars_in_grayscale()
{
    Image<jpeg_adaptor::data_type> img(600, 600, ColorSpace::GrayScale, 1);
    color_mono<jpeg_adaptor::data_type> first_color(0);
    unsigned char increment = 24;
    int width = 64;
    algorithm::bars(img, first_color, increment, width);

    std::string outImgPath{ output_img_path };
    outImgPath += "/bars_grayscale.jpg";

    jpeg_adaptor::export_to_jpeg(outImgPath, 60, img);
}

void generate_bars_in_rgb()
{
    Image<jpeg_adaptor::data_type> img(600, 600, ColorSpace::RGB, 3);
    color_rgb<jpeg_adaptor::data_type> first_color(50, 100, 150);
    unsigned char increment = 20;
    int width = 20;
    algorithm::bars(img, first_color, increment, width);

    std::string outImgPath{ output_img_path };
    outImgPath += "/bars_rgb.jpg";

    jpeg_adaptor::export_to_jpeg(outImgPath, 60, img);
}