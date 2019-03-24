#include "Image.hpp"
#include "Algorithms/Block.hpp"
#include "Algorithms/Bars.hpp"
#include "Algorithms/Convert.hpp"
#include "Algorithms/Shrink.hpp"
#include "Algorithms/Contrast.hpp"
#include "Algorithms/Invert.hpp"
#include "Algorithms/AutoContrast.hpp"
#include "Algorithms/ModifiedAutoContrast.hpp"
#include "Algorithms/Convolve.hpp"

void generate_blocks_in_grayscale();
void generate_blocks_in_rgb();
void generate_bars_in_grayscale();
void generate_bars_in_rgb();
void generate_grayscale_from_rgb_image();
void generate_shrinked_image_from_grayscale();
void generate_shrinked_image_from_rgb();
void generate_increased_contrast_images();
void generate_decreased_contrast_images();
void generate_inverted_images();
void generate_auto_contrast_images();
void generate_modified_auto_contrast_images();
void test_apply_filter();
void generate_edge_detected_images();
void generate_averaged_images();
void generate_gradient_images();

int main() {

    generate_blocks_in_rgb();
    generate_blocks_in_grayscale();

    generate_bars_in_grayscale();
    generate_bars_in_rgb();

    // generate_grayscale_from_rgb_image();

    // generate_shrinked_image_from_grayscale();
    // generate_shrinked_image_from_rgb();

    // generate_increased_contrast_images();
    // generate_decreased_contrast_images();

    // generate_inverted_images();

    // generate_auto_contrast_images();

    // generate_modified_auto_contrast_images();

    // test_apply_filter();

    // generate_averaged_images();

    // generate_edge_detected_images();

    // generate_gradient_images();

    return 0;
}

void generate_blocks_in_grayscale() {
    int width_third, top, bottom, left, right;

    Image<JSAMPLE> img1(600, 600, GRAYSCALE, 1, 100);
    color_mono<JSAMPLE> inside_box_color(127);
    width_third = img1.num_rows()/3;
    top = width_third;
    bottom = img1.num_rows() - width_third;
    left = width_third;
    right = img1.num_columns() - width_third;
    block(img1, top, left, bottom, right, inside_box_color);
    img1.save_jpeg("13_4_a.jpg",60);

    Image<JSAMPLE> img2(600, 600, GRAYSCALE, 1, 154);
    block(img2, top, left, bottom, right, inside_box_color);
    img2.save_jpeg("13_4_b.jpg",60);
}

void generate_blocks_in_rgb() {
    int width_third, top, bottom, left, right;

    Image<JSAMPLE> img1(600, 600, RGB, 3);
    color_rgb<JSAMPLE> img1_color(100,0,0);
    set_color(img1, img1_color);
    color_rgb<JSAMPLE> inside_box_color(127,0,0);
    width_third = img1.num_rows()/3;
    top = width_third;
    bottom = img1.num_rows() - width_third;
    left = width_third;
    right = img1.num_columns() - width_third;
    block(img1, top, left, bottom, right, inside_box_color);
    img1.save_jpeg("13_4_a_rgb.jpg",60);

    Image<JSAMPLE> img2(600, 600, RGB, 3);
    color_rgb<JSAMPLE> img2_color(154,0,0);
    set_color(img2, img2_color);
    block(img2, top, left, bottom, right, inside_box_color);
    img2.save_jpeg("13_4_b_rgb.jpg",60);
}

void generate_bars_in_grayscale() {
    Image<JSAMPLE> img(600, 600, GRAYSCALE, 1);
    color_mono<JSAMPLE> first_color(0);
    JSAMPLE increment = 24;
    int width = 64;
    bars(img, first_color, increment, width);
    img.save_jpeg("13_5.jpg",60);
}

void generate_bars_in_rgb() {
    Image<JSAMPLE> img(600, 600, RGB, 3);
    color_rgb<JSAMPLE> first_color(50,100,150);
    JSAMPLE increment = 20;
    int width = 20;
    bars(img, first_color, increment, width);
    img.save_jpeg("13_5_rgb.jpg",60);
}

void generate_grayscale_from_rgb_image() {
    Image<JSAMPLE> img1_rgb("img1_rgb.jpg");
    Image<JSAMPLE> img1_grayscale(184, 274, GRAYSCALE, 1);
    convert_rgb_to_grayscale(img1_rgb, img1_grayscale);
    img1_grayscale.save_jpeg("img1_grayscale.jpg", 90);

    Image<JSAMPLE> img2_rgb("img2_rgb.jpg");
    Image<JSAMPLE> img2_grayscale(275, 183, GRAYSCALE, 1);
    convert_rgb_to_grayscale(img2_rgb, img2_grayscale);
    img2_grayscale.save_jpeg("img2_grayscale.jpg", 90);

    Image<JSAMPLE> img3_rgb("earth_rgb.jpg");
    Image<JSAMPLE> img3_grayscale(2048, 2048, GRAYSCALE, 1);
    convert_rgb_to_grayscale(img3_rgb, img3_grayscale);
    img3_grayscale.save_jpeg("earth_grayscale.jpg", 80);
}

void generate_shrinked_image_from_grayscale() {
    Image<JSAMPLE> img1_grayscale("img1_grayscale.jpg");
    Image<JSAMPLE> img1_grayscale_shrink;
    shrink(img1_grayscale, 2, img1_grayscale_shrink);
    img1_grayscale_shrink.save_jpeg("img1_grayscale_shrink.jpg", 80);

    Image<JSAMPLE> img2_grayscale("img2_grayscale.jpg");
    Image<JSAMPLE> img2_grayscale_shrink;
    shrink(img2_grayscale, 2, img2_grayscale_shrink);
    img2_grayscale_shrink.save_jpeg("img2_grayscale_shrink.jpg", 80);

    Image<JSAMPLE> earth_grayscale("earth_grayscale.jpg");
    Image<JSAMPLE> earth_grayscale_shrink;
    shrink(earth_grayscale, 4, earth_grayscale_shrink);
    earth_grayscale_shrink.save_jpeg("earth_grayscale_shrink.jpg", 80);
}

void generate_shrinked_image_from_rgb() {
    Image<JSAMPLE> img1_rgb("img1_rgb.jpg");
    Image<JSAMPLE> img1_rgb_shrink;
    shrink(img1_rgb, 16, img1_rgb_shrink);
    img1_rgb_shrink.save_jpeg("img1_rgb_shrink.jpg", 80);

    Image<JSAMPLE> img2_rgb("img2_rgb.jpg");
    Image<JSAMPLE> img2_rgb_shrink;
    shrink(img2_rgb, 16, img2_rgb_shrink);
    img2_rgb_shrink.save_jpeg("img2_rgb_shrink.jpg", 80);

    Image<JSAMPLE> earth_rgb("earth_rgb.jpg");
    Image<JSAMPLE> earth_rgb_shrink;
    shrink(earth_rgb, 32, earth_rgb_shrink);
    earth_rgb_shrink.save_jpeg("earth_rgb_shrink.jpg", 80);
}

void generate_increased_contrast_images() {
    Image<JSAMPLE> img1("img1_grayscale.jpg");
    contrast(img1, 1.5);
    img1.save_jpeg("img1_grayscale_increased_contrast.jpg", 70);

    Image<JSAMPLE> img2("img2_grayscale.jpg");
    contrast(img2, 1.5);
    img2.save_jpeg("img2_grayscale_increased_contrast.jpg", 70);

    Image<JSAMPLE> img3("earth_grayscale.jpg");
    contrast(img3, 1.5);
    img3.save_jpeg("earth_grayscale_increased_contrast.jpg", 70);

    Image<JSAMPLE> img4("img1_rgb.jpg");
    contrast(img4, 1.5);
    img4.save_jpeg("img1_rgb_increased_contrast.jpg", 70);

    Image<JSAMPLE> img5("img2_rgb.jpg");
    contrast(img5, 1.5);
    img5.save_jpeg("img2_rgb_increased_contrast.jpg", 70);

    Image<JSAMPLE> img6("earth_rgb.jpg");
    contrast(img6, 1.5);
    img6.save_jpeg("earth_rgb_increased_contrast.jpg", 70);
}

void generate_decreased_contrast_images() {
    Image<JSAMPLE> img1("img1_grayscale.jpg");
    contrast(img1, 0.5);
    img1.save_jpeg("img1_grayscale_decreased_contrast.jpg", 70);

    Image<JSAMPLE> img2("img2_grayscale.jpg");
    contrast(img2, 0.5);
    img2.save_jpeg("img2_grayscale_decreased_contrast.jpg", 70);

    Image<JSAMPLE> img3("earth_grayscale.jpg");
    contrast(img3, 0.5);
    img3.save_jpeg("earth_grayscale_decreased_contrast.jpg", 70);

    Image<JSAMPLE> img4("img1_rgb.jpg");
    contrast(img4, 0.5);
    img4.save_jpeg("img1_rgb_decreased_contrast.jpg", 70);

    Image<JSAMPLE> img5("img2_rgb.jpg");
    contrast(img5, 0.5);
    img5.save_jpeg("img2_rgb_decreased_contrast.jpg", 70);

    Image<JSAMPLE> img6("earth_rgb.jpg");
    contrast(img6, 0.5);
    img6.save_jpeg("earth_rgb_decreased_contrast.jpg", 70);
}

void generate_inverted_images() {
    Image<JSAMPLE> img1("img1_grayscale.jpg");
    invert(img1);
    img1.save_jpeg("img1_grayscale_inverted.jpg", 70);

    Image<JSAMPLE> img2("img2_grayscale.jpg");
    invert(img2);
    img2.save_jpeg("img2_grayscale_inverted.jpg", 70);

    Image<JSAMPLE> img3("earth_grayscale.jpg");
    invert(img3);
    img3.save_jpeg("earth_grayscale_inverted.jpg", 70);

    Image<JSAMPLE> img4("img1_rgb.jpg");
    invert(img4);
    img4.save_jpeg("img1_rgb_inverted.jpg", 70);

    Image<JSAMPLE> img5("img2_rgb.jpg");
    invert(img5);
    img5.save_jpeg("img2_rgb_inverted.jpg", 70);

    Image<JSAMPLE> img6("earth_rgb.jpg");
    invert(img6);
    img6.save_jpeg("earth_rgb_inverted.jpg", 70);
}

void generate_auto_contrast_images() {
    Image<JSAMPLE> img1("img1_grayscale.jpg");
    if(auto_contrast(img1)) {
        img1.save_jpeg("img1_grayscale_if(auto_contrasted.jpg", 70);
    }

    Image<JSAMPLE> img2("img2_grayscale.jpg");
    if(auto_contrast(img2)) {
        img2.save_jpeg("img2_grayscale_if(auto_contrasted.jpg", 70);
    }

    Image<JSAMPLE> img3("earth_grayscale.jpg");
    if(auto_contrast(img3)) {
        img3.save_jpeg("earth_grayscale_if(auto_contrasted.jpg", 70);
    }

    Image<JSAMPLE> img4("img1_rgb.jpg");
    if(auto_contrast(img4)) {
        img4.save_jpeg("img1_rgb_if(auto_contrasted.jpg", 70);
    }

    Image<JSAMPLE> img5("img2_rgb.jpg");
    if(auto_contrast(img5)) {
        img5.save_jpeg("img2_rgb_if(auto_contrasted.jpg", 70);
    }

    Image<JSAMPLE> img6("earth_rgb.jpg");
    if(auto_contrast(img6)) {
        img6.save_jpeg("earth_rgb_if(auto_contrasted.jpg", 70);
    }
}

void generate_modified_auto_contrast_images() {
    Image<JSAMPLE> img1("img1_grayscale.jpg");
    modified_auto_contrast(img1, 0.005, 0.005);
    img1.save_jpeg("img1_grayscale_modified_auto_contrasted.jpg", 70);

    Image<JSAMPLE> img2("img2_grayscale.jpg");
    modified_auto_contrast(img2, 0.005, 0.005);
    img2.save_jpeg("img2_grayscale_modified_auto_contrasted.jpg", 70);

    Image<JSAMPLE> img3("earth_grayscale.jpg");
    modified_auto_contrast(img3, 0.135, 0.005);
    img3.save_jpeg("earth_grayscale_modified_auto_contrasted.jpg", 70);

    Image<JSAMPLE> img4("img1_rgb.jpg");
    modified_auto_contrast(img4, 0.005, 0.005);
    img4.save_jpeg("img1_rgb_modified_auto_contrasted.jpg", 70);

    Image<JSAMPLE> img5("img2_rgb.jpg");
    modified_auto_contrast(img5, 0.005, 0.005);
    img5.save_jpeg("img2_rgb_modified_auto_contrasted.jpg", 70);

    Image<JSAMPLE> img6("earth_rgb.jpg");
    modified_auto_contrast(img6, 0.135, 0.005);
    img6.save_jpeg("earth_rgb_modified_auto_contrasted.jpg", 70);
}

void test_apply_filter() {
    Image<JSAMPLE> img(6, 5, GRAYSCALE, 1);
    img(0)(0,0) = 7;
    img(0)(0,1) = 4;
    img(0)(0,2) = 10;
    img(0)(0,3) = 0;
    img(0)(0,4) = 1;

    img(0)(1,0) = 3;
    img(0)(1,1) = 25;
    img(0)(1,2) = 26;
    img(0)(1,3) = 20;
    img(0)(1,4) = 0;

    img(0)(2,0) = 9;
    img(0)(2,1) = 5;
    img(0)(2,2) = 8;
    img(0)(2,3) = 7;
    img(0)(2,4) = 1;

    img(0)(3,0) = 10;
    img(0)(3,1) = 8;
    img(0)(3,2) = 6;
    img(0)(3,3) = 4;
    img(0)(3,4) = 3;

    img(0)(4,0) = 5;
    img(0)(4,1) = 6;
    img(0)(4,2) = 5;
    img(0)(4,3) = 6;
    img(0)(4,4) = 2;

    img(0)(5,0) = 19;
    img(0)(5,1) = 10;
    img(0)(5,2) = 5;
    img(0)(5,3) = 5;
    img(0)(5,4) = 0;

    img(0).print_channel();

    /*
    filter_kernel kernel(3,3);
    kernel(-1,1) = 0;
    kernel(0,1) = -1;
    kernel(1,1) = 0;
    kernel(-1,0) = -1;
    kernel(0,0) = 4;
    kernel(1,0) = -1;
    kernel(-1,-1) = 0;
    kernel(0,-1) = -1;
    kernel(1,-1) = 0;
    */


    filter_kernel kernela(3,3, 0.1111111111);
    kernela(-1,1) = 1;
    kernela(0,1) = 1;
    kernela(1,1) = 1;
    kernela(-1,0) = 1;
    kernela(0,0) = 1;
    kernela(1,0) = 1;
    kernela(-1,-1) = 1;
    kernela(0,-1) = 1;
    kernela(1,-1) = 1;
    // kernela.print_kernel();

    filter_kernel kernelx(1, 3, 0.5);
    kernelx(-1,0) = -1;
    kernelx(0,0) = 0;
    kernelx(1,0) = 1;
    // kernelx.print_kernel();

    filter_kernel kernely(3, 1, 0.5);
    kernely(0,-1) = -1;
    kernely(0,0) = 0;
    kernely(0,1) = 1;
    kernely.print_kernel();

    Image<JSAMPLE> img2;
    apply_linear_filter(img, kernely, img2);
    img2(0).print_channel();

}

void generate_averaged_images() {

    // filter_kernel kernel(3, 3, 0.111111111, 1);
    // filter_kernel kernel(5, 5, 0.04, 1);
       filter_kernel kernel(7, 7, 0.020408163, 1);

    Image<JSAMPLE> img1("img1_grayscale.jpg");
    Image<JSAMPLE> img1_;
    apply_linear_filter(img1, kernel, img1_);
    img1_.save_jpeg("img1_grayscale_averaged.jpg", 70);

    Image<JSAMPLE> img2("img2_grayscale.jpg");
    Image<JSAMPLE> img2_;
    apply_linear_filter(img2, kernel, img2_);
    img2_.save_jpeg("img2_grayscale_averaged.jpg", 70);

    Image<JSAMPLE> img3("earth_grayscale.jpg");
    Image<JSAMPLE> img3_;
    apply_linear_filter(img3, kernel, img3_);
    img3_.save_jpeg("earth_grayscale_averaged.jpg", 70);

    Image<JSAMPLE> img4("img1_rgb.jpg");
    Image<JSAMPLE> img4_;
    apply_linear_filter(img4, kernel, img4_);
    img4_.save_jpeg("img1_rgb_averaged.jpg", 70);

    Image<JSAMPLE> img5("img2_rgb.jpg");
    Image<JSAMPLE> img5_;
    apply_linear_filter(img5, kernel, img5_);
    img5_.save_jpeg("img2_rgb_averaged.jpg", 70);

    Image<JSAMPLE> img6("earth_rgb.jpg");
    Image<JSAMPLE> img6_;
    apply_linear_filter(img6, kernel, img6_);
    img6_.save_jpeg("earth_rgb_averaged.jpg", 70);
}

void generate_edge_detected_images() {
    filter_kernel kernelx(1, 3, 0.5);
    kernelx(-1,0) = -1;
    kernelx(0,0) = 0;
    kernelx(1,0) = 1;

    filter_kernel kernely(3, 1, 0.5);
    kernely(0,-1) = -1;
    kernely(0,0) = 0;
    kernely(0,1) = 1;

    Image<JSAMPLE> img1("img1_grayscale.jpg");
    Image<JSAMPLE> img1_;
    apply_linear_filter(img1, kernelx, img1_);
    contrast(img1_, 1.5);
    invert(img1_);
    img1_.save_jpeg("img1_grayscale_dx.jpg", 70);
    apply_linear_filter(img1, kernely, img1_);
    contrast(img1_, 1.5);
    invert(img1_);
    img1_.save_jpeg("img1_grayscale_dy.jpg", 70);
    img1.clear();
    img1_.clear();

    Image<JSAMPLE> img2("img2_grayscale.jpg");
    Image<JSAMPLE> img2_;
    apply_linear_filter(img2, kernelx, img2_);
    contrast(img2_, 1.5);
    invert(img2_);
    img2_.save_jpeg("img2_grayscale_dx.jpg", 70);
    apply_linear_filter(img2, kernely, img2_);
    contrast(img2_, 1.5);
    invert(img2_);
    img2_.save_jpeg("img2_grayscale_dy.jpg", 70);
    img2.clear();
    img2_.clear();

    Image<JSAMPLE> img3("earth_grayscale.jpg");
    Image<JSAMPLE> img3_;
    apply_linear_filter(img3, kernelx, img3_);
    contrast(img3_, 1.5);
    invert(img3_);
    img3_.save_jpeg("earth_grayscale_dx.jpg", 70);
    apply_linear_filter(img3, kernely, img3_);
    contrast(img3_, 1.5);
    invert(img3_);
    img3_.save_jpeg("earth_grayscale_dy.jpg", 70);
    img3.clear();
    img3_.clear();

    Image<JSAMPLE> img4("img1_rgb.jpg");
    Image<JSAMPLE> img4_;
    apply_linear_filter(img4, kernelx, img4_);
    contrast(img4_, 1.5);
    invert(img4_);
    img4_.save_jpeg("img1_rgb_dx.jpg", 70);
    apply_linear_filter(img4, kernely, img4_);
    contrast(img4_, 1.5);
    invert(img4_);
    img4_.save_jpeg("img1_rgb_dy.jpg", 70);
    img4.clear();
    img4_.clear();

    Image<JSAMPLE> img5("img2_rgb.jpg");
    Image<JSAMPLE> img5_;
    apply_linear_filter(img5, kernelx, img5_);
    contrast(img5_, 1.5);
    invert(img5_);
    img5_.save_jpeg("img2_rgb_dx.jpg", 70);
    apply_linear_filter(img5, kernely, img5_);
    contrast(img5_, 1.5);
    invert(img5_);
    img5_.save_jpeg("img2_rgb_dy.jpg", 70);
    img5.clear();
    img5_.clear();

    Image<JSAMPLE> img6("earth_rgb.jpg");
    Image<JSAMPLE> img6_;
    apply_linear_filter(img6, kernelx, img6_);
    contrast(img6_, 1.5);
    invert(img6_);
    img6_.save_jpeg("earth_rgb_dx.jpg", 70);
    apply_linear_filter(img6, kernely, img6_);
    contrast(img6_, 1.5);
    invert(img6_);
    img6_.save_jpeg("earth_rgb_dy.jpg", 70);
    img6.clear();
    img6_.clear();
}

void generate_gradient_images() {

    filter_kernel grad(3, 3, 0.5);
    grad(-1,-1) = 0;
    grad(-1,0) = -1;
    grad(-1,1) = 0;
    grad(0,-1) = -1;
    grad(0,0) = 0;
    grad(0,1) = 1;
    grad(1,-1) = 0;
    grad(1,0) = 1;
    grad(1,1) = 0;

    filter_kernel kernelx(1, 3, 0.5);
    kernelx(-1,0) = -1;
    kernelx(0,0) = 0;
    kernelx(1,0) = 1;

    filter_kernel kernely(3, 1, 0.5);
    kernely(0,-1) = -1;
    kernely(0,0) = 0;
    kernely(0,1) = 1;

    Image<JSAMPLE> img1("im.jpg");
    Image<JSAMPLE> img1_;
    apply_linear_filter(img1, grad, img1_);
    contrast(img1_, 2.5);
    invert(img1_);
    img1_.save_jpeg("im_grad.jpg", 70);
    img1_.clear();

    apply_linear_filter(img1, kernelx, img1_);
    contrast(img1_, 2.5);
    invert(img1_);
    img1_.save_jpeg("im_dx.jpg", 70);
    apply_linear_filter(img1, kernely, img1_);
    contrast(img1_, 2.5);
    invert(img1_);
    img1_.save_jpeg("im_dy.jpg", 70);
    img1.clear();
    img1_.clear();;
}
