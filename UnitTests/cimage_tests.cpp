#include "pch.h"

#include <imglib/image/cimage.hpp>
#include "test_helpers.h"

using namespace imglib;

TEST(CImageTests, DefaultConstructor)
{
	auto img = CImage<uint8_t, 1> {};
	EXPECT_EQ(img.height(), 0);
	EXPECT_EQ(img.width(), 0);
	EXPECT_EQ(img.num_channels(), 1);
	EXPECT_EQ(img.color_space(), ColorSpace::Unspecified);
	EXPECT_EQ(img.size(), 0);
	EXPECT_EQ(img.data_size(), 0);
	EXPECT_EQ(img.data(), nullptr);
}

TEST(CImageTests, Constructor_test1)
{
	EXPECT_THROW((CImage<uint8_t, 1>(0, 10, ColorSpace::GrayScale, 0)), std::invalid_argument);
	EXPECT_THROW((CImage<uint8_t, 1>(10, 0, ColorSpace::GrayScale, 0)), std::invalid_argument);
	EXPECT_THROW((CImage<uint8_t, 2>(10, 10, ColorSpace::GrayScale, 0)), std::invalid_argument);
	EXPECT_THROW((CImage<uint8_t, 2>(10, 10, ColorSpace::RGB, 0)), std::invalid_argument);
	EXPECT_THROW((CImage<uint8_t, 4>(10, 10, ColorSpace::RGB, 0)), std::invalid_argument);
	EXPECT_THROW((CImage<uint8_t, 2>(10, 10, ColorSpace::CMYK, 0)), std::invalid_argument);
	EXPECT_THROW((CImage<uint8_t, 4>(10, 10, ColorSpace::CMYK, 0)), std::invalid_argument);
	EXPECT_THROW((CImage<uint8_t, 2>(10, 10, ColorSpace::YCbCr, 0)), std::invalid_argument);
	EXPECT_THROW((CImage<uint8_t, 4>(10, 10, ColorSpace::YCbCr, 0)), std::invalid_argument);
	EXPECT_THROW((CImage<uint8_t, 2>(10, 10, ColorSpace::YCCK, 0)), std::invalid_argument);
	EXPECT_THROW((CImage<uint8_t, 4>(10, 10, ColorSpace::YCCK, 0)), std::invalid_argument);

	EXPECT_NO_THROW((CImage<uint8_t, 1>(10, 10, ColorSpace::GrayScale, 0)));
	EXPECT_NO_THROW((CImage<uint8_t, 3>(10, 10, ColorSpace::RGB, 0)));
	EXPECT_NO_THROW((CImage<uint8_t, 3>(10, 10, ColorSpace::CMYK, 0)));
	EXPECT_NO_THROW((CImage<uint8_t, 3>(10, 10, ColorSpace::YCbCr, 0)));
	EXPECT_NO_THROW((CImage<uint8_t, 3>(10, 10, ColorSpace::YCCK, 0)));
	EXPECT_NO_THROW((CImage<uint8_t, 5>(10, 10, ColorSpace::Unspecified, 0)));
}

TEST(CImageTests, Constructor_test2)
{
	auto img1 = CImage<uint8_t, 1>{ 5, 10, ColorSpace::GrayScale, 0 };
	EXPECT_EQ(img1.height(), 5);
	EXPECT_EQ(img1.width(), 10);
	EXPECT_EQ(img1.num_channels(), 1);
	EXPECT_EQ(img1.color_space(), ColorSpace::GrayScale);
	EXPECT_EQ(img1.size(), 50);
	EXPECT_EQ(img1.data_size(), 50);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img1.data(), img1.data_size(), 0));

	auto img2 = CImage<int, 1>{ 3, 4, ColorSpace::GrayScale, -3 };
	EXPECT_EQ(img2.height(), 3);
	EXPECT_EQ(img2.width(), 4);
	EXPECT_EQ(img2.num_channels(), 1);
	EXPECT_EQ(img2.color_space(), ColorSpace::GrayScale);
	EXPECT_EQ(img2.size(), 12);
	EXPECT_EQ(img2.data_size(), 12);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img2.data(), img2.data_size(), -3));

	auto img3 = CImage<float, 3>{ 6, 4, ColorSpace::RGB, 0.5f };
	EXPECT_EQ(img3.height(), 6);
	EXPECT_EQ(img3.width(), 4);
	EXPECT_EQ(img3.num_channels(), 3);
	EXPECT_EQ(img3.color_space(), ColorSpace::RGB);
	EXPECT_EQ(img3.size(), 24);
	EXPECT_EQ(img3.data_size(), 72);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(img3.data(), img3.data_size(), 0.5f));

	auto img4 = CImage<char, 5>{ 3, 3, ColorSpace::Unspecified, 7 };
	EXPECT_EQ(img4.height(), 3);
	EXPECT_EQ(img4.width(), 3);
	EXPECT_EQ(img4.num_channels(), 5);
	EXPECT_EQ(img4.color_space(), ColorSpace::Unspecified);
	EXPECT_EQ(img4.size(), 9);
	EXPECT_EQ(img4.data_size(), 45);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(img4.data(), img4.data_size(), 7));
}

TEST(CImageTests, CopyConstructor_and_CopyAssignment)
{
	auto img1 = CImage<char, 3>(5, 2, ColorSpace::RGB, 23);
	auto img2 = CImage<char, 3>(7, 4, ColorSpace::RGB, 29);

	// Copy constructor
	CImage<char,3> img3 = img1;
	EXPECT_EQ(img3.height(), 5);
	EXPECT_EQ(img3.width(), 2);
	EXPECT_EQ(img3.size(), 10);
	EXPECT_EQ(img3.data_size(), 30);
	EXPECT_EQ(img3.num_channels(), 3);
	EXPECT_EQ(img3.color_space(), ColorSpace::RGB);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(img3.data(), img3.data_size(), 23));

	// Copy assignment
	img3 = img2;
	EXPECT_EQ(img3.height(), 7);
	EXPECT_EQ(img3.width(), 4);
	EXPECT_EQ(img3.size(), 28);
	EXPECT_EQ(img3.data_size(), 84);
	EXPECT_EQ(img3.num_channels(), 3);
	EXPECT_EQ(img3.color_space(), ColorSpace::RGB);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(img3.data(), img3.data_size(), 29));

	// Self copy assignment
	img3 = img3;
	EXPECT_EQ(img3.height(), 7);
	EXPECT_EQ(img3.width(), 4);
	EXPECT_EQ(img3.size(), 28);
	EXPECT_EQ(img3.data_size(), 84);
	EXPECT_EQ(img3.num_channels(), 3);
	EXPECT_EQ(img3.color_space(), ColorSpace::RGB);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(img3.data(), img3.data_size(), 29));
}

TEST(CImageTests, MoveConstructor_and_MoveAssignment)
{
	auto img1 = CImage<int, 4>{ 4, 6, ColorSpace::Unspecified, 21 };
	auto img2 = CImage<int, 4>{ 8, 8, ColorSpace::Unspecified, 17 };

	// Move constructor
	CImage<int, 4> img3 = std::move(img1);
	EXPECT_EQ(img3.height(), 4);
	EXPECT_EQ(img3.width(), 6);
	EXPECT_EQ(img3.size(), 24);
	EXPECT_EQ(img3.data_size(), 96);
	EXPECT_EQ(img3.num_channels(), 4);
	EXPECT_EQ(img3.color_space(), ColorSpace::Unspecified);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img3.data(), img3.data_size(), 21));

	EXPECT_EQ(img1.height(), 0);
	EXPECT_EQ(img1.width(), 0);
	EXPECT_EQ(img1.size(), 0);
	EXPECT_EQ(img1.data_size(), 0);
	EXPECT_EQ(img1.num_channels(), 4);
	EXPECT_EQ(img1.color_space(), ColorSpace::Unspecified);
	EXPECT_EQ(img1.data(), nullptr);

	// Move assignment
	img3 = std::move(img2);
	EXPECT_EQ(img3.height(), 8);
	EXPECT_EQ(img3.width(), 8);
	EXPECT_EQ(img3.size(), 64);
	EXPECT_EQ(img3.data_size(), 256);
	EXPECT_EQ(img3.num_channels(), 4);
	EXPECT_EQ(img3.color_space(), ColorSpace::Unspecified);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img3.data(), img3.data_size(), 17));

	EXPECT_EQ(img2.height(), 0);
	EXPECT_EQ(img2.width(), 0);
	EXPECT_EQ(img2.size(), 0);
	EXPECT_EQ(img2.data_size(), 0);
	EXPECT_EQ(img2.num_channels(), 4);
	EXPECT_EQ(img2.color_space(), ColorSpace::Unspecified);
	EXPECT_EQ(img1.data(), nullptr);

	// Self move assignment
	img3 = std::move(img3);
	EXPECT_EQ(img3.height(), 8);
	EXPECT_EQ(img3.width(), 8);
	EXPECT_EQ(img3.size(), 64);
	EXPECT_EQ(img3.data_size(), 256);
	EXPECT_EQ(img3.num_channels(), 4);
	EXPECT_EQ(img3.color_space(), ColorSpace::Unspecified);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img3.data(), img3.data_size(), 17));
}

TEST(CImageTests, Clear)
{
	auto img = CImage<uint8_t, 3>{ 4, 6, ColorSpace::RGB, 19 };
	img.clear();
	EXPECT_EQ(img.height(), 0);
	EXPECT_EQ(img.width(), 0);
	EXPECT_EQ(img.size(), 0);
	EXPECT_EQ(img.data_size(), 0);
	EXPECT_EQ(img.num_channels(), 3);
	EXPECT_EQ(img.color_space(), ColorSpace::RGB);
	EXPECT_EQ(img.data(), nullptr);
}

TEST(CImageTests, AssignValue) 
{
	auto img = CImage<uint8_t, 3>{ 4, 6, ColorSpace::RGB, 19 };
	img = static_cast<uint8_t>(33);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img.data(), img.data_size(), 33));
	img = static_cast<uint8_t>(12);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img.data(), img.data_size(), 12));
}

TEST(CImageTests, Set_and_Get_Channel)
{
	auto img = CImage<int, 3>{ 4, 6, ColorSpace::RGB, 17 };
	for (size_t i = 0; i < img.num_channels(); i++)
		EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img.get_channel(i).get(), img.size(), 17));

	img.set_channel(0, -2);
	img.set_channel(1, 87);
	img.set_channel(2, 103);

	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img.get_channel(0).get(), img.size(), -2));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img.get_channel(1).get(), img.size(), 87));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img.get_channel(2).get(), img.size(), 103));
}

TEST(CImageTests, SetChannels)
{
	auto img = CImage<int, 5>{ 4, 6, ColorSpace::Unspecified, -1 };
	img.set_channels(2, 4, 6);

	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img.get_channel(0).get(), img.size(), 2));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img.get_channel(1).get(), img.size(), 4));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img.get_channel(2).get(), img.size(), 6));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img.get_channel(3).get(), img.size(), -1));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img.get_channel(4).get(), img.size(), -1));

	img.set_channels(12, 14, 16, 18, 20);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img.get_channel(0).get(), img.size(), 12));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img.get_channel(1).get(), img.size(), 14));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img.get_channel(2).get(), img.size(), 16));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img.get_channel(3).get(), img.size(), 18));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img.get_channel(4).get(), img.size(), 20));
}

TEST(CImageTests, Resize) 
{
	auto img = CImage<uint8_t, 3>{ 10, 5, ColorSpace::RGB };
	img.resize(4, 8);
	EXPECT_EQ(img.height(), 4);
	EXPECT_EQ(img.width(), 8);
	EXPECT_EQ(img.size(), 32);
	EXPECT_EQ(img.data_size(), 96);
	EXPECT_EQ(img.num_channels(), 3);
	EXPECT_EQ(img.color_space(), ColorSpace::RGB);
}

TEST(CImageTests, SetPixel) 
{
	auto img = CImage<uint8_t, 3>{ 10, 5, ColorSpace::RGB, 0 };
	uint8_t x{ 0 };

	for (size_t i = 0; i < img.size(); i++, x += 3)
		img.set_pixel(i, static_cast<uint8_t>(x+1), static_cast<uint8_t>(x + 2), static_cast<uint8_t>(x + 3));
	
	x = 0;
	const auto* data = img.data();
	for (size_t i = 0; i < img.data_size(); i++)
		EXPECT_EQ(data[i], ++x);

	x = 10;
	for (size_t r = 0; r < img.height(); r++) 
	{
		for (size_t c = 0; c < img.width(); c++)
		{
			img.set_pixel(r, c, static_cast<uint8_t>(x + 1), static_cast<uint8_t>(x + 2), static_cast<uint8_t>(x + 3));
			x += 3;
		}
	}
		
	x = 10;
	for (size_t i = 0; i < img.data_size(); i++)
		EXPECT_EQ(data[i], ++x);
}

TEST(CImageTests, AccessPixels)
{
	auto img = CImage<uint8_t, 3>{ 10, 5, ColorSpace::RGB, 0 };
	uint8_t x{ 0 };

	for (size_t i = 0; i < img.size(); i++)
		for (size_t j = 0; j < 3; j++)
			img(i, j) = ++x;

	x = 0;
	const auto* data = img.data();
	for (size_t i = 0; i < img.data_size(); i++)
		EXPECT_EQ(data[i], ++x);

	x = 0;
	for (size_t i = 0; i < img.size(); i++)
		for (size_t j = 0; j < 3; j++)
			EXPECT_EQ(img(i, j), ++x);

	x = 0;
	const CImage<uint8_t, 3> img2 = img;
	for (size_t i = 0; i < img2.size(); i++)
		for (size_t j = 0; j < 3; j++)
			EXPECT_EQ(img2(i, j), ++x);
}