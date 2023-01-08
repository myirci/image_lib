#include "homogeneous_point_operations_tests.hpp"
#include <algorithms/homogeneous_point_operations.hpp>
#include <adaptors/jpeg_adapter.hpp>
#include "test_config.hpp"

using namespace SmpImgLib;

void generate_increased_contrast_images()
{
    Image<JpegAdapter::data_type> img1;
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    JpegAdapter::import_from_jpeg(inImgPath1, img1);
    Algorithm::contrast(img1, 1.5);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_grayscale_increased_contrast.jpg";
    JpegAdapter::export_to_jpeg(outImgPath1, 70, img1);

    Image<JpegAdapter::data_type> img2;
    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    JpegAdapter::import_from_jpeg(inImgPath2, img2);
    Algorithm::contrast(img2, 1.5);
    std::string outImgPath2{ output_img_path };
    outImgPath2 += "/petit_prince_increased_contrast.jpg";
    JpegAdapter::export_to_jpeg(outImgPath2, 70, img2);
}

void generate_decreased_contrast_images()
{
    Image<JpegAdapter::data_type> img1;
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    JpegAdapter::import_from_jpeg(inImgPath1, img1);
    Algorithm::contrast(img1, 0.5);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_decreased_contrast.jpg";
    JpegAdapter::export_to_jpeg(outImgPath1, 70, img1);

    Image<JpegAdapter::data_type> img2;
    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    JpegAdapter::import_from_jpeg(inImgPath2, img2);
    Algorithm::contrast(img2, 0.5);
    std::string outImgPath2{ output_img_path };
    outImgPath2 += "/petit_prince_decreased_contrast.jpg";
    JpegAdapter::export_to_jpeg(outImgPath2, 70, img2);
}

void generate_auto_contrast_images()
{
    Image<JpegAdapter::data_type> img1;
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    JpegAdapter::import_from_jpeg(inImgPath1, img1);
    if (Algorithm::auto_contrast(img1)) 
    {
        std::string outImgPath1{ output_img_path };
        outImgPath1 += "/petit_prince_grayscale_grayscale_if_auto_contrasted.jpg";
        JpegAdapter::export_to_jpeg(outImgPath1, 70, img1);
    }

    Image<JpegAdapter::data_type> img2;
    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    JpegAdapter::import_from_jpeg(inImgPath2, img2);
    if (Algorithm::auto_contrast(img2))
    {
        std::string outImgPath2{ output_img_path };
        outImgPath2 += "/petit_prince_if_auto_contrasted.jpg";
        JpegAdapter::export_to_jpeg(outImgPath2, 70, img2);
    }
}

void generate_modified_auto_contrast_images()
{
    Image<JpegAdapter::data_type> img1;
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    JpegAdapter::import_from_jpeg(inImgPath1, img1);
    Algorithm::modified_auto_contrast(img1, 0.005, 0.005);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_grayscale_modified_auto_contrasted.jpg";
    JpegAdapter::export_to_jpeg(outImgPath1, 70, img1);


    Image<JpegAdapter::data_type> img2;
    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    JpegAdapter::import_from_jpeg(inImgPath2, img2);
    Algorithm::modified_auto_contrast(img2, 0.005, 0.005);
    std::string outImgPath2{ output_img_path };
    outImgPath2 += "/petit_prince_modified_auto_contrasted.jpg";
    JpegAdapter::export_to_jpeg(outImgPath2, 70, img2);
}

void generate_inverted_images()
{
    Image<JpegAdapter::data_type> img1;
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    JpegAdapter::import_from_jpeg(inImgPath1, img1);
    Algorithm::invert(img1);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_inverted.jpg";
    JpegAdapter::export_to_jpeg(outImgPath1, 70, img1);

    Image<JpegAdapter::data_type> img2;
    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    JpegAdapter::import_from_jpeg(inImgPath2, img2);
    Algorithm::invert(img2);
    std::string outImgPath2{ output_img_path };
    outImgPath2 += "/petit_prince_inverted.jpg";
    JpegAdapter::export_to_jpeg(outImgPath2, 70, img2);
}