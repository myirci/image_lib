#include "HomogeneousPointOperationsTests.hpp"
#include <Algorithms/HomogeneousPointOperations.hpp>
#include <Adaptors/JPEGAdaptor.hpp>
#include "TestConfig.hpp"

using namespace imglib;

void generate_increased_contrast_images()
{
    Image<jpeg_adaptor::data_type> img1;
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    jpeg_adaptor::import_from_jpeg(inImgPath1, img1);
    algorithm::contrast(img1, 1.5);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_grayscale_increased_contrast.jpg";
    jpeg_adaptor::export_to_jpeg(outImgPath1, 70, img1);

    Image<jpeg_adaptor::data_type> img2;
    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    jpeg_adaptor::import_from_jpeg(inImgPath2, img2);
    algorithm::contrast(img2, 1.5);
    std::string outImgPath2{ output_img_path };
    outImgPath2 += "/petit_prince_increased_contrast.jpg";
    jpeg_adaptor::export_to_jpeg(outImgPath2, 70, img2);
}

void generate_decreased_contrast_images()
{
    Image<jpeg_adaptor::data_type> img1;
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    jpeg_adaptor::import_from_jpeg(inImgPath1, img1);
    algorithm::contrast(img1, 0.5);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_decreased_contrast.jpg";
    jpeg_adaptor::export_to_jpeg(outImgPath1, 70, img1);

    Image<jpeg_adaptor::data_type> img2;
    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    jpeg_adaptor::import_from_jpeg(inImgPath2, img2);
    algorithm::contrast(img2, 0.5);
    std::string outImgPath2{ output_img_path };
    outImgPath2 += "/petit_prince_decreased_contrast.jpg";
    jpeg_adaptor::export_to_jpeg(outImgPath2, 70, img2);
}

void generate_auto_contrast_images()
{
    Image<jpeg_adaptor::data_type> img1;
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    jpeg_adaptor::import_from_jpeg(inImgPath1, img1);
    if (algorithm::auto_contrast(img1)) 
    {
        std::string outImgPath1{ output_img_path };
        outImgPath1 += "/petit_prince_grayscale_grayscale_if_auto_contrasted.jpg";
        jpeg_adaptor::export_to_jpeg(outImgPath1, 70, img1);
    }

    Image<jpeg_adaptor::data_type> img2;
    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    jpeg_adaptor::import_from_jpeg(inImgPath2, img2);
    if (algorithm::auto_contrast(img2))
    {
        std::string outImgPath2{ output_img_path };
        outImgPath2 += "/petit_prince_if_auto_contrasted.jpg";
        jpeg_adaptor::export_to_jpeg(outImgPath2, 70, img2);
    }
}

void generate_modified_auto_contrast_images()
{
    Image<jpeg_adaptor::data_type> img1;
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    jpeg_adaptor::import_from_jpeg(inImgPath1, img1);
    algorithm::modified_auto_contrast(img1, 0.005, 0.005);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_grayscale_modified_auto_contrasted.jpg";
    jpeg_adaptor::export_to_jpeg(outImgPath1, 70, img1);


    Image<jpeg_adaptor::data_type> img2;
    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    jpeg_adaptor::import_from_jpeg(inImgPath2, img2);
    algorithm::modified_auto_contrast(img2, 0.005, 0.005);
    std::string outImgPath2{ output_img_path };
    outImgPath2 += "/petit_prince_modified_auto_contrasted.jpg";
    jpeg_adaptor::export_to_jpeg(outImgPath2, 70, img2);
}

void generate_inverted_images()
{
    Image<jpeg_adaptor::data_type> img1;
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    jpeg_adaptor::import_from_jpeg(inImgPath1, img1);
    algorithm::invert(img1);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_inverted.jpg";
    jpeg_adaptor::export_to_jpeg(outImgPath1, 70, img1);

    Image<jpeg_adaptor::data_type> img2;
    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    jpeg_adaptor::import_from_jpeg(inImgPath2, img2);
    algorithm::invert(img2);
    std::string outImgPath2{ output_img_path };
    outImgPath2 += "/petit_prince_inverted.jpg";
    jpeg_adaptor::export_to_jpeg(outImgPath2, 70, img2);
}