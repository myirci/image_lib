#include "shrink_tests.hpp"
#include "shrink.hpp"
#include "jpeg_adapter.hpp"

using namespace SmpImgLib;

void generate_shrinked_image_from_grayscale()
{
    Image<JpegAdapter::data_type> imgGrayscale;
    JpegAdapter::import_from_jpeg("../data/petit_prince_grayscale.jpg", imgGrayscale);
    Image<JpegAdapter::data_type> imgShrinkedGrayscale = shrink(imgGrayscale, 2);
    JpegAdapter::export_to_jpeg("petit_prince_grayscale_shrinked.jpg", 80, imgShrinkedGrayscale);
}

void generate_shrinked_image_from_rgb()
{
    Image<JpegAdapter::data_type> imgRgb;
    JpegAdapter::import_from_jpeg("../data/petit_prince.jpg", imgRgb);
    Image<JpegAdapter::data_type> imgShrinkedRgb = shrink(imgRgb, 8);
    JpegAdapter::export_to_jpeg("petit_prince_shrinked.jpg", 80, imgShrinkedRgb);
}
