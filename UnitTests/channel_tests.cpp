#include "pch.h"
#include "test_helpers.h"

#include <imglib/image/channel.hpp>
#include <algorithm>

using namespace imglib;
TEST(ChannelTests, DefaultConstructor)
{
	EXPECT_NO_THROW(Channel<uint8_t>{});
	auto ch = Channel<uint8_t>{};
	EXPECT_EQ(ch.num_rows(), 0);
	EXPECT_EQ(ch.num_columns(), 0);
	EXPECT_EQ(ch.size(), 0);
	EXPECT_EQ(ch.data(), nullptr);
	EXPECT_TRUE(ch.empty());
}

TEST(ChannelTests, Constructor_test1)
{
	auto ch = Channel<uint8_t>{ 2, 3, 0 };
	EXPECT_EQ(ch.num_rows(), 2);
	EXPECT_EQ(ch.num_columns(), 3);
	EXPECT_EQ(ch.size(), 6);
	EXPECT_FALSE(ch.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(ch.data(), ch.size(), 0));
}

TEST(ChannelTests, Constructor_test2)
{
	auto ch = Channel<uint8_t>{ 6, 4, 123 };
	EXPECT_EQ(ch.num_rows(), 6);
	EXPECT_EQ(ch.num_columns(), 4);
	EXPECT_EQ(ch.size(), 24);
	EXPECT_FALSE(ch.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(ch.data(), ch.size(), 123));
}

TEST(ChannelTests, Constructor_test3)
{
	auto ch = Channel<float>{ 7, 8, 0.0f };
	EXPECT_EQ(ch.num_rows(), 7);
	EXPECT_EQ(ch.num_columns(), 8);
	EXPECT_EQ(ch.size(), 56);
	EXPECT_FALSE(ch.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(ch.data(), ch.size(), 0.0f));
}

TEST(ChannelTests, Constructor_test4)
{
	auto ch = Channel<float>{ 4, 4, 0.21f };
	EXPECT_EQ(ch.num_rows(), 4);
	EXPECT_EQ(ch.num_columns(), 4);
	EXPECT_EQ(ch.size(), 16);
	EXPECT_FALSE(ch.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(ch.data(), ch.size(), 0.21f));
}

TEST(ChannelTests, Constructor_test5)
{
	auto ch = Channel<float>{ 1, 1, 0.1f };
	EXPECT_EQ(ch.num_rows(), 1);
	EXPECT_EQ(ch.num_columns(), 1);
	EXPECT_EQ(ch.size(), 1);
	EXPECT_FALSE(ch.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(ch.data(), ch.size(), 0.1f));
}

TEST(ChannelTests, Constructor_test6)
{
	EXPECT_THROW(Channel<float>(0, 1, 0.1f), std::invalid_argument);
	EXPECT_THROW(Channel<float>(1, 0, 0.2f), std::invalid_argument);
	EXPECT_THROW(Channel<float>(0, 0, 0.3f), std::invalid_argument);
}

TEST(ChannelTests, CopyConstructor_and_CopyAssignment_test1) 
{
	auto ch1 = Channel<uint8_t>{ 12, 15, 0 };
	auto ch2 = Channel<uint8_t>{ 3, 6, 42 };
	
	// Copy constructor
	Channel<uint8_t> ch3{ ch1 };
	EXPECT_EQ(ch3.num_rows(), 12);
	EXPECT_EQ(ch3.num_columns(), 15);
	EXPECT_EQ(ch3.size(), 180);
	EXPECT_FALSE(ch3.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(ch3.data(), ch3.size(), 0));

	// Copy assignment
	ch3 = ch2;
	EXPECT_EQ(ch3.num_rows(), 3);
	EXPECT_EQ(ch3.num_columns(), 6);
	EXPECT_EQ(ch3.size(), 18);
	EXPECT_FALSE(ch3.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(ch3.data(), ch3.size(), 42));

	// Self copy assignment
	ch3 = ch3;
	EXPECT_EQ(ch3.num_rows(), 3);
	EXPECT_EQ(ch3.num_columns(), 6);
	EXPECT_EQ(ch3.size(), 18);
	EXPECT_FALSE(ch3.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(ch3.data(), ch3.size(), 42));
}

TEST(ChannelTests, CopyConstructor_and_CopyAssignment_test2)
{
	auto ch1 = Channel<uint8_t>{ 11, 11, 34 };
	auto ch2 = Channel<uint8_t>{ 2, 2, 10 };

	// Copy constructor
	Channel<uint8_t> ch3{ ch1 };
	EXPECT_EQ(ch3.num_rows(), 11);
	EXPECT_EQ(ch3.num_columns(), 11);
	EXPECT_EQ(ch3.size(), 121);
	EXPECT_FALSE(ch3.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(ch3.data(), ch3.size(), 34));

	// Copy assignment
	ch3 = ch2;
	EXPECT_EQ(ch3.num_rows(), 2);
	EXPECT_EQ(ch3.num_columns(), 2);
	EXPECT_EQ(ch3.size(), 4);
	EXPECT_FALSE(ch3.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint8_t>(ch3.data(), ch3.size(), 10));
}

TEST(ChannelTests, CopyConstructor_and_CopyAssignment_test3)
{
	auto ch1 = Channel<uint16_t>{ 12, 15, 2 };
	auto ch2 = Channel<uint16_t>{ 3, 4, 99 };

	// Copy constructor
	Channel<uint16_t> ch3{ ch1 };
	EXPECT_EQ(ch3.num_rows(), 12);
	EXPECT_EQ(ch3.num_columns(), 15);
	EXPECT_EQ(ch3.size(), 180);
	EXPECT_FALSE(ch3.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint16_t>(ch3.data(), ch3.size(), 2));

	// Copy assignment
	ch3 = ch2;
	EXPECT_EQ(ch3.num_rows(), 3);
	EXPECT_EQ(ch3.num_columns(), 4);
	EXPECT_EQ(ch3.size(), 12);
	EXPECT_FALSE(ch3.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<uint16_t>(ch3.data(), ch3.size(), 99));
}

TEST(ChannelTests, CopyConstructor_and_CopyAssignment_test4)
{
	auto ch1 = Channel<int>{ 4, 3, 255 };
	auto ch2 = Channel<int>{ 5, 4, -2 };

	// Copy constructor
	Channel<int> ch3{ ch1 };
	EXPECT_EQ(ch3.num_rows(), 4);
	EXPECT_EQ(ch3.num_columns(), 3);
	EXPECT_EQ(ch3.size(), 12);
	EXPECT_FALSE(ch3.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(ch3.data(), ch3.size(), 255));

	// Copy assignment
	ch3 = ch2;
	EXPECT_EQ(ch3.num_rows(), 5);
	EXPECT_EQ(ch3.num_columns(), 4);
	EXPECT_EQ(ch3.size(), 20);
	EXPECT_FALSE(ch3.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(ch3.data(), ch3.size(), -2));
}

TEST(ChannelTests, CopyConstructor_and_CopyAssignment_test5)
{
	auto ch1 = Channel<float>{ 40, 2, 0.21f };
	auto ch2 = Channel<float>{ 1, 1, 34.567f };

	// Copy constructor
	Channel<float> ch3{ ch1 };
	EXPECT_EQ(ch3.num_rows(), 40);
	EXPECT_EQ(ch3.num_columns(), 2);
	EXPECT_EQ(ch3.size(), 80);
	EXPECT_FALSE(ch3.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(ch3.data(), ch3.size(), 0.21f));

	// Copy assignment
	ch3 = ch2;
	EXPECT_EQ(ch3.num_rows(), 1);
	EXPECT_EQ(ch3.num_columns(), 1);
	EXPECT_EQ(ch3.size(), 1);
	EXPECT_FALSE(ch3.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(ch3.data(), ch3.size(), 34.567f));
}

TEST(ChannelTests, MoveConstructor_and_MoveAssignment) 
{
	auto ch1 = Channel<double>{ 17, 3, 0.99 };
	auto ch2 = Channel<double>{ 23, 5, -0.23 };

	// Move constructor
	Channel<double> ch3 = std::move(ch1);
	EXPECT_EQ(ch3.num_rows(), 17);
	EXPECT_EQ(ch3.num_columns(), 3);
	EXPECT_EQ(ch3.size(), 51);
	EXPECT_FALSE(ch3.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<double>(ch3.data(), ch3.size(), 0.99));
	
	EXPECT_EQ(ch1.num_rows(), 0);
	EXPECT_EQ(ch1.num_columns(), 0);
	EXPECT_EQ(ch1.size(), 0);
	EXPECT_EQ(ch1.data(), nullptr);
	EXPECT_TRUE(ch1.empty());

	// Move assignment
	ch3 = std::move(ch2);
	EXPECT_EQ(ch3.num_rows(), 23);
	EXPECT_EQ(ch3.num_columns(), 5);
	EXPECT_EQ(ch3.size(), 115);
	EXPECT_FALSE(ch3.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<double>(ch3.data(), ch3.size(), -0.23));

	EXPECT_EQ(ch2.num_rows(), 0);
	EXPECT_EQ(ch2.num_columns(), 0);
	EXPECT_EQ(ch2.size(), 0);
	EXPECT_EQ(ch2.data(), nullptr);
	EXPECT_TRUE(ch2.empty());

	// Self move assignment
	ch3 = std::move(ch3);
	EXPECT_EQ(ch3.num_rows(), 23);
	EXPECT_EQ(ch3.num_columns(), 5);
	EXPECT_EQ(ch3.size(), 115);
	EXPECT_FALSE(ch3.empty());
	EXPECT_TRUE(helpers::AllPixelsEqualTo<double>(ch3.data(), ch3.size(), -0.23));

}

TEST(ChannelTests, SetAllPixelValues)
{
	auto ch = Channel<char>{ 2, 3, 0 };
	EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(ch.data(), ch.size(), 0));
	ch = 27;
	EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(ch.data(), ch.size(), 27));
	ch = -12;
	EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(ch.data(), ch.size(), -12));
}

TEST(ChannelTests, GetSetPixelValues) 
{
	auto ch = Channel<char>{ 2, 3, 0 };
	ch(0, 0) = 1;
	ch(0, 1) = 2;
	ch(0, 2) = 3;
	ch(1, 0) = 4;
	ch(1, 1) = 5;
	ch(1, 2) = 6;

	for (char c = 0; c < 6; ++c)
		EXPECT_EQ(ch(c), c+1);

	for (char c = 0; c < 6; ++c)
		ch(c) = -ch(c);

	for (char c = 0; c < 6; ++c)
		EXPECT_EQ(ch(c), -c-1);
}

TEST(ChannelTests, Iterator) 
{
	auto ch = Channel<int8_t>{ 5, 5, 53 };

	EXPECT_EQ(static_cast<size_t>(ch.end() - ch.begin()), ch.size());
	EXPECT_EQ(static_cast<size_t>(ch.cend() - ch.cbegin()), ch.size());

	for (auto it = ch.begin(); it != ch.end(); ++it)
		EXPECT_EQ(*it, 53);

	int8_t count{ 0 };
	for (auto it = ch.begin(); it != ch.end(); ++it)
		*it = count++;

	count = 0;
	for (auto it = ch.cbegin(); it != ch.cend(); ++it)
		EXPECT_EQ(*it, count++);

	for (auto it = ch.rbegin(); it != ch.rend(); it++)
		EXPECT_EQ(*it, --count);

	count = *ch.crbegin();
	for (auto it = ch.rbegin(); it != ch.rend(); it++)
		EXPECT_EQ(*it, count--);
}

TEST(ChannelTests, GetIterator) 
{
	auto ch = Channel<int8_t>{ 5, 5, -1};
	int8_t count{ 0 };
	std::generate(ch.begin(), ch.end(), [&count] { return count++; });

	count = 0;
	for (auto i = 0; i < ch.num_rows(); i++) 
	{
		for (auto j = 0; j < ch.num_columns(); j++) 
		{
			auto it = ch.get_iterator(i, j);
			EXPECT_EQ(*it, count++);
		}
	}
}

TEST(ChannelTests, RowIterator)
{
	auto ch = Channel<int8_t>{ 10, 10, -1 };
	int8_t count{ 0 };
	std::generate(ch.begin(), ch.end(), [&count] { return count++; });

	// iterate over the first row
	count = 0;
	for (auto it = ch.row_begin(0); it != ch.row_end(0); it++)
		EXPECT_EQ(*it, count++);

	// iterate over the 5th row
	count = 50;
	for (auto it = ch.const_row_begin(5); it != ch.const_row_end(5); it++)
		EXPECT_EQ(*it, count++);

	// reverse it over the 5th row
	for (auto it = ch.rrow_begin(5); it != ch.rrow_end(5); it++)
		EXPECT_EQ(*it, --count);

	// iterate over the last row
	count = 90;
	for (auto it = ch.row_begin(9); it != ch.row_end(9); it++)
		EXPECT_EQ(*it, count++);

	for (auto it = ch.const_rrow_begin(9); it != ch.const_rrow_end(9); it++)
		EXPECT_EQ(*it, --count);
}

TEST(ChannelTests, ColumnIterator)
{
	auto ch = Channel<int8_t>{ 10, 10, -1 };
	int8_t count{ 0 };
	std::generate(ch.begin(), ch.end(), [&count] { return count++; });

	// iterate over the first column
	count = 0;
	for (auto it = ch.column_begin(0); it != ch.column_end(0); it++) 
	{
		EXPECT_EQ(*it, count);
		count += static_cast<int8_t>(ch.num_columns());
	}

	EXPECT_EQ(ch.column_end(0) - ch.column_begin(0), ch.num_rows());
		
	// iterate over the 5th column
	count = 5;
	for (auto it = ch.const_column_begin(5); it != ch.const_column_end(5); it++) 
	{
		EXPECT_EQ(*it, count);
		count += static_cast<int8_t>(ch.num_columns());
	}

	// reverse iterator over the 5th column
	for (auto it = ch.rcolumn_begin(5); it != ch.rcolumn_end(5); it++) 
	{
		count -= static_cast<int8_t>(ch.num_columns());
		EXPECT_EQ(*it, count);
	}

	EXPECT_EQ(ch.column_end(5) - ch.column_begin(5), ch.num_rows());
		
	// iterate over the last row
	count = 9;
	for (auto it = ch.column_begin(9); it != ch.column_end(9); it++) 
	{
		EXPECT_EQ(*it, count);
		count += static_cast<int8_t>(ch.num_columns());
	}

	// reverse iterator over the 9th column
	for (auto it = ch.const_rcolumn_begin(9); it != ch.const_rcolumn_end(9); it++)
	{
		count -= static_cast<int8_t>(ch.num_columns());
		EXPECT_EQ(*it, count);
	}

	EXPECT_EQ(ch.column_end(9) - ch.column_begin(9), ch.num_rows());
}

TEST(ChannelTests, Resize)
{
	auto ch = Channel<int>{ 3, 3, 1 };
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(ch.data(), ch.size(), 1));
	EXPECT_FALSE(ch.resize(3, 3));
	EXPECT_TRUE(ch.resize(5, 5));
	EXPECT_EQ(ch.size(), 25);
	EXPECT_EQ(ch.end() - ch.begin(), 25);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(ch.data(), ch.size(), 0));
}

TEST(ChannelTests, Copy_test1)
{
	auto ch1 = Channel<char>{ 17, 21, -1 };
	auto ch2 = Channel<char>{ 17, 21, -3 };
	ch2.copy(ch1);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(ch1.data(), ch1.size(), -1));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<char>(ch2.data(), ch2.size(), -1));

	auto ch3 = Channel<unsigned char>{ 16, 8, 4 };
	auto ch4 = Channel<unsigned char>{ 16, 8, 2 };
	ch4.copy(ch3);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<unsigned char>(ch3.data(), ch3.size(), 4));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<unsigned char>(ch4.data(), ch4.size(), 4));
	
	auto ch5 = Channel<int>{ 4, 4, 23 };
	auto ch6 = Channel<int>{ 4, 4, 32 };
	ch6.copy(ch5);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(ch5.data(), ch5.size(), 23));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<int>(ch6.data(), ch6.size(), 23));

	auto ch7 = Channel<float>{ 10, 8, 1.34 };
	auto ch8 = Channel<float>{ 10, 8, 4.56 };
	ch8.copy(ch7);
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(ch7.data(), ch7.size(), 1.34));
	EXPECT_TRUE(helpers::AllPixelsEqualTo<float>(ch8.data(), ch8.size(), 1.34));
}

TEST(ChannelTests, Copy_test2) 
{
	auto ch1 = Channel<char>{ 3, 5, -1 };
	auto ch2 = Channel<char>{ 3, 4, -3 };
	EXPECT_THROW(ch2.copy(ch1), std::invalid_argument);

	auto ch3 = Channel<char>{ 4, 5, -3 };
	EXPECT_THROW(ch3.copy(ch1), std::invalid_argument);
}