#include "pch.h"
#include "test_helpers.h"

#include <numeric>
#include <array>

#include <imglib/adaptors/jpeg_adaptor.hpp>
#include <imglib/algorithms/image_generation.hpp>
#include <imglib/algorithms/histogram.hpp>
#include <imglib/image/image.hpp>

using namespace imglib;

using Point2D = Point<size_t, 2u>;

TEST(AlgorithmTests, image_block_grayscale)
{
    jpeg::JpegImg img{ 8, 8, ColorSpace::GrayScale, 1, 50 };

    Rectangle2D<size_t> box1{ Point2D{ 2, 1 },  3, 2 };
    algorithm::block(img, box1, 127);
    size_t expectedSum1 = static_cast<size_t>(64 * 50) + static_cast<size_t>(6 * 77);
    size_t sum1 = std::accumulate(img(0).begin(), img(0).end(), 0);
    EXPECT_EQ(expectedSum1, sum1);

    img = 20;
    Rectangle2D<size_t> box2{ Point2D{ 0, 0 },  8, 8 };
    algorithm::block(img, box2, 127);
    size_t expectedSum2 = static_cast<size_t>(64) * 127;
    size_t sum2 = std::accumulate(img(0).begin(), img(0).end(), 0);
    EXPECT_EQ(expectedSum2, sum2);

    img = 10;
    Rectangle2D<size_t> box3{ Point2D{ 4, 4 },  1, 1 };
    algorithm::block(img, box3, 25);
    size_t expectedSum3 = static_cast<size_t>(63 * 10) + 25;
    size_t sum3 = std::accumulate(img(0).begin(), img(0).end(), 0);
    EXPECT_EQ(expectedSum3, sum3);
}

TEST(AlgorithmTests, image_block_rgb)
{
    jpeg::JpegImg img{ 8, 8, ColorSpace::RGB, 3 };
    img.set_channels(34, 45, 179);

    // Test-1
    Rectangle2D<size_t> box1{ Point2D{ 2, 1 },  3, 2 };
    algorithm::block(img, box1, 67, 129, 156);

    size_t expectedSum1 = static_cast<size_t>(58 * 34) + static_cast<size_t>(6 * 67);
    size_t sum1 = std::accumulate(img(0).begin(), img(0).end(), 0);
    EXPECT_EQ(expectedSum1, sum1);

    size_t expectedSum2 = static_cast<size_t>(58 * 45) + static_cast<size_t>(6 * 129);
    size_t sum2 = std::accumulate(img(1).begin(), img(1).end(), 0);
    EXPECT_EQ(expectedSum2, sum2);
        
    size_t expectedSum3 = static_cast<size_t>(58 * 179) + static_cast<size_t>(6 * 156);
    size_t sum3 = std::accumulate(img(2).begin(), img(2).end(), 0);
    EXPECT_EQ(expectedSum3, sum3);

    // Test-2
    img.set_channels(0, 29, 0);

    Rectangle2D<size_t> box2{ Point2D{ 4, 3 },  2, 3 };
    algorithm::block(img, box2, 0, 0, 0);

    expectedSum1 = 0;
    sum1 = std::accumulate(img(0).begin(), img(0).end(), 0);
    EXPECT_EQ(expectedSum1, sum1);

    expectedSum2 = static_cast<size_t>(58 * 29);
    sum2 = std::accumulate(img(1).begin(), img(1).end(), 0);
    EXPECT_EQ(expectedSum2, sum2);

    expectedSum3 = 0;
    sum3 = std::accumulate(img(2).begin(), img(2).end(), 0);
    EXPECT_EQ(expectedSum3, sum3);
}

TEST(AlgorithmTests, image_bars_grayscale) 
{
    jpeg::JpegImg img{ 10, 15, ColorSpace::GrayScale, 1 };
    algorithm::bars(img, 3, (jpeg::data_type)10, 10);
    
    std::array<jpeg::data_type, 15> expected_values{ 10, 10, 10, 20, 20, 20, 30, 30, 30, 40, 40, 40, 50, 50, 50 };
    
    for (size_t i = 0; i < img.height(); i++)
    {
        size_t j = 0;
        for (auto it = img(0).row_begin(i); it != img(0).row_end(i); it++)
            EXPECT_EQ(expected_values[j++], *it);
    }
}

TEST(AlgorithmTests, image_bars_rgb)
{
    jpeg::JpegImg img{ 10, 16, ColorSpace::RGB, 3 };
    algorithm::bars(img, 5, (jpeg::data_type)10, 10, 15, 20);

    std::array<jpeg::data_type, 16> r{ 10, 10, 10, 10, 10, 20, 20, 20, 20, 20, 30, 30, 30, 30, 30, 40 };
    std::array<jpeg::data_type, 16> g{ 15, 15, 15, 15, 15, 25, 25, 25, 25, 25, 35, 35, 35, 35, 35, 45 };
    std::array<jpeg::data_type, 16> b{ 20, 20, 20, 20, 20, 30, 30, 30, 30, 30, 40, 40, 40, 40, 40, 50 };

    for (size_t i = 0; i < img.height(); i++)
    {
        size_t j = 0;
        for (auto it = img(0).row_begin(i); it != img(0).row_end(i); it++)
            EXPECT_EQ(r[j++], *it);

        j = 0;
        for (auto it = img(1).row_begin(i); it != img(1).row_end(i); it++)
            EXPECT_EQ(g[j++], *it);

        j = 0;
        for (auto it = img(2).row_begin(i); it != img(2).row_end(i); it++)
            EXPECT_EQ(b[j++], *it);
    }
}

TEST(AlgorithmTests, histogram_test1) 
{
    auto img1 = Image<std::uint8_t>(5, 10, ColorSpace::GrayScale, 1, 0);
    img1(0)(0) = 25;
    img1(0)(1) = 50;
    img1(0)(2) = 75;
    img1(0)(3) = 100;
    img1(0)(4) = 125;
    img1(0)(5) = 150;
    img1(0)(6) = 175;
    img1(0)(7) = 200;
    img1(0)(8) = 225;
    img1(0)(9) = 250;

    auto histVec = algorithm::get_histogram(img1(0), 10);
    EXPECT_EQ(histVec[0], 41);
    for (size_t i = 1; i < 10; i++)
        EXPECT_EQ(histVec[i], 1);
}

TEST(AlgorithmTests, histogram_test2) 
{
    algorithm::HistogramImageSettings<std::uint8_t, 1> settings;
    settings.bin_width = 30;
    settings.num_bins = 10;
    settings.max_bin_height = 300;
    settings.padding = 10;
    settings.front = Color<std::uint8_t, 1>{ColorSpace::GrayScale, 0 };
    settings.back = Color<std::uint8_t, 1>{ ColorSpace::GrayScale, 255 };

    auto img1 = Image<std::uint8_t>(2, 3, ColorSpace::GrayScale, 1, 0);
    img1(0)(0) = 53;
    img1(0)(1) = 45;
    img1(0)(2) = 125;
    img1(0)(3) = 34;
    img1(0)(4) = 67;
    img1(0)(5) = 2;

    auto histVec = algorithm::get_histogram(img1(0), 10);
    EXPECT_EQ(histVec[0], 1);
    EXPECT_EQ(histVec[1], 2);
    EXPECT_EQ(histVec[2], 2);
    EXPECT_EQ(histVec[3], 0);
    EXPECT_EQ(histVec[4], 1);
    EXPECT_EQ(histVec[5], 0);
    EXPECT_EQ(histVec[6], 0);
    EXPECT_EQ(histVec[7], 0);
    EXPECT_EQ(histVec[8], 0);
    EXPECT_EQ(histVec[9], 0);
    
    auto histImg = algorithm::get_histogram_image(img1(0), settings);

    EXPECT_EQ(histImg.height(), settings.max_bin_height + 2 * settings.padding);
    EXPECT_EQ(histImg.width(), settings.num_bins * settings.bin_width + 2 * settings.padding);

    // Verify that image padding is all back color
    for (size_t i = 0; i < settings.padding; i++)
        for (size_t j = 0; j < histImg.width(); j++)
            EXPECT_EQ(histImg(0)(i, j), settings.back(0));

    for (size_t i = histImg.height() - 1; i > histImg.height() - 1 - settings.padding; i--)
        for (size_t j = 0; j < histImg.width(); j++)
            EXPECT_EQ(histImg(0)(i, j), settings.back(0));

    for (size_t j = 0; j < settings.padding; j++)
        for (size_t i = 0; i < histImg.height(); i++)
            EXPECT_EQ(histImg(0)(i, j), settings.back(0));

    for (size_t j = histImg.width() - 1; j > histImg.width() - 1 - settings.padding; j--)
        for (size_t i = 0; i < histImg.height(); i++)
            EXPECT_EQ(histImg(0)(i, j), settings.back(0));
}