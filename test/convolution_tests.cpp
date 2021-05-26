#include "convolution_tests.hpp"
#include "convolve.hpp"
#include "contrast.hpp"
#include "invert.hpp"
#include "jpeg_adapter.hpp"

using namespace SmpImgLib;

void generate_averaged_images()
{
    // filter_kernel kernel(3, 3, 0.111111111, 1);
    // filter_kernel kernel(5, 5, 0.04, 1);
    filter_kernel kernel(7, 7, 0.020408163, 1);

    Image<JpegAdapter::data_type> img1;
    JpegAdapter::import_from_jpeg("../data/petit_prince_grayscale.jpg", img1);
    Image<JpegAdapter::data_type> img1_;
    apply_linear_filter(img1, kernel, img1_);
    JpegAdapter::export_to_jpeg("petit_prince_grayscale_averaged.jpg", 70, img1_);

    Image<JpegAdapter::data_type> img2;
    JpegAdapter::import_from_jpeg("../data/petit_prince.jpg", img2);
    Image<JpegAdapter::data_type> img2_;
    apply_linear_filter(img2, kernel, img2_);
    JpegAdapter::export_to_jpeg("petit_prince_averaged.jpg", 70, img2_);
}

void generate_edge_detected_images()
{
    filter_kernel kernelx(1, 3, 0.5);
    kernelx(-1, 0) = -1;
    kernelx(0, 0) = 0;
    kernelx(1, 0) = 1;

    filter_kernel kernely(3, 1, 0.5);
    kernely(0, -1) = -1;
    kernely(0, 0) = 0;
    kernely(0, 1) = 1;

    Image<JpegAdapter::data_type> img1, img1_;
    JpegAdapter::import_from_jpeg("../data/petit_prince_grayscale.jpg", img1);
    apply_linear_filter(img1, kernelx, img1_);
    contrast(img1_, 1.5);
    invert(img1_);
    JpegAdapter::export_to_jpeg("petit_prince_grayscale_dx.jpg", 70, img1_);
    apply_linear_filter(img1, kernely, img1_);
    contrast(img1_, 1.5);
    invert(img1_);
    JpegAdapter::export_to_jpeg("petit_prince_grayscale_dy.jpg", 70, img1_);
    
    Image<JpegAdapter::data_type> img2, img2_;
    JpegAdapter::import_from_jpeg("../data/petit_prince.jpg", img2);
    apply_linear_filter(img2, kernelx, img2_);
    contrast(img2_, 1.5);
    invert(img2_);
    JpegAdapter::export_to_jpeg("petit_prince_dx.jpg", 70, img2_);
    apply_linear_filter(img2, kernely, img2_);
    contrast(img2_, 1.5);
    invert(img2_);
    JpegAdapter::export_to_jpeg("petit_prince_dy.jpg", 70, img2_);
}

void generate_gradient_images()
{
    filter_kernel grad(3, 3, 0.5);
    grad(-1, -1) = 0;
    grad(-1, 0) = -1;
    grad(-1, 1) = 0;
    grad(0, -1) = -1;
    grad(0, 0) = 0;
    grad(0, 1) = 1;
    grad(1, -1) = 0;
    grad(1, 0) = 1;
    grad(1, 1) = 0;

    filter_kernel kernelx(1, 3, 0.5);
    kernelx(-1, 0) = -1;
    kernelx(0, 0) = 0;
    kernelx(1, 0) = 1;

    filter_kernel kernely(3, 1, 0.5);
    kernely(0, -1) = -1;
    kernely(0, 0) = 0;
    kernely(0, 1) = 1;

    Image<unsigned char> img1, img1_;
    JpegAdapter::import_from_jpeg("../data/petit_prince_grayscale.jpg", img1);
    apply_linear_filter(img1, grad, img1_);
    contrast(img1_, 2.5);
    invert(img1_);
    JpegAdapter::export_to_jpeg("petit_prince_grayscale_grad.jpg", 70, img1_);
    img1_.clear();
    apply_linear_filter(img1, kernelx, img1_);
    contrast(img1_, 2.5);
    invert(img1_);
    JpegAdapter::export_to_jpeg("petit_prince_grayscale_grad_dx.jpg", 70, img1_);
    apply_linear_filter(img1, kernely, img1_);
    contrast(img1_, 2.5);
    invert(img1_);
    JpegAdapter::export_to_jpeg("petit_prince_grayscale_grad_dy.jpg", 70, img1_);
}