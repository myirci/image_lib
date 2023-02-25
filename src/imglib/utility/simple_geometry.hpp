#pragma once

#include <utility>

namespace imglib 
{
	enum class Orientation2D 
	{
		Horizontal,
		Vertical
	};

	template <typename T = size_t, size_t Dimension = 2u>
	class Point
	{
	public:
		Point()
		{
			for (size_t i = 0; i < Dimension; i++)
				coord[i] = T();
		}

		template < std::convertible_to<T> ... U>
			requires (sizeof...(U) == Dimension)
		Point(U... args)
		{
			size_t i{ 0 };
			for (T arg : { args... })
				coord[i++] = arg;
		}

		T operator()(size_t index) const { return coord[index]; }

	private:
		T coord[Dimension];
	};

	template <typename T = size_t>
	class Rectangle2D
	{
	public:

		using Point2D = Point<T, 2u>;

		Rectangle2D() = default;

		template <typename PointType>
			requires std::same_as<typename std::remove_cvref_t<PointType>, Point2D>
		Rectangle2D(PointType&& topLeft, PointType&& bottomRight) :
			m_topLeft{ std::forward<PointType>(topLeft) },
			m_bottomRight{ std::forward<PointType>(bottomRight) }
		{
			m_height = m_bottomRight(0) - m_topLeft(0) + 1;
			m_width = m_bottomRight(1) - m_topLeft(1) + 1;

			if (m_height <= 0 || m_width <= 0)
				throw std::invalid_argument();
		}

		template <typename PointType>
			requires std::same_as<typename std::remove_cvref_t<PointType>, Point2D>
		Rectangle2D(PointType&& topLeft, T height, T width) :
			m_topLeft{ std::forward<PointType>(topLeft) },
			m_height{ height },
			m_width{ width }
		{
			m_bottomRight = Point2D{ m_topLeft(0) + m_height - 1, m_topLeft(1) + m_width - 1};
		}

		T height() const noexcept { return m_height; }
		T width() const noexcept { return m_width; }
		const Point2D& top_left() const noexcept { return m_topLeft; }
		const Point2D& bottom_right() const noexcept { return m_bottomRight; }

	private:

		T m_height{ T{} };
		T m_width{ T{} };
		Point2D m_topLeft;
		Point2D m_bottomRight;
	};
}
