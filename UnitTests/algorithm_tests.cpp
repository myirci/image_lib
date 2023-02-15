#include "pch.h"
#include "test_helpers.h"

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

TEST(AlgorithmTests, block_grayscale) 
{
    size_t width{ 600 }, height{ 600 };
    JpegImage img{ height, width, ColorSpace::GrayScale, 1, 50 };

  /*  size_t oneThird{ width / 3 };
    Rectangle2D<size_t> box{ Point2D{ oneThird, oneThird },  oneThird, oneThird };

    algorithm::new_block(img, box, 127);*/

    export_jpeg(img, "block_new_grayscale.jpg");
}

TEST(AlgorithmTests, block_rgb)
{

}