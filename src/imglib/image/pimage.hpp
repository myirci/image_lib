#pragma once

#include <memory>

#include <imglib/color/color.hpp>
#include <imglib/utility/utility.hpp>

namespace imglib 
{
    template <typename T, size_t NumChannels>
        requires (NumChannels >= 1)
    struct Pixel
    {
        Pixel()
        {
            for (size_t i = 0; i < NumChannels; i++)
                p[i] = T();
        }

        template <std::same_as<T> ...U>
            requires (sizeof...(U) == NumChannels)
        Pixel(U... args)
        {
            size_t i{ 0 };
            for (T arg : { args... })
                p[i++] = arg;
        }

        bool operator==(const Pixel<T, NumChannels>& other) const { return std::equal(p, p + NumChannels, other.p); }

        bool operator!=(const Pixel<T, NumChannels>& other) const { return !(this->operator==(other)); }

        T p[NumChannels];
    };

    template <typename T>
    class Pixelterator
    {
    public:
        using iterator_category = std::contiguous_iterator_tag;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;
        using difference_type = std::ptrdiff_t;
        using self = Pixelterator<T>;

        // Default constructible -> required by the std::forward_iterator concept
        Pixelterator() noexcept = default;

        // Constructor
        explicit Pixelterator(pointer ptr) noexcept : m_ptr{ ptr } { }

        // Dereferencable -> required by the std::input_or_output_iterator concept 
        // Note that although the pointee can be modified using the returned reference, m_ptr is not modified, therefore marked with const specifier. 
        reference operator*() const { return *m_ptr; }

        // Pre-incrementable -> required by the std::input_or_output_iterator concept 
        self& operator++() noexcept
        {
            ++m_ptr;
            return *this;
        }

        // Post-incrementable -> required by the std::input_or_output_iterator concept 
        self operator++(int)
        {
            auto copy = *this;
            ++m_ptr;
            return copy;
        }

        // Pre-decrementable -> required by the std::bidirectional_iterator concept 
        self& operator--() noexcept
        {
            --m_ptr;
            return *this;
        }

        // Post-decrementable -> required by the std::bidirectional_iterator concept 
        self& operator--(int)
        {
            auto copy = *this;
            --m_ptr;
            return copy;
        }

        // Equality -> required by the std::input_iterator concept 
        bool operator==(const self& other) const noexcept { return m_ptr == other.m_ptr; }

        // Inequality
        bool operator!=(const self& other) const noexcept { return m_ptr != other.m_ptr; }

        // Compound addition assignment -> required by the std::random_access_iterator concept
        self& operator+=(const difference_type n) noexcept
        {
            m_ptr += n;
            return *this;
        }

        // Compound substraction assignment -> required by the std::random_access_iterator concept
        self& operator-=(const difference_type n) noexcept
        {
            m_ptr -= n;
            return *this;
        }

        // Addition, substraction -> required by the std::random_access_iterator concept
        self operator+(const difference_type n) const noexcept { return self{ m_ptr + n }; }
        self operator-(const difference_type n) const noexcept { return self{ m_ptr - n }; }

        friend self operator+(const difference_type n, const self& it) noexcept { return it + n; }
        friend difference_type operator-(const self& it1, const self& it2) noexcept { return it1.m_ptr - it2.m_ptr; }

        // Subscripting -> required by the std::random_access_iterator concept
        reference operator[](const difference_type n) const { return *(m_ptr + n); }

        // Less than, less than or equal to, greather than, greater than or equal to -> required by the std::random_access_iterator concept
        bool operator<(const self& other) const noexcept { return m_ptr < other.m_ptr; }
        bool operator<=(const self& other) const noexcept { return m_ptr <= other.m_ptr; }
        bool operator>(const self& other) const noexcept { return m_ptr > other.m_ptr; }
        bool operator>=(const self& other) const noexcept { return m_ptr >= other.m_ptr; }

        // required by the std::contiguous_iterator concept
        const pointer operator->() const noexcept { return m_ptr; }

    private:
        pointer m_ptr{ nullptr };
    };

	template <typename T, size_t NumChannels>
		requires (NumChannels >= 1)
	class PImage 
	{
	public:
        using iterator = typename Pixelterator<Pixel<T, NumChannels>>;
        using const_iterator = typename Pixelterator<const Pixel<T, NumChannels>>;
        using reverse_iterator = typename std::reverse_iterator<iterator>;
        using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;
    
		// Default constructor
		PImage() = default;

		// Constructor
        explicit PImage(size_t height, size_t width, ColorSpace colorSpace, Pixel<T, NumChannels> px = Pixel<T, NumChannels>{}) :
			m_height{ height },
			m_width{ width },
			m_colorSpace{ colorSpace }
		{
			if (m_height < 1 || m_width < 1 || !IsValid(m_colorSpace, NumChannels))
				throw std::invalid_argument("At least one of the arguments is invalid.");	

			m_pixels = std::make_unique<Pixel<T, NumChannels>[]>(m_height * m_width);

            if (px != Pixel<T, NumChannels>{})
                *this = px;
		}

		// Copy constructor
		PImage(const PImage<T, NumChannels>& other) : m_height{ other.m_height }, m_width{ other.m_width }, m_colorSpace{ other.m_colorSpace }
		{
			auto sz = m_height * m_width;
			m_pixels = std::make_unique<Pixel<T, NumChannels>[]>(sz);
			std::memcpy(static_cast<void*>(m_pixels.get()), static_cast<void const*>(other.m_pixels.get()), sz * sizeof(Pixel<T, NumChannels>));
		}

		// Move constructor
		PImage(PImage<T, NumChannels>&& other) noexcept
		{
			*this = std::move(other);
		}

		// Copy assignment operator
		PImage<T, NumChannels>& operator=(const PImage<T, NumChannels>& other) 
		{
			if (this == &other)
				return *this;

			auto temp = other;
			*this = std::move(temp);
		}

		// Move assignment operator
		PImage<T, NumChannels>& operator=(PImage<T, NumChannels>&& other) noexcept
		{
			if (this == &other)
				return *this;

			m_height = other.m_height;
			m_width = other.m_width;
			m_colorSpace = other.m_colorSpace;
			m_pixels = std::move(other.m_pixels);
			other.clear();
			return *this;
		}

        PImage<T, NumChannels>& operator=(const Pixel<T, NumChannels>& px) 
        { 
            std::fill(m_pixels.get(), m_pixels.get() + size(), px);
            return *this;
        }

		Pixel<T, NumChannels>& operator()(size_t index) { return m_pixels[index]; }

		Pixel<T, NumChannels> const & operator()(size_t index) const { return m_pixels[index]; }

		Pixel<T, NumChannels>& operator()(size_t row, size_t col) { return m_pixels[row * m_width + col]; }

		Pixel<T, NumChannels> const& operator()(size_t row, size_t col) const { return m_pixels[row * m_width + col]; }

		size_t width() const noexcept { return m_width; }

		size_t height() const noexcept { return m_height; }

		constexpr size_t num_channels() const noexcept { return NumChannels; }

		ColorSpace color_space() const noexcept { return m_colorSpace; }

		size_t size() const noexcept { return m_width * m_height; }

		size_t data_size() const noexcept { return size() * NumChannels; }

		void clear() noexcept
		{
			m_height = 0;
			m_width = 0;
			m_pixels.reset();
		}

        // Itearators
        iterator begin() noexcept { return iterator{ m_pixels.get() }; }
        const_iterator begin() const noexcept { return cbegin(); }
        const_iterator cbegin() const noexcept { return const_iterator{ m_pixels.get() }; }

        iterator end() noexcept { return iterator{ m_pixels.get() + size() }; }
        const_iterator end() const noexcept { return cend(); }
        const_iterator cend() const noexcept { return const_iterator(m_pixels.get() + size()); }

        // Reverse iterators
        reverse_iterator rbegin() noexcept { return reverse_iterator{ end() }; }
        const_reverse_iterator rbegin() const noexcept { return crbegin(); }
        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator{ cend() }; }

        reverse_iterator rend() noexcept { return reverse_iterator{ begin() }; }
        const_reverse_iterator rend() const noexcept { return crend(); }
        const_reverse_iterator crend() const noexcept { return const_reverse_iterator{ cbegin() }; }

        // Row iterators
        iterator row_begin(size_t row) noexcept { return iterator{ m_pixels.get() + row * m_width }; }
        const_iterator row_begin(size_t row) const noexcept { return crow_begin(row); }
        const_iterator crow_begin(size_t row) const noexcept { return const_iterator{ m_pixels.get() + row * m_width }; }

        iterator row_end(size_t row) noexcept { return row_begin(row + 1); }
        const_iterator row_end(size_t row) const noexcept { return crow_begin(row + 1); }
        const_iterator crow_end(size_t row) const noexcept { return crow_begin(row + 1); }

        // Reverse row iterators
        reverse_iterator rrow_begin(size_t row) noexcept { return reverse_iterator{ row_end(row) }; }
        const_reverse_iterator rrow_begin(size_t row) const noexcept { return crrow_begin(row); }
        const_reverse_iterator crrow_begin(size_t row) const noexcept { return const_reverse_iterator{ crow_end(row) }; }

        reverse_iterator rrow_end(size_t row) noexcept { return reverse_iterator{ row_begin(row) }; }
        const_reverse_iterator rrow_end(size_t row) const noexcept { return crrow_end(row); }
        const_reverse_iterator crrow_end(size_t row) const noexcept { return const_reverse_iterator{ crow_begin(row) }; }

        // Generic iterator 
        iterator git(size_t row, size_t col) noexcept { return iterator{ m_pixels.get() + to_index(row, col) }; }
        const_iterator git(size_t row, size_t col) const noexcept { return cgit(row, col); }
        const_iterator cgit(size_t row, size_t col) const noexcept { return const_iterator{ m_pixels.get() + to_index(row, col) }; }

	private:
		size_t m_height{ 0 };
		size_t m_width{ 0 };
		ColorSpace m_colorSpace{ ColorSpace::Unspecified };
		std::unique_ptr<Pixel<T, NumChannels>[]> m_pixels{ nullptr };

        auto to_index(size_t row, size_t col) const noexcept { return row * m_width + col; }
	};
}