#include "ImageGenerationTests.hpp"

#include <algorithms/ImageGeneration.hpp>
#include <adaptors/JPEGAdaptor.hpp>
#include "TestConfig.hpp"

using namespace SmpImgLib;

void generate_blocks_in_grayscale()
{
    int width{ 600 }, height{ 600 };
    int oneThird{ width / 3 };
    int top{ oneThird }, bottom{ height - oneThird }, left{ oneThird }, right{ width - oneThird };
    color_mono<JpegAdapter::data_type> boxColor{ 127 };
    
    Image<JpegAdapter::data_type> img(height, width, ColorSpace::GrayScale, 1, 50);
    Algorithm::block(img, top, left, bottom, right, boxColor);

    std::string outImgPath{ output_img_path };
    outImgPath += "/block_grayscale.jpg";

    JpegAdapter::export_to_jpeg(outImgPath, 60, img);
}

void generate_blocks_in_rgb()
{
    int width{ 600 }, height{ 600 };
    int oneThird{ width / 3 };
    int top{ oneThird }, bottom{ height - oneThird }, left{ oneThird }, right{ width - oneThird };
    color_rgb<JpegAdapter::data_type> boxColor(127, 0, 0);

    Image<JpegAdapter::data_type> img(height, width, ColorSpace::RGB, 3);
    color_rgb<JpegAdapter::data_type> imgColor(100, 0, 0);
    set_color(img, imgColor);
    Algorithm::block(img, top, left, bottom, right, boxColor);

    std::string outImgPath{ output_img_path };
    outImgPath += "/block_rgb.jpg";

    JpegAdapter::export_to_jpeg(outImgPath, 60, img);
}

void generate_bars_in_grayscale()
{
    Image<JpegAdapter::data_type> img(600, 600, ColorSpace::GrayScale, 1);
    color_mono<JpegAdapter::data_type> first_color(0);
    unsigned char increment = 24;
    int width = 64;
    Algorithm::bars(img, first_color, increment, width);

    std::string outImgPath{ output_img_path };
    outImgPath += "/bars_grayscale.jpg";

    JpegAdapter::export_to_jpeg(outImgPath, 60, img);
}

void generate_bars_in_rgb()
{
    Image<JpegAdapter::data_type> img(600, 600, ColorSpace::RGB, 3);
    color_rgb<JpegAdapter::data_type> first_color(50, 100, 150);
    unsigned char increment = 20;
    int width = 20;
    Algorithm::bars(img, first_color, increment, width);

    std::string outImgPath{ output_img_path };
    outImgPath += "/bars_rgb.jpg";

    JpegAdapter::export_to_jpeg(outImgPath, 60, img);
}