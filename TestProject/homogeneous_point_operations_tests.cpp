#include "homogeneous_point_operations_tests.hpp"
#include <imglib/algorithms/homogeneous_point_operations.hpp>
#include <imglib/adaptors/jpeg_adaptor.hpp>
#include "test_config.hpp"

using namespace imglib;

void GenerateIncreasedContrastImages()
{
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    auto img1 = jpeg::Import(inImgPath1);
    algorithm::contrast(img1, 1.5);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_grayscale_increased_contrast.jpg";
    jpeg::Export(outImgPath1, 70, img1);

    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    auto img2 = jpeg::Import(inImgPath2);
    algorithm::contrast(img2, 1.5);
    std::string outImgPath2{ output_img_path };
    outImgPath2 += "/petit_prince_increased_contrast.jpg";
    jpeg::Export(outImgPath2, 70, img2);
}

void GenerateDecreasedContrastImages()
{
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    auto img1 = jpeg::Import(inImgPath1);
    algorithm::contrast(img1, 0.5);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_decreased_contrast.jpg";
    jpeg::Export(outImgPath1, 70, img1);

    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    auto img2 = jpeg::Import(inImgPath2);
    algorithm::contrast(img2, 0.5);
    std::string outImgPath2{ output_img_path };
    outImgPath2 += "/petit_prince_decreased_contrast.jpg";
    jpeg::Export(outImgPath2, 70, img2);
}

void GenerateAutoContrastImages()
{
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    auto img1 = jpeg::Import(inImgPath1);
    if (algorithm::auto_contrast(img1)) 
    {
        std::string outImgPath1{ output_img_path };
        outImgPath1 += "/petit_prince_grayscale_grayscale_if_auto_contrasted.jpg";
        jpeg::Export(outImgPath1, 70, img1);
    }

    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    auto img2 = jpeg::Import(inImgPath2);
    if (algorithm::auto_contrast(img2))
    {
        std::string outImgPath2{ output_img_path };
        outImgPath2 += "/petit_prince_if_auto_contrasted.jpg";
        jpeg::Export(outImgPath2, 70, img2);
    }
}

void GenerateModifiedAutoContrastImages()
{
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    auto img1 = jpeg::Import(inImgPath1);
    algorithm::modified_auto_contrast(img1, 0.005, 0.005);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_grayscale_modified_auto_contrasted.jpg";
    jpeg::Export(outImgPath1, 70, img1);

    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    auto img2 = jpeg::Import(inImgPath2);
    algorithm::modified_auto_contrast(img2, 0.005, 0.005);
    std::string outImgPath2{ output_img_path };
    outImgPath2 += "/petit_prince_modified_auto_contrasted.jpg";
    jpeg::Export(outImgPath2, 70, img2);
}

void GenerateInvertedImages()
{
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    auto img1 = jpeg::Import(inImgPath1);
    algorithm::invert(img1);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_inverted.jpg";
    jpeg::Export(outImgPath1, 70, img1);

    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    auto img2 = jpeg::Import(inImgPath2);
    algorithm::invert(img2);
    std::string outImgPath2{ output_img_path };
    outImgPath2 += "/petit_prince_inverted.jpg";
    jpeg::Export(outImgPath2, 70, img2);
}
