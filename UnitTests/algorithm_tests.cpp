#include "pch.h"
#include "test_helpers.h"

#include <numeric>
#include <array>

#include <imglib/adaptors/jpeg_adaptor.hpp>
#include <imglib/algorithms/image_generation.hpp>
#include <imglib/image/image.hpp>

using namespace imglib;

using JpegImage = Image<jpeg::data_type>;
using Point2D = Point<size_t, 2u>;

TEST(AlgorithmTests, image_block_grayscale)
{
    JpegImage img{ 8, 8, ColorSpace::GrayScale, 1, 50 };

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
    JpegImage img{ 8, 8, ColorSpace::RGB, 3 };
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
    JpegImage img{ 10, 15, ColorSpace::GrayScale, 1 };
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
    JpegImage img{ 10, 16, ColorSpace::RGB, 3 };
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