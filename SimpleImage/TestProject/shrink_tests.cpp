#include "shrink_tests.hpp"
#include <algorithms/shrink.hpp>
#include <adaptors/jpeg_adapter.hpp>
#include "test_config.hpp"

using namespace SmpImgLib;

void generate_shrinked_image_from_grayscale()
{
    Image<JpegAdapter::data_type> imgGrayscale;

    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    JpegAdapter::import_from_jpeg(inImgPath1, imgGrayscale);
    Image<JpegAdapter::data_type> imgShrinkedGrayscale = Algorithm::shrink(imgGrayscale, 2);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_shrinked.jpg";
    JpegAdapter::export_to_jpeg(outImgPath1, 80, imgShrinkedGrayscale);
}

void generate_shrinked_image_from_rgb()
{
    Image<JpegAdapter::data_type> imgRgb;
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince.jpg";
    JpegAdapter::import_from_jpeg(inImgPath1, imgRgb);
    Image<JpegAdapter::data_type> imgShrinkedRgb = Algorithm::shrink(imgRgb, 8);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_shrinked.jpg";
    JpegAdapter::export_to_jpeg(outImgPath1, 80, imgShrinkedRgb);
}
