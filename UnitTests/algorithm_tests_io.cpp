#include "pch.h"
#include "test_helpers.h"

#include <numeric>

#include <imglib/adaptors/jpeg_adaptor.hpp>
#include <imglib/algorithms/image_generation.hpp>
#include <imglib/image/image.hpp>

using namespace imglib;

using JpegImage = Image<jpeg::data_type>;
using Point2D = Point<size_t, 2u>;

void export_jpeg(const JpegImage& img, std::string_view name, int quality = 60)
{
    std::string outImgPath{ helpers::output_img_path };
    outImgPath += name;
    jpeg::Export(outImgPath, 60, img);
}

JpegImage import_jpeg(std::string_view name)
{
    std::string inputImgPath{ helpers::input_img_path };
    inputImgPath += name;
    return jpeg::Import(inputImgPath);
}

TEST(AlgorithmTestsIO, image_block_grayscale)
{
    size_t width{ 600 }, height{ 600 };
    JpegImage img{ height, width, ColorSpace::GrayScale, 1, 50 };

    size_t oneThird{ width / 3 };
    Rectangle2D<size_t> box{ Point2D{ oneThird, oneThird },  oneThird, oneThird };

    algorithm::block(img, box, 127);
    export_jpeg(img, "block_grayscale.jpg");
}

TEST(AlgorithmTestsIO, image_block_rgb)
{
    size_t width{ 600 }, height{ 600 };
    JpegImage img{ height, width, ColorSpace::RGB, 3 };
    img.set_channels(100, 0, 0);

    size_t oneThird{ width / 3 };
    Rectangle2D<size_t> box{ Point2D{ oneThird, oneThird },  oneThird, oneThird };

    algorithm::block(img, box, 127, 0, 0);
    export_jpeg(img, "block_rgb.jpg");
    export_jpeg(JpegImage{ ColorSpace::GrayScale, img(0) }, "block_new_rgb_RedChannel.jpg");
    export_jpeg(JpegImage{ ColorSpace::GrayScale, img(1) }, "block_new_rgb_GreenChanel.jpg");
    export_jpeg(JpegImage{ ColorSpace::GrayScale, img(2) }, "block_new_rgb_BlueChannel.jpg");
}

TEST(AlgorithmTestsIO, image_bars_grayscale)
{
    JpegImage img(600, 600, ColorSpace::GrayScale, 1);
    jpeg::data_type increment = 24;
    size_t width = 64;
    algorithm::bars(img, width, increment, 0);
    export_jpeg(img, "bars_grayscale.jpg");
}

TEST(AlgorithmTestsIO, image_bars_rgb)
{
    JpegImage img(600, 600, ColorSpace::RGB, 3);
    jpeg::data_type increment = 20;
    size_t width = 20;
    algorithm::bars(img, width, increment, 50, 100, 150);
    export_jpeg(img, "bars_rgb.jpg");
}

TEST(AlgorithmTestsIO, image_horizontal_linear_gradient_grayscale) 
{
    Color<jpeg::data_type, 1> start{ ColorSpace::GrayScale, 0 };
    Color<jpeg::data_type, 1> end{ ColorSpace::GrayScale, 255 };
    auto img = algorithm::horizontal_linear_gradient(600, 30, 20, start, end);
    export_jpeg(img, "horizontal_linear_gradient_grayscale_1.jpg");
}

TEST(AlgorithmTestsIO, image_horizontal_linear_gradient_rgb1)
{
    Color<jpeg::data_type, 3> start{ ColorSpace::RGB, 255, 0, 0 };
    Color<jpeg::data_type, 3> end{ ColorSpace::RGB, 255, 255, 255 };
    auto img = algorithm::horizontal_linear_gradient(600, 1, 600, start, end);
    export_jpeg(img, "horizontal_linear_gradient_rgb_1.jpg");
}

TEST(AlgorithmTestsIO, image_horizontal_linear_gradient_rgb2)
{
    Color<jpeg::data_type, 3> start{ ColorSpace::RGB, 123, 12, 89 };
    Color<jpeg::data_type, 3> end{ ColorSpace::RGB, 45, 155, 17 };
    auto img = algorithm::horizontal_linear_gradient(600, 20, 30, start, end);
    export_jpeg(img, "horizontal_linear_gradient_rgb_2.jpg");
}

TEST(AlgorithmTestsIO, image_horizontal_linear_gradient_rgb3)
{
    Color<jpeg::data_type, 3> start{ ColorSpace::RGB, 123, 12, 89 };
    Color<jpeg::data_type, 3> end{ ColorSpace::RGB, 45, 155, 17 };
    auto img = algorithm::horizontal_linear_gradient(600, 2, 300, start, end);
    export_jpeg(img, "horizontal_linear_gradient_rgb_3.jpg");
}

TEST(AlgorithmTestsIO, image_rgb_to_grayscale_conversion)
{
    auto imgRgb = import_jpeg("petit_prince.jpg");
    auto imgGray = rgb_to_grayscale(imgRgb);
    export_jpeg(imgGray, "petit_prince_grayscale.jpg");
}
