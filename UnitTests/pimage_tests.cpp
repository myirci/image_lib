#include "pch.h"

#include <imglib/image/pimage.hpp>

using namespace imglib;

TEST(PImageTests, PixelConstructor)
{
	auto p1 = Pixel<int, 3>{};
	EXPECT_EQ(p1.p[0], int{});
	EXPECT_EQ(p1.p[1], int{});
	EXPECT_EQ(p1.p[2], int{});

	auto p2 = Pixel<char, 2>{(char)3, (char)5};
	EXPECT_EQ(p2.p[0], 3);
	EXPECT_EQ(p2.p[1], 5);

	auto p3 = Pixel<float, 4>{ 0.1f, 0.2f, 0.3f, 0.4f };
	EXPECT_FLOAT_EQ(p3.p[0], 0.1f);
	EXPECT_FLOAT_EQ(p3.p[1], 0.2f);
	EXPECT_FLOAT_EQ(p3.p[2], 0.3f);
	EXPECT_FLOAT_EQ(p3.p[3], 0.4f);
}

TEST(PImageTests, DefaultConstructor)
{
	auto img = PImage<uint8_t, 1>{};
	EXPECT_EQ(img.height(), 0);
	EXPECT_EQ(img.width(), 0);
	EXPECT_EQ(img.num_channels(), 1);
	EXPECT_EQ(img.color_space(), ColorSpace::Unspecified);
	EXPECT_EQ(img.size(), 0);
	EXPECT_EQ(img.data_size(), 0);
}

TEST(PImageTests, Constructor)
{
	EXPECT_THROW((PImage<char, 3>(0, 10, ColorSpace::RGB)), std::invalid_argument);
	EXPECT_THROW((PImage<char, 3>(10, 0, ColorSpace::RGB)), std::invalid_argument);
	EXPECT_THROW((PImage<char, 3>(3, 5, ColorSpace::GrayScale)), std::invalid_argument);
	EXPECT_NO_THROW((PImage<char, 3>(3, 4, ColorSpace::RGB)));
	EXPECT_NO_THROW((PImage<char, 3>(3, 4, ColorSpace::YCbCr)));
	EXPECT_NO_THROW((PImage<char, 4>(3, 4, ColorSpace::CMYK)));
	EXPECT_NO_THROW((PImage<char, 5>(3, 4, ColorSpace::Unspecified)));

	uint8_t val = 10;
	Pixel<uint8_t, 3> px{ val, ++val, ++val };
	auto img = PImage<uint8_t, 3>{ 5, 7, ColorSpace::RGB, px };
	EXPECT_EQ(img.height(), 5);
	EXPECT_EQ(img.width(), 7);
	EXPECT_EQ(img.num_channels(), 3);
	EXPECT_EQ(img.color_space(), ColorSpace::RGB);
	EXPECT_EQ(img.size(), 35);
	EXPECT_EQ(img.data_size(), 105);

	for (size_t i = 0; i < img.height(); i++)
		for (size_t j = 0; j < img.width(); j++)
			EXPECT_TRUE(img(i, j) == px);
}

TEST(PImageTests, CopyConstructor_and_CopyAssignment) 
{
	Pixel<int, 3> px1{ 12, -5, 23 };
	auto img1 = PImage<int, 3>{ 5, 10, ColorSpace::RGB, px1 };
	
	Pixel<int, 3> px2{ 112, 52, -24 };
	auto img2 = PImage<int, 3>{ 4, 8, ColorSpace::Unspecified, px2 };

	// Copy constructor
	PImage<int, 3> img3{ img1 };
	EXPECT_EQ(img3.height(), 5);
	EXPECT_EQ(img3.width(), 10);
	EXPECT_EQ(img3.num_channels(), 3);
	EXPECT_EQ(img3.color_space(), ColorSpace::RGB);
	EXPECT_EQ(img3.size(), 50);
	EXPECT_EQ(img3.data_size(), 150);

	for (size_t i = 0; i < img1.size(); i++)
		EXPECT_TRUE(img3(i) == img1(i));

	// Copy assingment
	img3 = img2;
	EXPECT_EQ(img3.height(), 4);
	EXPECT_EQ(img3.width(), 8);
	EXPECT_EQ(img3.num_channels(), 3);
	EXPECT_EQ(img3.color_space(), ColorSpace::Unspecified);
	EXPECT_EQ(img3.size(), 32);
	EXPECT_EQ(img3.data_size(), 96);

	for (size_t i = 0; i < img2.size(); i++)
		EXPECT_TRUE(img3(i) == img2(i));
}

TEST(PImageTests, MoveConstructor_and_MoveAssignment)
{
	Pixel<int, 3> px1{ 12, -5, 3 };
	auto img1 = PImage<int, 3>{ 5, 10, ColorSpace::RGB, px1 };

	Pixel<int, 3> px2{ 21, 2, -2 };
	auto img2 = PImage<int, 3>{ 4, 8, ColorSpace::Unspecified, px2 };

	// Move constructor
	PImage<int, 3> img3{ std::move(img1) };
	EXPECT_EQ(img3.height(), 5);
	EXPECT_EQ(img3.width(), 10);
	EXPECT_EQ(img3.num_channels(), 3);
	EXPECT_EQ(img3.color_space(), ColorSpace::RGB);
	EXPECT_EQ(img3.size(), 50);
	EXPECT_EQ(img3.data_size(), 150);

	for (size_t i = 0; i < img3.size(); i++)
		EXPECT_TRUE(img3(i) == px1);

	EXPECT_EQ(img1.height(), 0);
	EXPECT_EQ(img1.width(), 0);
	EXPECT_EQ(img1.num_channels(), 3);
	EXPECT_EQ(img1.color_space(), ColorSpace::RGB);
	EXPECT_EQ(img1.size(), 0);
	EXPECT_EQ(img1.data_size(), 0);

	// Move assingment
	img3 = std::move(img2);
	EXPECT_EQ(img3.height(), 4);
	EXPECT_EQ(img3.width(), 8);
	EXPECT_EQ(img3.num_channels(), 3);
	EXPECT_EQ(img3.color_space(), ColorSpace::Unspecified);
	EXPECT_EQ(img3.size(), 32);
	EXPECT_EQ(img3.data_size(), 96);

	for (size_t i = 0; i < img3.size(); i++)
		EXPECT_TRUE(img3(i) == px2);

	EXPECT_EQ(img2.height(), 0);
	EXPECT_EQ(img2.width(), 0);
	EXPECT_EQ(img2.num_channels(), 3);
	EXPECT_EQ(img2.color_space(), ColorSpace::Unspecified);
	EXPECT_EQ(img2.size(), 0);
	EXPECT_EQ(img2.data_size(), 0);
}

TEST(PImageTests, SetAllPixels)
{
	Pixel<float, 3> px1{ 1.3f, -5.23f, 3.45f };
	auto img = PImage<float, 3>{ 5, 10, ColorSpace::RGB, px1 };

	for (size_t i = 0; i < img.size(); i++)
		EXPECT_TRUE(img(i) == px1);

	Pixel<float, 3> px2{ 12.123f, -543.233f, 13.4345f };
	img = px2;

	for (size_t i = 0; i < img.size(); i++)
		EXPECT_TRUE(img(i) == px2);

	Pixel<float, 3> px3{ 142.13f, -43.33f, 3.45f };
	img = px3;

	for (size_t i = 0; i < img.size(); i++)
		EXPECT_TRUE(img(i) == px3);
}

TEST(PImageTests, Iterator_test1)
{
	auto img = PImage<int, 3>{ 5, 10, ColorSpace::RGB };
	int x{ 0 }, y{ 1 }, z{ 2 };
	std::vector<Pixel<int, 3>> pixels;
	
	for (size_t i = 0; i < img.size(); i++) 
	{
		pixels.push_back(Pixel<int, 3>{ x++, y++, z++ });
		img(i) = pixels.back();
	}

	size_t i = 0;
	for (auto it = img.begin(); it != img.end(); it++, i++)
		EXPECT_TRUE(*it == pixels[i]);

	i = pixels.size() - 1;
	for (auto it = img.rbegin(); it != img.rend(); it++, i--)
		EXPECT_TRUE(*it == pixels[i]);

	i = 0;
	for (auto it = img.cbegin(); it != img.cend(); it++, i++)
		EXPECT_TRUE(*it == pixels[i]);

	i = pixels.size() - 1;
	for (auto it = img.crbegin(); it != img.crend(); it++, i--)
		EXPECT_TRUE(*it == pixels[i]);
}

TEST(PImageTests, Iterator_test2)
{
	Pixel<int, 3> px{ 10, -23, 4 };
	const auto img = PImage<int, 3>{ 5, 10, ColorSpace::RGB, px };

	for (auto it = img.begin(); it != img.end(); it++)
		EXPECT_TRUE(*it == px);

	for (auto it = img.cbegin(); it != img.cend(); it++)
		EXPECT_TRUE(*it == px);

	for (auto it = img.rbegin(); it != img.rend(); it++)
		EXPECT_TRUE(*it == px);

	for (auto it = img.crbegin(); it != img.crend(); it++)
		EXPECT_TRUE(*it == px);
}

TEST(PImageTests, GenericIterator_test1)
{
	auto img = PImage<int, 3>{ 5, 10, ColorSpace::RGB };

	int x{ 0 }, y{ 1 }, z{ 2 };
	std::vector<Pixel<int, 3>> pixels;
	for (size_t i = 0; i < img.size(); i++)
	{
		pixels.push_back(Pixel<int, 3>{ x++, y++, z++ });
		img(i) = pixels.back();
	}

	size_t row1{ 2 }, col1{ 3 }, row2 { 4 }, col2 { 8 };
	size_t idx1{ row1 * img.width() + col1 }, idx2{ row2 * img.width() + col2 };

	EXPECT_EQ(idx2 - idx1, img.git(row2, col2) - img.git(row1, col1));
	EXPECT_EQ(idx2 - idx1, img.cgit(row2, col2) - img.cgit(row1, col1));

	size_t i = idx1;
	for (auto it = img.git(row1, col1); it != img.git(row2, col2); it++, i++)
		EXPECT_TRUE(*it == pixels[i]);

	i = idx1;
	for (auto it = img.cgit(row1, col1); it != img.cgit(row2, col2); it++, i++)
		EXPECT_TRUE(*it == pixels[i]);
}

TEST(PImageTests, GenericIterator_test2)
{
	Pixel<int, 3> px{ 10, -23, 4 };
	const auto img = PImage<int, 3>{ 5, 10, ColorSpace::RGB, px };

	size_t row1{ 2 }, col1{ 3 }, row2{ 4 }, col2{ 8 };
	size_t idx1{ row1 * img.width() + col1 }, idx2{ row2 * img.width() + col2 };

	EXPECT_EQ(idx2 - idx1, img.git(row2, col2) - img.git(row1, col1));
	EXPECT_EQ(idx2 - idx1, img.cgit(row2, col2) - img.cgit(row1, col1));

	for (auto it = img.git(row1, col1); it != img.git(row2, col2); it++)
		EXPECT_TRUE(*it == px);

	for (auto it = img.cgit(row1, col1); it != img.cgit(row2, col2); it++)
		EXPECT_TRUE(*it == px);
}

TEST(PImageTests, RowIterator_test1)
{
	auto img = PImage<int, 3>{ 5, 10, ColorSpace::RGB };

	int x{ 0 }, y{ 1 }, z{ 2 };
	std::vector<Pixel<int, 3>> pixels;
	for (size_t i = 0; i < img.size(); i++)
	{
		pixels.push_back(Pixel<int, 3>{ x++, y++, z++ });
		img(i) = pixels.back();
	}

	EXPECT_EQ(img.width(), img.row_end(0) - img.row_begin(0));
	EXPECT_EQ(img.width(), img.crow_end(1) - img.crow_begin(1));
	EXPECT_EQ(img.width(), img.rrow_end(2) - img.rrow_begin(2));
	EXPECT_EQ(img.width(), img.crrow_end(3) - img.crrow_begin(3));

	size_t row = 2;
	size_t idx_first = row * img.width();
	size_t idx_last = (row + 1) * img.width() - 1;

	size_t i = idx_first;
	for (auto it = img.row_begin(row); it != img.row_end(row); it++, i++)
		EXPECT_TRUE(*it == pixels[i]);

	i = idx_first;
	for (auto it = img.crow_begin(row); it != img.crow_end(row); it++, i++)
		EXPECT_TRUE(*it == pixels[i]);

	i = idx_last;
	for (auto it = img.rrow_begin(row); it != img.rrow_end(row); it++, i--)
		EXPECT_TRUE(*it == pixels[i]);

	i = idx_last;
	for (auto it = img.crrow_begin(row); it != img.crrow_end(row); it++, i--)
		EXPECT_TRUE(*it == pixels[i]);
}

TEST(PImageTests, RowIterator_test2)
{
	Pixel<int, 3> px{ 10, -23, 4 };
	const auto img = PImage<int, 3>{ 5, 10, ColorSpace::RGB, px };

	for (auto it = img.row_begin(3); it != img.row_end(3); it++)
		EXPECT_TRUE(*it == px);

	for (auto it = img.crow_begin(3); it != img.crow_end(3); it++)
		EXPECT_TRUE(*it == px);

	for (auto it = img.rrow_begin(3); it != img.rrow_end(3); it++)
		EXPECT_TRUE(*it == px);

	for (auto it = img.crow_begin(3); it != img.crow_end(3); it++)
		EXPECT_TRUE(*it == px);
}
TEST(PImageTests, Clear)
{
	auto img = PImage<int, 3>{ 5, 10, ColorSpace::RGB };
	img.clear();
	EXPECT_EQ(img.height(), 0);
	EXPECT_EQ(img.width(), 0);
	EXPECT_EQ(img.size(), 0);
	EXPECT_EQ(img.data_size(), 0);
}
TEST(PImageTests, Resize)
{
	
}