#include "bar_tests.hpp"
#include "bars.hpp"
#include "jpeg_adapter.hpp"

using namespace SmpImgLib;

void generate_bars_in_grayscale()
{
    Image<JpegAdapter::data_type> img(600, 600, ColorSpace::GrayScale, 1);
    color_mono<JpegAdapter::data_type> first_color(0);
    unsigned char increment = 24;
    int width = 64;
    bars(img, first_color, increment, width);
    JpegAdapter::export_to_jpeg("bars_grayscale.jpg", 60, img);
}

void generate_bars_in_rgb()
{
    Image<JpegAdapter::data_type> img(600, 600, ColorSpace::RGB, 3);
    color_rgb<JpegAdapter::data_type> first_color(50, 100, 150);
    unsigned char increment = 20;
    int width = 20;
    bars(img, first_color, increment, width);
    JpegAdapter::export_to_jpeg("bars_rgb.jpg", 60, img);
}