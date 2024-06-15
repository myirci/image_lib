#include "pch.h"
#include "test_helpers.h"

#include <numeric>

#include <imglib/adaptors/jpeg_adaptor.hpp>
#include <imglib/adaptors/png_adaptor.hpp>
#include <imglib/algorithms/image_generation.hpp>
#include <imglib/algorithms/geometric_modifications.hpp>
#include <imglib/algorithms/homogeneous_point_operations.hpp>
#include <imglib/image/image.hpp>

using namespace imglib;

using Point2D = Point<size_t, 2u>;

void write_jpeg(const jpeg::JpegImg& img, std::wstring_view name, int quality = 60)
{
    std::wstring outImgPath{ helpers::output_img_path };
    outImgPath += name;
    jpeg::Write(outImgPath, 60, img);
}

jpeg::JpegImg read_jpeg(std::wstring_view name)
{
    std::wstring inputImgPath{ helpers::input_img_path };
    inputImgPath += name;
    return jpeg::Read(inputImgPath);
}

void write_png(const png::PngImg& img, std::wstring_view name)
{
    std::wstring outImgPath{ helpers::output_img_path };
    outImgPath += name;
    png::Write(img, outImgPath);
}

png::PngImg read_png(std::wstring_view name)
{
    std::wstring inputImgPath{ helpers::input_img_path };
    inputImgPath += name;
    return png::Read(inputImgPath);
}

TEST(AlgorithmTestsIO, image_block_grayscale)
{
    size_t width{ 600 }, height{ 600 };
    jpeg::JpegImg img{ height, width, ColorSpace::GrayScale, 1, 50 };

    size_t oneThird{ width / 3 };
    Rectangle2D<size_t> box{ Point2D{ oneThird, oneThird },  oneThird, oneThird };

    algorithm::block(img, box, 127);
    write_jpeg(img, L"block_grayscale.jpg");
    write_png(img, L"block_grapyscale.png");
}

TEST(AlgorithmTestsIO, image_block_rgb)
{
    size_t width{ 600 }, height{ 600 };
    jpeg::JpegImg img{ height, width, ColorSpace::RGB, 3 };
    img.set_channels(100, 124, 145);

    size_t oneThird{ width / 3 };
    Rectangle2D<size_t> box{ Point2D{ oneThird, oneThird },  oneThird, oneThird };

    algorithm::block(img, box, 127, 50, 70);
    write_jpeg(img, L"block_rgb.jpg");
    write_jpeg(jpeg::JpegImg{ ColorSpace::GrayScale, img(0) }, L"block_RedChannel.jpg");
    write_jpeg(jpeg::JpegImg{ ColorSpace::GrayScale, img(1) }, L"block_GreenChanel.jpg");
    write_jpeg(jpeg::JpegImg{ ColorSpace::GrayScale, img(2) }, L"block_BlueChannel.jpg");

    write_png(img, L"block_rgb.png");
    write_png(jpeg::JpegImg{ ColorSpace::GrayScale, img(0) }, L"block_RedChannel.png");
    write_png(jpeg::JpegImg{ ColorSpace::GrayScale, img(1) }, L"block_GreenChanel.png");
    write_png(jpeg::JpegImg{ ColorSpace::GrayScale, img(2) }, L"block_BlueChannel.png");
}

TEST(AlgorithmTestsIO, image_bars_grayscale)
{
    jpeg::JpegImg img(600, 600, ColorSpace::GrayScale, 1);
    jpeg::data_type increment = 24;
    size_t width = 64;
    algorithm::bars(img, width, increment, 0);
    write_jpeg(img, L"bars_grayscale.jpg");
    write_png(img, L"bars_grayscale.png");
}

TEST(AlgorithmTestsIO, image_bars_rgb)
{
    jpeg::JpegImg img(600, 600, ColorSpace::RGB, 3);
    jpeg::data_type increment = 20;
    size_t width = 20;
    algorithm::bars(img, width, increment, 50, 100, 150);
    write_jpeg(img, L"bars_rgb.jpg");
    write_png(img, L"bars_rgb.png");
}

TEST(AlgorithmTestsIO, image_horizontal_linear_gradient_grayscale) 
{
    Color<jpeg::data_type, 1> start{ ColorSpace::GrayScale, 0 };
    Color<jpeg::data_type, 1> end{ ColorSpace::GrayScale, 255 };
    auto img = algorithm::horizontal_linear_gradient(600, 30, 20, start, end);
    write_jpeg(img, L"horizontal_linear_gradient_grayscale_1.jpg");
    write_png(img, L"horizontal_linear_gradient_grayscale_1.png");
}

TEST(AlgorithmTestsIO, image_horizontal_linear_gradient_rgb1)
{
    Color<jpeg::data_type, 3> start{ ColorSpace::RGB, 255, 0, 0 };
    Color<jpeg::data_type, 3> end{ ColorSpace::RGB, 255, 255, 255 };
    auto img = algorithm::horizontal_linear_gradient(600, 1, 600, start, end);
    write_jpeg(img, L"horizontal_linear_gradient_rgb_1.jpg");
    write_png(img, L"horizontal_linear_gradient_rgb_1.png");
}

TEST(AlgorithmTestsIO, image_horizontal_linear_gradient_rgb2)
{
    Color<jpeg::data_type, 3> start{ ColorSpace::RGB, 123, 12, 89 };
    Color<jpeg::data_type, 3> end{ ColorSpace::RGB, 45, 155, 17 };
    auto img = algorithm::horizontal_linear_gradient(600, 20, 30, start, end);
    write_jpeg(img, L"horizontal_linear_gradient_rgb_2.jpg");
    write_png(img, L"horizontal_linear_gradient_rgb_2.png");
}

TEST(AlgorithmTestsIO, image_horizontal_linear_gradient_rgb3)
{
    Color<jpeg::data_type, 3> start{ ColorSpace::RGB, 123, 12, 89 };
    Color<jpeg::data_type, 3> end{ ColorSpace::RGB, 45, 155, 17 };
    auto img = algorithm::horizontal_linear_gradient(600, 2, 300, start, end);
    write_jpeg(img, L"horizontal_linear_gradient_rgb_3.jpg");
    write_png(img, L"horizontal_linear_gradient_rgb_3.png");
}

TEST(AlgorithmTestsIO, image_rgb_to_grayscale_conversion)
{
    auto imgRgb = read_jpeg(L"petit_prince.jpg");
    auto imgGray = rgb_to_grayscale(imgRgb);
    write_jpeg(imgGray, L"petit_prince_grayscale.jpg");
    write_png(imgGray, L"petit_prince_grayscale.png");
}

TEST(AlgorithmTestsIO, image_shrink_grayscale)
{
    auto imgGrayscale = read_jpeg(L"petit_prince_grayscale.jpg");
    Image<jpeg::data_type> imgShrinkedGrayscale = algorithm::Shrink(imgGrayscale, 2);
    write_jpeg(imgShrinkedGrayscale, L"petit_prince_grayscale_shrinked.jpg");
    write_png(imgShrinkedGrayscale, L"petit_prince_grayscale_shrinked.png");
}

TEST(AlgorithmTestsIO, image_shrink_rgb)
{
    auto imgRgb = read_jpeg(L"petit_prince.jpg");
    Image<jpeg::data_type> imgShrinkedRgb = algorithm::Shrink(imgRgb, 8);
    write_jpeg(imgShrinkedRgb, L"petit_prince_rgb_shrinked.jpg");
    write_png(imgShrinkedRgb, L"petit_prince_rgb_shrinked.png");
}

TEST(AlgorithmTestsIO, image_increase_contrast_grayscale) 
{
    auto img = read_jpeg(L"petit_prince_grayscale.jpg");
    algorithm::contrast(img, 1.5);
    write_jpeg(img, L"petit_prince_grayscale_increased_contrast.jpg");
    write_png(img, L"petit_prince_grayscale_increased_contrast.png");
}

TEST(AlgorithmTestsIO, image_increase_contrast_rgb) 
{
    auto img = read_jpeg(L"petit_prince.jpg");
    algorithm::contrast(img, 1.5);
    write_jpeg(img, L"petit_prince_rgb_increased_contrast.jpg");
    write_png(img, L"petit_prince_rgb_increased_contrast.png");
}

TEST(AlgorithmTestsIO, image_decrease_contrast_grayscale)
{
    auto img = read_jpeg(L"petit_prince_grayscale.jpg");
    algorithm::contrast(img, 0.5);
    write_jpeg(img, L"petit_prince_grayscale_decreased_contrast.jpg");
    write_png(img, L"petit_prince_grayscale_decreased_contrast.png");
}

TEST(AlgorithmTestsIO, image_decrease_contrast_rgb)
{
    auto img = read_jpeg(L"petit_prince.jpg");
    algorithm::contrast(img, 0.5);
    write_jpeg(img, L"petit_prince_rgb_decreased_contrast.jpg");
    write_png(img, L"petit_prince_rgb_decreased_contrast.png");
}

TEST(AlgorithmTestsIO, image_invert_grayscale) 
{
    auto img = read_jpeg(L"petit_prince_grayscale.jpg");
    algorithm::invert(img);
    write_jpeg(img, L"petit_prince_grayscale_inverted.jpg");
    write_png(img, L"petit_prince_grayscale_inverted.png");
}

TEST(AlgorithmTestsIO, image_invert_rgb)
{
    auto img = read_jpeg(L"petit_prince.jpg");
    algorithm::invert(img);
    write_jpeg(img, L"petit_prince_rgb_inverted.jpg");
    write_png(img, L"petit_prince_rgb_inverted.png");
}