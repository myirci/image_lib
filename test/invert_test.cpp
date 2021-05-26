#include "invert_test.hpp"
#include "invert.hpp"
#include "jpeg_adapter.hpp"

using namespace SmpImgLib;

void generate_inverted_images()
{
    Image<JpegAdapter::data_type> img1;
    JpegAdapter::import_from_jpeg("../data/petit_prince_grayscale.jpg", img1);
    invert(img1);
    JpegAdapter::export_to_jpeg("petit_prince_grayscale_inverted.jpg", 70, img1);

    Image<JpegAdapter::data_type> img2;
    JpegAdapter::import_from_jpeg("../data/petit_prince.jpg", img2);
    invert(img2);
    JpegAdapter::export_to_jpeg("petit_prince_inverted.jpg", 70, img2);
}