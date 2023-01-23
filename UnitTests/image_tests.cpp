#include "pch.h"
#include "test_helpers.h"

#include <imglib/image/image.hpp>

using namespace imglib;

TEST(ImageTests, DefaultConstructor)
{
	EXPECT_NO_THROW(Image<uint8_t>{});
	auto img = Image<uint8_t>{};
	EXPECT_EQ(img.height(), 0);
	EXPECT_EQ(img.width(), 0);
	EXPECT_EQ(img.num_channels(), 0);
	EXPECT_EQ(img.size(), 0);
	EXPECT_EQ(img.data_size(), 0);
	EXPECT_EQ(img.data(), nullptr);
}

TEST(ImageTests, Constructor_test1)
{
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::GrayScale, 0), std::logic_error);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::GrayScale, 2), std::logic_error);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::RGB, 2), std::logic_error);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::RGB, 4), std::logic_error);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::CMYK, 0), std::logic_error);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::CMYK, 5), std::logic_error);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::YCbCr, 2), std::logic_error);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::YCbCr, 4), std::logic_error);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::YCCK, 1), std::logic_error);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::YCCK, 6), std::logic_error);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::Unspecified, 0), std::logic_error);
	
	EXPECT_NO_THROW(Image<uint8_t>(2, 4, ColorSpace::GrayScale, 1));
	EXPECT_NO_THROW(Image<uint8_t>(2, 4, ColorSpace::RGB, 3));
	EXPECT_NO_THROW(Image<uint8_t>(2, 4, ColorSpace::YCbCr, 3));
	EXPECT_NO_THROW(Image<uint8_t>(2, 4, ColorSpace::YCCK, 3));
	EXPECT_NO_THROW(Image<uint8_t>(2, 4, ColorSpace::CMYK, 3));
	EXPECT_NO_THROW(Image<uint8_t>(2, 4, ColorSpace::Unspecified, 15));
}

TEST(ImageTests, Constructor_test2)
{
	auto img1 = Image<char>(4, 3, ColorSpace::GrayScale, 1);
	EXPECT_EQ(img1.height(), 4);
	EXPECT_EQ(img1.width(), 3);
	EXPECT_EQ(img1.color_space(), ColorSpace::GrayScale);
	EXPECT_EQ(img1.num_channels(), 1);
	EXPECT_EQ(img1.size(), 12);
	EXPECT_EQ(img1.data_size(), 12);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(img1(0).data(), img1.size(), 0));

	auto img2 = Image<char>(4, 3, ColorSpace::GrayScale, 1, 12);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(img2(0).data(), img2.size(), 12));
}

TEST(ImageTests, Constructor_test3)
{
	auto img1 = Image<char>(5, 2, ColorSpace::RGB, 3);
	EXPECT_EQ(img1.height(), 5);
	EXPECT_EQ(img1.width(), 2);
	EXPECT_EQ(img1.color_space(), ColorSpace::RGB);
	EXPECT_EQ(img1.num_channels(), 3);
	EXPECT_EQ(img1.size(), 10);
	EXPECT_EQ(img1.data_size(), 30);
	for (size_t i = 0; i < img1.num_channels(); i++)
		EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(img1(i).data(), img1.size(), 0));

	auto img2 = Image<char>(5, 2, ColorSpace::RGB, 3, 19);
	for (size_t i = 0; i < img2.num_channels(); i++)
		EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(img2(i).data(), img2.size(), 19));
}

TEST(ImageTests, CopyConstructor) 
{

}

TEST(ImageTests, MoveConstructor)
{

}

TEST(ImageTests, CopyAssignment)
{

}

TEST(ImageTests, MoveAssignment)
{

}

TEST(ImageTests, Clear)
{

}

TEST(ImageTests, AppendChannel)
{

}

TEST(ImageTests, DeleteChannel)
{

}

TEST(ImageTests, ReplaceChannel)
{

}

TEST(ImageTests, ReorderChannels)
{

}
