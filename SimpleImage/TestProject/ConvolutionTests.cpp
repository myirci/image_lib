#include "ConvolutionTests.hpp"
#include <algorithms/Convolution.hpp>
#include <algorithms/HomogeneousPointOperations.hpp>
#include <adaptors/JPEGAdaptor.hpp>
#include "TestConfig.hpp"

using namespace SmpImgLib;

void generate_averaged_images()
{
    // filter_kernel kernel(3, 3, 0.111111111, 1);
    // filter_kernel kernel(5, 5, 0.04, 1);
    Filter kernel(7, 7, 0.020408163, 1);

    Image<JpegAdapter::data_type> img1;

    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";

    JpegAdapter::import_from_jpeg(inImgPath1, img1);
    Image<JpegAdapter::data_type> img1_ = apply_linear_filter(img1, kernel);

    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_averaged.jpg";

    JpegAdapter::export_to_jpeg(outImgPath1, 70, img1_);

    Image<JpegAdapter::data_type> img2;

    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";

    JpegAdapter::import_from_jpeg(inImgPath2, img2);
    Image<JpegAdapter::data_type> img2_ = apply_linear_filter(img2, kernel);

    std::string outImgPath2{ output_img_path };
    outImgPath2 += "/petit_prince_averaged.jpg";

    JpegAdapter::export_to_jpeg(outImgPath2, 70, img2_);
}

void generate_edge_detected_images()
{
    Filter kernelx(1, 3, 0.5);
    kernelx(-1, 0) = -1;
    kernelx(0, 0) = 0;
    kernelx(1, 0) = 1;

    Filter kernely(3, 1, 0.5);
    kernely(0, -1) = -1;
    kernely(0, 0) = 0;
    kernely(0, 1) = 1;

    Image<JpegAdapter::data_type> img1;

    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";
    JpegAdapter::import_from_jpeg(inImgPath1, img1);
    auto img1_ = apply_linear_filter(img1, kernelx);
    Algorithm::contrast(img1_, 1.5);
    Algorithm::invert(img1_);
    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_dx.jpg";
    JpegAdapter::export_to_jpeg(outImgPath1, 70, img1_);
    
    auto img1__ = apply_linear_filter(img1, kernely);
    Algorithm::contrast(img1__, 1.5);
    Algorithm::invert(img1__);
    std::string outImgPath2{ output_img_path };
    outImgPath2 += "/petit_prince_grayscale_dy.jpg";
    JpegAdapter::export_to_jpeg(outImgPath2, 70, img1__);

    Image<JpegAdapter::data_type> img2;
    std::string inImgPath2{ input_img_path };
    inImgPath2 += "/petit_prince.jpg";
    JpegAdapter::import_from_jpeg(inImgPath2, img2);
    auto img2_ = apply_linear_filter(img2, kernelx);
    Algorithm::contrast(img2_, 1.5);
    Algorithm::invert(img2_);

    std::string outImgPath3{ output_img_path };
    outImgPath3 += "/petit_prince_dx.jpg";
    JpegAdapter::export_to_jpeg(outImgPath3, 70, img2_);
    auto img2__ = apply_linear_filter(img2, kernely);
    Algorithm::contrast(img2__, 1.5);
    Algorithm::invert(img2__);
    std::string outImgPath4{ output_img_path };
    outImgPath4 += "/petit_prince_dy.jpg";
    JpegAdapter::export_to_jpeg(outImgPath4, 70, img2__);
}

void generate_gradient_images()
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

    Image<unsigned char> img1;
    std::string inImgPath1{ input_img_path };
    inImgPath1 += "/petit_prince_grayscale.jpg";

    JpegAdapter::import_from_jpeg(inImgPath1, img1);
    auto img2 = apply_linear_filter(img1, grad);
    Algorithm::contrast(img2, 2.5);
    Algorithm::invert(img2);

    std::string outImgPath1{ output_img_path };
    outImgPath1 += "/petit_prince_grayscale_grad.jpg";
    JpegAdapter::export_to_jpeg(outImgPath1, 70, img2);
    
    auto img3 = apply_linear_filter(img1, kernelx);
    Algorithm::contrast(img3, 2.5);
    Algorithm::invert(img3);

    std::string outImgPath2{ output_img_path };
    outImgPath2 += "/petit_prince_grayscale_grad_dx.jpg";
    JpegAdapter::export_to_jpeg(outImgPath2, 70, img3);


    auto img4 = apply_linear_filter(img1, kernely);
    Algorithm::contrast(img4, 2.5);
    Algorithm::invert(img4);
    std::string outImgPath3{ output_img_path };
    outImgPath3 += "/petit_prince_grayscale_grad_dy.jpg";
    JpegAdapter::export_to_jpeg(outImgPath3, 70, img4);
}