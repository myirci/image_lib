// #include "contrast_test.hpp"
#include "contrast.hpp"
#include "auto_contrast.hpp"
#include "modified_auto_contrast.hpp"
#include "jpeg_adapter.hpp"

using namespace SmpImgLib;

void generate_increased_contrast_images()
{
    Image<JpegAdapter::data_type> img1;
    JpegAdapter::import_from_jpeg("../data/petit_prince_grayscale.jpg", img1);
    contrast(img1, 1.5);
    JpegAdapter::export_to_jpeg("petit_prince_grayscale_grayscale_increased_contrast.jpg", 70, img1);

    Image<JpegAdapter::data_type> img2;
    JpegAdapter::import_from_jpeg("../data/petit_prince.jpg", img2);
    contrast(img2, 1.5);
    JpegAdapter::export_to_jpeg("petit_prince_increased_contrast.jpg", 70, img2);
}

void generate_decreased_contrast_images()
{
    Image<JpegAdapter::data_type> img1;
    JpegAdapter::import_from_jpeg("../data/petit_prince_grayscale.jpg", img1);
    contrast(img1, 0.5);
    JpegAdapter::export_to_jpeg("petit_prince_grayscale_decreased_contrast.jpg", 70, img1);

    Image<JpegAdapter::data_type> img2;
    JpegAdapter::import_from_jpeg("../data/petit_prince.jpg", img2);
    contrast(img2, 0.5);
    JpegAdapter::export_to_jpeg("petit_prince_decreased_contrast.jpg", 70, img2);
}

void generate_auto_contrast_images()
{
    Image<JpegAdapter::data_type> img1;
    JpegAdapter::import_from_jpeg("../data/petit_prince_grayscale.jpg", img1);
    if (auto_contrast(img1))
        JpegAdapter::export_to_jpeg("petit_prince_grayscale_grayscale_if_auto_contrasted.jpg", 70, img1); 

    Image<JpegAdapter::data_type> img2;
    JpegAdapter::import_from_jpeg("../data/petit_prince.jpg", img2);
    if (auto_contrast(img2))
        JpegAdapter::export_to_jpeg("petit_prince_if_auto_contrasted.jpg", 70, img2);   
}

void generate_modified_auto_contrast_images()
{
    Image<JpegAdapter::data_type> img1;
    JpegAdapter::import_from_jpeg("../data/petit_prince_grayscale.jpg", img1);
    modified_auto_contrast(img1, 0.005, 0.005);
    JpegAdapter::export_to_jpeg("petit_prince_grayscale_grayscale_modified_auto_contrasted.jpg", 70, img1);

    Image<JpegAdapter::data_type> img2;
    JpegAdapter::import_from_jpeg("../data/petit_prince.jpg", img2);
    modified_auto_contrast(img2, 0.005, 0.005);
    JpegAdapter::export_to_jpeg("petit_prince_modified_auto_contrasted.jpg", 70, img2);
}
