#include "pch.h"
#include "test_helpers.h"

#include <imglib/image/image.hpp>

using namespace imglib;

#pragma warning(disable : 26800) 
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
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::GrayScale, 0), std::invalid_argument);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::GrayScale, 2), std::invalid_argument);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::RGB, 2), std::invalid_argument);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::RGB, 4), std::invalid_argument);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::CMYK, 0), std::invalid_argument);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::CMYK, 5), std::invalid_argument);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::YCbCr, 2), std::invalid_argument);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::YCbCr, 4), std::invalid_argument);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::YCCK, 1), std::invalid_argument);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::YCCK, 6), std::invalid_argument);
	EXPECT_THROW(Image<uint8_t>(2, 4, ColorSpace::Unspecified, 0), std::invalid_argument);

	EXPECT_THROW(Image<uint8_t>(0, 4, ColorSpace::GrayScale, 1), std::invalid_argument);
	EXPECT_THROW(Image<uint8_t>(2, 0, ColorSpace::GrayScale, 1), std::invalid_argument);
	
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

TEST(ImageTests, CopyConstructor_and_CopyAssignment)
{
	auto img1 = Image<char>(5, 2, ColorSpace::RGB, 3, 23);
	auto img2 = Image<char>(7, 4, ColorSpace::GrayScale, 1, 29);

	// Copy constructor
	Image<char> img3 = img1;
	EXPECT_EQ(img3.height(), 5);
	EXPECT_EQ(img3.width(), 2);
	EXPECT_EQ(img3.size(), 10);
	EXPECT_EQ(img3.data_size(), 30);
	EXPECT_EQ(img3.num_channels(), 3);
	EXPECT_EQ(img3.color_space(), ColorSpace::RGB);
	for (size_t i = 0; i < img3.num_channels(); i++)
		EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(img3(i).data(), img3.size(), 23));

	// Copy assignment
	img3 = img2;
	EXPECT_EQ(img3.height(), 7);
	EXPECT_EQ(img3.width(), 4);
	EXPECT_EQ(img3.size(), 28);
	EXPECT_EQ(img3.data_size(), 28);
	EXPECT_EQ(img3.num_channels(), 1);
	EXPECT_EQ(img3.color_space(), ColorSpace::GrayScale);
	for (size_t i = 0; i < img3.num_channels(); i++)
		EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(img3(i).data(), img3.size(), 29));

	// Self copy assignment
	img3 = img3;
	EXPECT_EQ(img3.height(), 7);
	EXPECT_EQ(img3.width(), 4);
	EXPECT_EQ(img3.size(), 28);
	EXPECT_EQ(img3.data_size(), 28);
	EXPECT_EQ(img3.num_channels(), 1);
	EXPECT_EQ(img3.color_space(), ColorSpace::GrayScale);
	for (size_t i = 0; i < img3.num_channels(); i++)
		EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(img3(i).data(), img3.size(), 29));
}

TEST(ImageTests, MoveConstructor_and_MoveAssignment)
{
	auto img1 = Image<int>{ 4, 6, ColorSpace::RGB, 3, 21 };
	auto img2 = Image<int>{ 8, 8, ColorSpace::RGB, 3, 17 };

	// Move constructor
	Image<int> img3 = std::move(img1);
	EXPECT_EQ(img3.height(), 4);
	EXPECT_EQ(img3.width(), 6);
	EXPECT_EQ(img3.size(), 24);
	EXPECT_EQ(img3.data_size(), 72);
	EXPECT_EQ(img3.num_channels(), 3);
	EXPECT_EQ(img3.color_space(), ColorSpace::RGB);
	for (size_t i = 0; i < img3.num_channels(); i++)
		EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img3(i).data(), img3.size(), 21));

	EXPECT_EQ(img1.height(), 0);
	EXPECT_EQ(img1.width(), 0);
	EXPECT_EQ(img1.size(), 0);
	EXPECT_EQ(img1.data_size(), 0);
	EXPECT_EQ(img1.num_channels(), 0);
	EXPECT_EQ(img1.color_space(), ColorSpace::Unspecified);
	
	// Move assignment
	img3 = std::move(img2);
	EXPECT_EQ(img3.height(), 8);
	EXPECT_EQ(img3.width(), 8);
	EXPECT_EQ(img3.size(), 64);
	EXPECT_EQ(img3.data_size(), 192);
	EXPECT_EQ(img3.num_channels(), 3);
	EXPECT_EQ(img3.color_space(), ColorSpace::RGB);
	for (size_t i = 0; i < img3.num_channels(); i++)
		EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img3(i).data(), img3.size(), 17));

	EXPECT_EQ(img2.height(), 0);
	EXPECT_EQ(img2.width(), 0);
	EXPECT_EQ(img2.size(), 0);
	EXPECT_EQ(img2.data_size(), 0);
	EXPECT_EQ(img2.num_channels(), 0);
	EXPECT_EQ(img2.color_space(), ColorSpace::Unspecified);

	// Self move assignment
	img3 = std::move(img3);
	EXPECT_EQ(img3.height(), 8);
	EXPECT_EQ(img3.width(), 8);
	EXPECT_EQ(img3.size(), 64);
	EXPECT_EQ(img3.data_size(), 192);
	EXPECT_EQ(img3.num_channels(), 3);
	EXPECT_EQ(img3.color_space(), ColorSpace::RGB);
	for (size_t i = 0; i < img3.num_channels(); i++)
		EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img3(i).data(), img3.size(), 17));
}

TEST(ImageTests, Data)
{
	Image<int8_t> img1;
	EXPECT_EQ(img1.data(), nullptr);

	auto img2 = Image<double>{ 6, 4, ColorSpace::GrayScale, 1, 3.14 };
	auto data2 = img2.data();
	for (auto i = 0; i < img2.data_size(); i++)
		EXPECT_EQ(data2[i], 3.14);

	auto img3 = Image<int8_t>{ 3, 2, ColorSpace::RGB, 3, 0 };
	img3(0) = 1;
	img3(1) = 2;
	img3(2) = 3;
	auto data3 = img3.data();
	for (auto i = 0; i < img3.data_size(); i++) 
	{
		if (i % 3 == 0)
			EXPECT_EQ(data3[i], 1);
		if (i % 3 == 1)
			EXPECT_EQ(data3[i], 2);
		if (i % 3 == 2)
			EXPECT_EQ(data3[i], 3);
	}
}
TEST(ImageTests, Clear)
{
	auto img = Image<uint8_t>{ 4, 6, ColorSpace::RGB, 3, 19 };
	img.clear();
	EXPECT_EQ(img.height(), 0);
	EXPECT_EQ(img.width(), 0);
	EXPECT_EQ(img.size(), 0);
	EXPECT_EQ(img.data_size(), 0);
	EXPECT_EQ(img.num_channels(), 0);
	EXPECT_EQ(img.color_space(), ColorSpace::Unspecified);
	EXPECT_EQ(img.data(), nullptr);
}

TEST(ImageTests, AppendChannel)
{
	// Append fails - color space must be unspecified
	auto img1 = Image<uint8_t>{ 4, 6, ColorSpace::GrayScale, 1, 19 };
	Channel<uint8_t> ch1{ 2, 6 };
	EXPECT_THROW(img1.append_channel(ch1), std::invalid_argument);

	// Append fails - height mismatch
	auto img2 = Image<uint8_t>{ 4, 6, ColorSpace::Unspecified, 1, 19 };
	Channel<uint8_t> ch2{ 4, 5 };
	EXPECT_THROW(img2.append_channel(ch2), std::invalid_argument);

	// Append fails - width mismatch
	Channel<uint8_t> ch3{ 3, 6 };
	EXPECT_THROW(img2.append_channel(ch3), std::invalid_argument);

	// Append with lvalue reference
	Channel<uint8_t> ch4{ 4, 6, 43 };
	EXPECT_NO_THROW(img2.append_channel(ch4));
	EXPECT_EQ(img2.num_channels(), 2);
	EXPECT_EQ(img2.data_size(), 48);
    EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img2(0).data(), img2.size(), 19));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img2(1).data(), img2.size(), 43));

	// Append with rvalue reference
	ch4 = 32;
	EXPECT_NO_THROW(img2.append_channel(std::move(ch4)));
	EXPECT_EQ(img2.num_channels(), 3);
	EXPECT_EQ(img2.data_size(), 72);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img2(0).data(), img2.size(), 19));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img2(1).data(), img2.size(), 43));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img2(2).data(), img2.size(), 32));
}

TEST(ImageTests, DeleteChannel_test1)
{
	auto img = Image<int>{ 5, 5, ColorSpace::Unspecified, 5 };
	img(0) = 103; 
	img(1) = -123;
	img(2) = -345; 
	img(3) = 12;
	img(4) = 42;

	EXPECT_THROW(img.delete_channel(5), std::invalid_argument);

	img.delete_channel(2);
	EXPECT_EQ(img.num_channels(), 4);
	EXPECT_EQ(img.data_size(), 100);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img(0).data(), img.size(), 103));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img(1).data(), img.size(), -123));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img(2).data(), img.size(), 12));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img(3).data(), img.size(), 42));
	EXPECT_THROW(img.delete_channel(4), std::invalid_argument);
	
	img.delete_channel(0);
	EXPECT_EQ(img.num_channels(), 3);
	EXPECT_EQ(img.data_size(), 75);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img(0).data(), img.size(), -123));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img(1).data(), img.size(), 12));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img(2).data(), img.size(), 42));
	EXPECT_THROW(img.delete_channel(3), std::invalid_argument);

	img.delete_channel(2);
	EXPECT_EQ(img.num_channels(), 2);
	EXPECT_EQ(img.data_size(), 50);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img(0).data(), img.size(), -123));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img(1).data(), img.size(), 12));
	EXPECT_THROW(img.delete_channel(2), std::invalid_argument);

	img.delete_channel(0);
	img.delete_channel(0);
	EXPECT_EQ(img.num_channels(), 0);
	EXPECT_EQ(img.data_size(), 0);
	EXPECT_THROW(img.delete_channel(0), std::invalid_argument);
}

TEST(ImageTests, DeleteChannel_test2)
{
	auto img = Image<int>{ 5, 5, ColorSpace::RGB, 3 };
	EXPECT_THROW(img.delete_channel(0), std::invalid_argument);
	EXPECT_THROW(img.delete_channel(1), std::invalid_argument);
	EXPECT_THROW(img.delete_channel(2), std::invalid_argument);
	EXPECT_THROW(img.delete_channel(3), std::invalid_argument);
}

TEST(ImageTests, ReplaceChannel_test1)
{
	auto img = Image<float>{ 10, 5, ColorSpace::RGB, 3 };
	EXPECT_THROW(img.replace_channel(3, Channel<float>{10, 5}), std::invalid_argument);
	EXPECT_THROW(img.replace_channel(0, Channel<float>{9, 5}), std::invalid_argument);
	EXPECT_THROW(img.replace_channel(1, Channel<float>{10, 7}), std::invalid_argument);
	img.clear();
	EXPECT_THROW(img.replace_channel(0, Channel<float>{10, 5}), std::invalid_argument);
}

TEST(ImageTests, ReplaceChannel_test2)
{
	auto img = Image<float>{ 10, 5, ColorSpace::RGB, 3, 0.5f};
	img.replace_channel(1, Channel<float>{10, 5, 1.2f});
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(img(0).data(), img.size(), 0.5f));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(img(1).data(), img.size(), 1.2f));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(img(2).data(), img.size(), 0.5f));

	img.replace_channel(0, Channel<float>{10, 5, 0.7f});
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(img(0).data(), img.size(), 0.7f));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(img(1).data(), img.size(), 1.2f));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(img(2).data(), img.size(), 0.5f));

	img.replace_channel(2, Channel<float>{10, 5, 0.2f});
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(img(0).data(), img.size(), 0.7f));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(img(1).data(), img.size(), 1.2f));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(img(2).data(), img.size(), 0.2f));

	auto ch1 = Channel<float>{ 10, 5, 0.8f };
	img.replace_channel(0, ch1);
	img.replace_channel(1, ch1);
	img.replace_channel(2, std::move(ch1));
	for (size_t i = 0; i < 3; i++)
		EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(img(i).data(), img.size(), 0.8f));
}
TEST(ImageTests, ExchangeChannels_test1)
{
	auto img1 = Image<uint8_t>{ 4, 6, ColorSpace::RGB, 3 };
	EXPECT_THROW(img1.exchange_channel(0, 3), std::invalid_argument);
	EXPECT_THROW(img1.exchange_channel(3, 0), std::invalid_argument);

	auto img2 = Image<uint8_t>{ 4, 6, ColorSpace::GrayScale, 1 };
	EXPECT_THROW(img2.exchange_channel(0, 0), std::invalid_argument);
}

TEST(ImageTests, ExchangeChannels_test2)
{
	auto img = Image<uint8_t>{ 4, 6, ColorSpace::RGB, 3 };
	img(0) = 23;
	img(1) = 45;
	img(2) = 89;
	img.exchange_channel(0, 0);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img(0).data(), img.size(), 23));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img(1).data(), img.size(), 45));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img(2).data(), img.size(), 89));

	img.exchange_channel(0, 2);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img(0).data(), img.size(), 89));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img(1).data(), img.size(), 45));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img(2).data(), img.size(), 23));

	img.exchange_channel(1, 2);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img(0).data(), img.size(), 89));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img(1).data(), img.size(), 23));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img(2).data(), img.size(), 45));
}

TEST(ImageTests, SetAllChannels) 
{
	// Set all channel pixels to the same value
	auto img = Image<uint8_t>{ 4, 6, ColorSpace::RGB, 3 };
	img = 12;

	for (size_t i = 0; i < img.num_channels(); i++)
		EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(img(i).data(), img.size(), 12));
}

TEST(ImageTests, SetChannels)
{
	// Set all channel pixels to the same value
	auto img = Image<int>{ 4, 6, ColorSpace::RGB, 3 };
	img.set_channels(-1, 2, 45);

	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img(0).data(), img.size(), -1));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img(1).data(), img.size(), 2));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(img(2).data(), img.size(), 45));

	EXPECT_THROW(img.set_channels(1), std::invalid_argument);
	EXPECT_THROW(img.set_channels(1,2), std::invalid_argument);
	EXPECT_THROW(img.set_channels(1,2,3,4), std::invalid_argument);
}
TEST(ImageTests, SetPixel_test1) 
{
	auto img = Image<uint8_t>{ 10, 5, ColorSpace::GrayScale, 1 };
	uint8_t start = 0;
	for (size_t i = 0; i < img.size(); i++)
		img.set_pixel(i, start++);

	start = 0;
	for (auto it = img(0).begin(); it != img(0).end(); ++it)
		EXPECT_FLOAT_EQ(*it, start++);

	img.set_pixel(0, 0, static_cast<uint8_t>(100));
	img.set_pixel(4, 3, static_cast<uint8_t>(101));
	img.set_pixel(9, 4, static_cast<uint8_t>(102));

	EXPECT_EQ(img(0)(0, 0), 100);
	EXPECT_EQ(img(0)(4, 3), 101);
	EXPECT_EQ(img(0)(9, 4), 102);
}
TEST(ImageTests, SetPixel_test2) 
{
	auto img = Image<float>{ 6, 10, ColorSpace::RGB, 3 };
	float start = 0.0f;
	for (size_t i = 0; i < img.size(); i++) 
	{
		img.set_pixel(i, start, start + 0.1f, start + 0.2f);
		start += 0.3f;
	}

	for (size_t i = 0; i < img.num_channels(); i++) 
	{
		start = (i * 0.1f);
		for (auto it = img(i).begin(); it != img(i).end(); ++it, start += 0.3f)
			EXPECT_FLOAT_EQ(*it, start);
	}

	img = 0.0f;
	img.set_pixel(3, 4, 0.1f, 0.2f, 0.3f);
	EXPECT_FLOAT_EQ(img(0)(3, 4), 0.1f);
	EXPECT_FLOAT_EQ(img(1)(3, 4), 0.2f);
	EXPECT_FLOAT_EQ(img(2)(3, 4), 0.3f);
}

TEST(ImageTests, SetPixel_test3)
{
	auto img = Image<int>{ 6, 10, ColorSpace::Unspecified, 6 };
	int start = 0;
	for (size_t i = 0; i < img.size(); i++)
	{
		img.set_pixel(i, start, start+1, start+2, start+3, start+4, start+5);
		start += 6;
	}

	for (size_t i = 0; i < img.num_channels(); i++)
	{
		start = static_cast<int>(i);
		for (auto it = img(i).begin(); it != img(i).end(); ++it, start += 6)
			EXPECT_EQ(*it, start);
	}

	img = 0;
	img.set_pixel((size_t)5, (size_t)6, 11, 23, 34, 56, 42, 109);
	EXPECT_EQ(img(0)(5, 6), 11);
	EXPECT_EQ(img(1)(5, 6), 23);
	EXPECT_EQ(img(2)(5, 6), 34);
	EXPECT_EQ(img(3)(5, 6), 56);
	EXPECT_EQ(img(4)(5, 6), 42);
	EXPECT_EQ(img(5)(5, 6), 109);
}

