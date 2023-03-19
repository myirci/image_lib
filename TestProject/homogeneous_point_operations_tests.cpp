#include "homogeneous_point_operations_tests.hpp"
#include <imglib/algorithms/homogeneous_point_operations.hpp>
#include <imglib/adaptors/jpeg_adaptor.hpp>
#include "test_config.hpp"

using namespace imglib;

void GenerateIncreasedContrastImages()
{
    std::wstring inImgPath1{ input_img_path };
    inImgPath1 += L"/petit_prince_grayscale.jpg";
    auto img1 = jpeg::Read(inImgPath1);
    algorithm::contrast(img1, 1.5);
    std::wstring outImgPath1{ output_img_path };
    outImgPath1 += L"/petit_prince_grayscale_grayscale_increased_contrast.jpg";
    jpeg::Write(outImgPath1, 70, img1);

    std::wstring inImgPath2{ input_img_path };
    inImgPath2 += L"/petit_prince.jpg";
    auto img2 = jpeg::Read(inImgPath2);
    algorithm::contrast(img2, 1.5);
    std::wstring outImgPath2{ output_img_path };
    outImgPath2 += L"/petit_prince_increased_contrast.jpg";
    jpeg::Write(outImgPath2, 70, img2);
}

void GenerateDecreasedContrastImages()
{
    std::wstring inImgPath1{ input_img_path };
    inImgPath1 += L"/petit_prince_grayscale.jpg";
    auto img1 = jpeg::Read(inImgPath1);
    algorithm::contrast(img1, 0.5);
    std::wstring outImgPath1{ output_img_path };
    outImgPath1 += L"/petit_prince_grayscale_decreased_contrast.jpg";
    jpeg::Write(outImgPath1, 70, img1);

    std::wstring inImgPath2{ input_img_path };
    inImgPath2 += L"/petit_prince.jpg";
    auto img2 = jpeg::Read(inImgPath2);
    algorithm::contrast(img2, 0.5);
    std::wstring outImgPath2{ output_img_path };
    outImgPath2 += L"/petit_prince_decreased_contrast.jpg";
    jpeg::Write(outImgPath2, 70, img2);
}

void GenerateAutoContrastImages()
{
    std::wstring inImgPath1{ input_img_path };
    inImgPath1 += L"/petit_prince_grayscale.jpg";
    auto img1 = jpeg::Read(inImgPath1);
    if (algorithm::auto_contrast(img1)) 
    {
        std::wstring outImgPath1{ output_img_path };
        outImgPath1 += L"/petit_prince_grayscale_grayscale_if_auto_contrasted.jpg";
        jpeg::Write(outImgPath1, 70, img1);
    }

    std::wstring inImgPath2{ input_img_path };
    inImgPath2 += L"/petit_prince.jpg";
    auto img2 = jpeg::Read(inImgPath2);
    if (algorithm::auto_contrast(img2))
    {
        std::wstring outImgPath2{ output_img_path };
        outImgPath2 += L"/petit_prince_if_auto_contrasted.jpg";
        jpeg::Write(outImgPath2, 70, img2);
    }
}

void GenerateModifiedAutoContrastImages()
{
    std::wstring inImgPath1{ input_img_path };
    inImgPath1 += L"/petit_prince_grayscale.jpg";
    auto img1 = jpeg::Read(inImgPath1);
    algorithm::modified_auto_contrast(img1, 0.005, 0.005);
    std::wstring outImgPath1{ output_img_path };
    outImgPath1 += L"/petit_prince_grayscale_grayscale_modified_auto_contrasted.jpg";
    jpeg::Write(outImgPath1, 70, img1);

    std::wstring inImgPath2{ input_img_path };
    inImgPath2 += L"/petit_prince.jpg";
    auto img2 = jpeg::Read(inImgPath2);
    algorithm::modified_auto_contrast(img2, 0.005, 0.005);
    std::wstring outImgPath2{ output_img_path };
    outImgPath2 += L"/petit_prince_modified_auto_contrasted.jpg";
    jpeg::Write(outImgPath2, 70, img2);
}

void GenerateInvertedImages()
{
    std::wstring inImgPath1{ input_img_path };
    inImgPath1 += L"/petit_prince_grayscale.jpg";
    auto img1 = jpeg::Read(inImgPath1);
    algorithm::invert(img1);
    std::wstring outImgPath1{ output_img_path };
    outImgPath1 += L"/petit_prince_grayscale_inverted.jpg";
    jpeg::Write(outImgPath1, 70, img1);

    std::wstring inImgPath2{ input_img_path };
    inImgPath2 += L"/petit_prince.jpg";
    auto img2 = jpeg::Read(inImgPath2);
    algorithm::invert(img2);
    std::wstring outImgPath2{ output_img_path };
    outImgPath2 += L"/petit_prince_inverted.jpg";
    jpeg::Write(outImgPath2, 70, img2);
}
