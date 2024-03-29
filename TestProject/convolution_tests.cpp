#include "convolution_tests.hpp"
#include <imglib/algorithms/convolution.hpp>
#include <imglib/algorithms/homogeneous_point_operations.hpp>
#include <imglib/adaptors/jpeg_adaptor.hpp>
#include "test_config.hpp"

using namespace imglib;

void GenerateAveragedImages()
{
    // filter_kernel kernel(3, 3, 0.111111111, 1);
    // filter_kernel kernel(5, 5, 0.04, 1);
    Filter kernel(7, 7, 0.020408163, 1);

    std::wstring inImgPath1{ input_img_path };
    inImgPath1 += L"/petit_prince_grayscale.jpg";

    auto img1 = jpeg::Read(inImgPath1);

    Image<jpeg::data_type> img1_ = apply_linear_filter(img1, kernel);
    
    std::wstring outImgPath1{ output_img_path };
    outImgPath1 += L"/petit_prince_grayscale_averaged.jpg";

    jpeg::Write(outImgPath1, 70, img1_);

   
    std::wstring inImgPath2{ input_img_path };
    inImgPath2 += L"/petit_prince.jpg";

    auto img2 = jpeg::Read(inImgPath2);
    Image<jpeg::data_type> img2_ = apply_linear_filter(img2, kernel);

    std::wstring outImgPath2{ output_img_path };
    outImgPath2 += L"/petit_prince_averaged.jpg";

    jpeg::Write(outImgPath2, 70, img2_);
}

void GenerateEdgeDetectedImages()
{
    Filter kernelx(1, 3, 0.5);
    kernelx(-1, 0) = -1;
    kernelx(0, 0) = 0;
    kernelx(1, 0) = 1;

    Filter kernely(3, 1, 0.5);
    kernely(0, -1) = -1;
    kernely(0, 0) = 0;
    kernely(0, 1) = 1;

    std::wstring inImgPath1{ input_img_path };
    inImgPath1 += L"/petit_prince_grayscale.jpg";
    auto img1 = jpeg::Read(inImgPath1);
    auto img1_ = apply_linear_filter(img1, kernelx);
    algorithm::contrast(img1_, 1.5);
    algorithm::invert(img1_);
    std::wstring outImgPath1{ output_img_path };
    outImgPath1 += L"/petit_prince_grayscale_dx.jpg";
    jpeg::Write(outImgPath1, 70, img1_);
    
    auto img1__ = apply_linear_filter(img1, kernely);
    algorithm::contrast(img1__, 1.5);
    algorithm::invert(img1__);
    std::wstring outImgPath2{ output_img_path };
    outImgPath2 += L"/petit_prince_grayscale_dy.jpg";
    jpeg::Write(outImgPath2, 70, img1__);

    std::wstring inImgPath2{ input_img_path };
    inImgPath2 += L"/petit_prince.jpg";
    auto img2 = jpeg::Read(inImgPath2);
    auto img2_ = apply_linear_filter(img2, kernelx);
    algorithm::contrast(img2_, 1.5);
    algorithm::invert(img2_);

    std::wstring outImgPath3{ output_img_path };
    outImgPath3 += L"/petit_prince_dx.jpg";
    jpeg::Write(outImgPath3, 70, img2_);
    auto img2__ = apply_linear_filter(img2, kernely);
    algorithm::contrast(img2__, 1.5);
    algorithm::invert(img2__);
    std::wstring outImgPath4{ output_img_path };
    outImgPath4 += L"/petit_prince_dy.jpg";
    jpeg::Write(outImgPath4, 70, img2__);
}

void GenerateGradientImages()
{
    Filter grad(3, 3, 0.5);
    grad(-1, -1) = 0;
    grad(-1, 0) = -1;
    grad(-1, 1) = 0;
    grad(0, -1) = -1;
    grad(0, 0) = 0;
    grad(0, 1) = 1;
    grad(1, -1) = 0;
    grad(1, 0) = 1;
    grad(1, 1) = 0;

    Filter kernelx(1, 3, 0.5);
    kernelx(-1, 0) = -1;
    kernelx(0, 0) = 0;
    kernelx(1, 0) = 1;

    Filter kernely(3, 1, 0.5);
    kernely(0, -1) = -1;
    kernely(0, 0) = 0;
    kernely(0, 1) = 1;

    std::wstring inImgPath1{ input_img_path };
    inImgPath1 += L"/petit_prince_grayscale.jpg";

    auto img1 = jpeg::Read(inImgPath1);
    auto img2 = apply_linear_filter(img1, grad);
    algorithm::contrast(img2, 2.5);
    algorithm::invert(img2);

    std::wstring outImgPath1{ output_img_path };
    outImgPath1 += L"/petit_prince_grayscale_grad.jpg";
    jpeg::Write(outImgPath1, 70, img2);
    
    auto img3 = apply_linear_filter(img1, kernelx);
    algorithm::contrast(img3, 2.5);
    algorithm::invert(img3);

    std::wstring outImgPath2{ output_img_path };
    outImgPath2 += L"/petit_prince_grayscale_grad_dx.jpg";
    jpeg::Write(outImgPath2, 70, img3);


    auto img4 = apply_linear_filter(img1, kernely);
    algorithm::contrast(img4, 2.5);
    algorithm::invert(img4);
    std::wstring outImgPath3{ output_img_path };
    outImgPath3 += L"/petit_prince_grayscale_grad_dy.jpg";
    jpeg::Write(outImgPath3, 70, img4);
}