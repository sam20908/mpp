/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#pragma once

// Buffer types
// clang-format off
#include <array>
#include <vector>
// clang-format on

#include <algorithm>   // std::ranges::transform, std::rotate
#include <functional>  // std::bind_front
#include <iostream>    // std::cout
#include <iterator>    // std::iter_swap
#include <limits>      // std::numeric_limits
#include <ranges>      // std::ranges::range_value_t
#include <span>        // std::dynamic_extent
#include <stdexcept>   // std::runtime_error
#include <type_traits> // std::is_arithmetic

namespace matrixpp
{
	template<typename Element>
	concept arithmetic = std::is_arithmetic_v<Element>;

	template<typename Range>
	concept range_2d = std::ranges::range<std::ranges::range_value_t<Range>> &&
					   // Verify that the elements aren't a range, otherwise we'd have a 3D+ range
					   !std::ranges::range<std::ranges::range_value_t<std::ranges::range_value_t<Range>>>;

	enum class matrix_type
	{
		fully_static,
		fully_dynamic,
		dynamic_rows,
		dynamic_columns
	};

	struct matrix_reserve_only_tag
	{
	};

	inline constexpr matrix_reserve_only_tag matrix_reserve_only{};

	namespace detail
	{
		template<typename>
		struct is_vector : std::false_type
		{
		};

		template<typename Element>
		struct is_vector<std::vector<Element>> : std::true_type
		{
		};

		template<typename Type>
		inline constexpr auto is_vector_v = is_vector<Type>::value;

		struct identity_matrix_impl_tag
		{
		};

		struct perfect_forward_range_constructor_tag
		{
		};

		inline constexpr identity_matrix_impl_tag identity_matrix_impl{};
		inline constexpr perfect_forward_range_constructor_tag perfect_forward_range_constructor{};

		template<typename Derived, typename Element>
		class expr_base
		{
			// @TODO: Uncomment out constexpr specifier when constexpr std::vector ships

			/* constexpr */ decltype(auto) derived() const
			{
				return static_cast<const Derived&>(*this);
			}

		public:
			using element_t = Element;

			// @TODO: Uncomment out constexpr specifier when constexpr std::vector ships

			[[nodiscard]] /* constexpr */ decltype(auto) data() const
			{
				return derived().data();
			}

			[[nodiscard]] /* constexpr */ auto rows() const
			{
				return derived().rows();
			}

			[[nodiscard]] /* constexpr */ auto columns() const
			{
				return derived().columns();
			}

			[[nodiscard]] /* constexpr */ decltype(auto) at(std::size_t row_index, std::size_t column_index) const
			{
				if (row_index >= rows() || column_index >= columns())
				{
					throw std::out_of_range("Access out of range!");
				}

				return derived().at(row_index, column_index);
			}

			[[nodiscard]] /* constexpr */ decltype(auto) operator()(std::size_t row_index,
				std::size_t column_index) const
			{
				return derived()(row_index, column_index);
			}
		};

		template<typename Left, typename Right, typename Op>
		class expr_op : public expr_base<expr_op<Left, Right, Op>, typename Left::element_t>
		{
			const Left& _lhs;
			const Right& _rhs;
			const Op& _op;
			std::size_t _result_rows;
			std::size_t _result_columns;

		public:
			// @TODO: Uncomment out constexpr specifier when constexpr std::vector ships

			/* constexpr */ explicit expr_op(const Left& lhs,
				const Right& rhs,
				const Op& op,
				std::size_t result_rows,
				std::size_t result_columns) :
				_lhs(lhs),
				_rhs(rhs),
				_op(op),
				_result_rows(result_rows),
				_result_columns(result_columns)
			{
			}

			[[nodiscard]] /* constexpr */ auto rows() const
			{
				return _result_rows;
			}

			[[nodiscard]] /* constexpr */ auto columns() const
			{
				return _result_columns;
			}

			[[nodiscard]] /* constexpr */ decltype(auto) at(std::size_t row_index, std::size_t column_index) const
			{
				if (row_index >= _result_rows || column_index >= _result_columns)
				{
					throw std::out_of_range("Access out of range!");
				}

				return _op(_lhs, _rhs, row_index, column_index);
			}

			[[nodiscard]] /* constexpr */ decltype(auto) operator()(std::size_t row_index,
				std::size_t column_index) const
			{
				return _op(_lhs, _rhs, row_index, column_index);
			}
		};

		template<typename Object, typename Constant, typename Op>
		class expr_constant_op : public expr_base<expr_constant_op<Object, Constant, Op>, typename Object::element_t>
		{
			const Object& _object;
			Constant _constant; // Store the constant by value because of lifetime issues
			const Op& _op;
			std::size_t _result_rows;
			std::size_t _result_columns;

		public:
			// @TODO: Uncomment out constexpr specifier when constexpr std::vector ships

			/* constexpr */ explicit expr_constant_op(const Object& object, Constant constant, const Op& op) :
				_object(object),
				_constant(std::move(constant)),
				_op(op),
				// We know that operands of a matrix and a scalar results in the same
				// size matrix
				_result_rows(object.rows()),
				_result_columns(object.columns())
			{
			}

			[[nodiscard]] /* constexpr */ std::size_t rows() const
			{
				return _result_rows;
			}

			[[nodiscard]] /* constexpr */ std::size_t columns() const
			{
				return _result_columns;
			}

			[[nodiscard]] /* constexpr */ decltype(auto) at(std::size_t row_index, std::size_t column_index) const
			{
				if (row_index >= _result_rows || column_index >= _result_columns)
				{
					throw std::out_of_range("Access out of range!");
				}

				return _op(_object, _constant, row_index, column_index);
			}

			[[nodiscard]] /* constexpr */ decltype(auto) operator()(std::size_t row_index,
				std::size_t column_index) const
			{
				return _op(_object, _constant, row_index, column_index);
			}
		};

		constexpr void validate_dimension(std::size_t rows, std::size_t columns)
		{
			if ((rows == 0 && columns != 0) || (rows != 0 && columns == 0))
			{
				throw std::invalid_argument("Matrices cannot have one side being zero and "
											"other side being non-zero!");
			}
		}

		[[nodiscard]] constexpr auto range_2d_columns(auto&& rng)
		{
			auto rng_size = std::ranges::size(rng);

			return rng_size == 0 ? 0 : rng.begin()->size();
		}

		// @TODO: Uncomment out constexpr specifier when constexpr std::vector ships

		inline /* constexpr */ void validate_matrices_same_size(const auto& lhs, const auto& rhs)
		{
			if (lhs.rows() != rhs.rows() || lhs.columns() != rhs.columns())
			{
				throw std::runtime_error("Both matrices don't have the same size!");
			}
		}

		inline /* constexpr */ void validate_matrices_multipliable(const auto& lhs, const auto& rhs)
		{
			if (lhs.columns() != rhs.rows())
			{
				throw std::runtime_error("Left matrix's columns is not equal to right matrix's rows!");
			}
		}

		[[nodiscard]] constexpr std::size_t
		index_2d_to_1d(std::size_t columns, std::size_t row_index, std::size_t column_index)
		{
			// This is mainly for avoiding bug-prone code, because this calculation occurs
			// in a lot of places, and a typo can cause a lot of things to fail. It's
			// safer to wrap this calculation in a function, so the bug is easier to spot

			return row_index * columns + column_index;
		}

		template<typename Precision>
		[[nodiscard]] constexpr Precision determinant_impl(auto& data,
			std::size_t rows,
			std::size_t columns,
			std::size_t row_begin,
			std::size_t column_end)
		{
			// The size of the data stays the same, so we have to determine the actual
			// size of the data we're allowed to work with by using column_end
			auto columns_range = column_end + 1;

			if (columns_range == 0)
			{
				return static_cast<Precision>(1);
			}
			else if (columns_range == 1)
			{
				// We don't have to worry about row_begin because this case won't happen
				// unless the *starting* matrix is 1x1

				return static_cast<Precision>(data[0]);
			}
			else if (columns_range == 2)
			{
				// We start at row_begin because that's the range of rows we're working
				// with. This matters when we're working with matrices bigger than 2x2
				// because the 2x2 matrix in there doesn't start at the top, and row_begin
				// helps defining the index which the inner matrix is intended to start

				// Columns aren't affected by this, because the way we rearrange the row
				// elements, the intended elements always start at the beginning

				auto top_left_index     = index_2d_to_1d(columns, row_begin, 0);
				auto top_right_index    = index_2d_to_1d(columns, row_begin, 1);
				auto bottom_left_index  = index_2d_to_1d(columns, row_begin + 1, 0);
				auto bottom_right_index = index_2d_to_1d(columns, row_begin + 1, 1);

				auto ad =
					static_cast<Precision>(data[top_left_index]) * static_cast<Precision>(data[bottom_right_index]);
				auto bc =
					static_cast<Precision>(data[top_right_index]) * static_cast<Precision>(data[bottom_left_index]);

				return ad - bc;
			}
			else
			{
				auto result = static_cast<Precision>(0);
				auto sign   = static_cast<Precision>(1);

				// 3x3 and higher. Use recursion

				/*
				* Instead of manually recreating the entire inner matrix for calculation,
				* we can just rotate and swap the elements of the untrimmed inner matrix
				* (the matrix which we haven't "cut off" a column). With this method, we
				* can just ignore the last column of this untrimmed matrix. Here is an
				* example of this in action:
				*
				* Suppose we start with this 4x4 matrix:
				* [1 2 3 4]
				* [5 6 7 8]
				* [9 8 7 6]
				* [5 4 3 2]
				*
				* We can already cut off the top row in trivial fashion by using row_begin,
				* so we can start off by cutting the first column. We're just going to work
				* with this matrix where the top row got cut off: [5 6 7 8] [9 8 7 6] [5 4
				* 3 2]
				*
				* Cutting off the first column would *always* start by rotating the row
				* left. This can be achieved by using `std::rotate`: [6 7 8 5] [8 7 6 9] [4
				* 3 2 5]
				*
				* Then ignoring the second column would be swapping the first element and
				* the last element. [5 7 8 6] [9 7 6 8] [5 3 2 4]
				*
				* Ignoring the third column would be swapping the second element and the
				* last element. [5 6 8 7] [9 8 6 7] [5 4 2 3]
				*
				* Igoring the fourth column would be swapping the third element and the
				* last element. This would also go back to where we started! [5 6 7 8] [9 8
				* 7 6] [5 4 3 2]
				*
				* This is important because we guarantee that all minor determinants are
				* based on their "base" matrix, and the base matrix's values are back to
				* where it should be. Suprisingly, this also guarantees that all minor
				* determinant calculations are going to restore the base matrix back to
				* where it should be
				*/

				for (auto column_index = std::size_t{ 0 }; column_index <= column_end; ++column_index)
				{
					if (column_index == 0)
					{
						for (auto row_index = row_begin + 1; row_index < rows; ++row_index)
						{
							auto row_begin_index = index_2d_to_1d(columns, row_index, 0);

							auto row_begin          = std::next(data.begin(), row_begin_index);
							auto row                = std::views::counted(row_begin, column_end + 1);
							auto row_second_element = std::next(row_begin, 1);

							std::ranges::rotate(row, row_second_element);
						}
					}
					else
					{
						for (auto row_index = row_begin + 1; row_index < rows; ++row_index)
						{
							auto row_begin_index = index_2d_to_1d(columns, row_index, 0);

							auto row_begin        = std::next(data.begin(), row_begin_index);
							auto left_element_it  = std::next(row_begin, column_index - 1);
							auto right_element_it = std::next(row_begin, column_end);

							std::iter_swap(left_element_it, right_element_it);
						}
					}

					// We effectively "shrink" the working range in the recursion call by
					// making the minor start at the row below the current row_begin and
					// "removing" the last column
					auto minor_determinant =
						determinant_impl<Precision>(data, rows, columns, row_begin + 1, column_end - 1);

					auto coefficient_index = index_2d_to_1d(columns, row_begin, column_index);

					result += static_cast<Precision>(data[coefficient_index]) * minor_determinant * sign;
					sign *= -1;
				}

				return result;
			}
		}

		template<typename Precision, bool Vector>
		inline /* constexpr */ void
		inverse_impl(auto& result, auto& data, std::size_t rows, std::size_t columns, Precision determinant)
		{
			auto& result_data = result.data();

			if (rows == 0)
			{
				result_data[0] = static_cast<Precision>(data[0]);
			}
			else if (rows == 1)
			{
				result_data[0] = static_cast<Precision>(1) / static_cast<Precision>(data[0]);
			}
			else if (rows == 2)
			{
				auto element_1 = static_cast<Precision>(data[3]);
				auto element_2 = static_cast<Precision>(data[1]) * -1;
				auto element_3 = static_cast<Precision>(data[2]) * -1;
				auto element_4 = static_cast<Precision>(data[0]);

				if constexpr (Vector)
				{
					result_data.push_back(std::move(element_1));
					result_data.push_back(std::move(element_2));
					result_data.push_back(std::move(element_3));
					result_data.push_back(std::move(element_4));
				}
				else
				{
					result_data[0] = std::move(element_1);
					result_data[1] = std::move(element_2);
					result_data[2] = std::move(element_3);
					result_data[3] = std::move(element_4);
				}

				auto multiplier = static_cast<Precision>(1) / determinant;

				result *= multiplier;
			}
			else
			{
				/**
				* The swapping and rotating logic works a lot like `determinant_impl`'s
				* logic, except that we put the minor matrix at the top left corner and
				* call `determinant_impl` to calucate the determinant of that minor matrix.
				* We do this because We ignore the values on the current row and column
				*
				* We use the same way to rotate and swap the columns, except that we do it
				* to the rows as well
				*/

				// First step: Compute matrix of minors

				for (auto row_index = std::size_t{ 0 }; row_index < rows; ++row_index)
				{
					if (row_index == 0)
					{
						auto second_row_first_element_it = std::next(data.begin(), columns);

						std::ranges::rotate(data, second_row_first_element_it);
					}
					else
					{
						auto row_n_begin_index    = (row_index - 1) * columns;
						auto last_row_begin_index = (columns - 1) * columns;

						auto row_n_begin    = std::next(data.begin(), row_n_begin_index);
						auto last_row_begin = std::next(data.begin(), last_row_begin_index);

						auto row_n    = std::views::counted(row_n_begin, columns);
						auto last_row = std::views::counted(last_row_begin, columns);

						std::ranges::swap_ranges(row_n, last_row);
					}

					for (auto column_index = std::size_t{ 0 }; column_index < columns; ++column_index)
					{
						if (column_index == 0)
						{
							for (auto minor_row_index = std::size_t{ 0 }; minor_row_index < rows - 1; ++minor_row_index)
							{
								auto minor_row_begin_index = index_2d_to_1d(columns, minor_row_index, 0);

								auto minor_row_begin          = std::next(data.begin(), minor_row_begin_index);
								auto minor_row                = std::views::counted(minor_row_begin, columns);
								auto minor_row_second_element = std::next(minor_row_begin, 1);

								std::ranges::rotate(minor_row, minor_row_second_element);
							}
						}
						else
						{
							for (auto minor_row_index = std::size_t{ 0 }; minor_row_index < rows - 1; ++minor_row_index)
							{
								auto minor_row_begin_index = index_2d_to_1d(columns, minor_row_index, 0);

								auto minor_row_begin        = std::next(data.begin(), minor_row_begin_index);
								auto minor_left_element_it  = std::next(minor_row_begin, column_index - 1);
								auto minor_right_element_it = std::next(minor_row_begin, columns - 1);

								std::iter_swap(minor_left_element_it, minor_right_element_it);
							}
						}

						auto minor_determinant = determinant_impl<Precision>(data, rows, columns, 0, columns - 2);

						if constexpr (Vector)
						{
							result_data.push_back(std::move(minor_determinant));
						}
						else
						{

							auto result_index = index_2d_to_1d(columns, row_index, column_index);

							result_data[result_index] = minor_determinant;
						}
					}
				}

				// Second step: Apply a "checkerboard" pattern to the matrix of minors

				std::ranges::for_each(result_data, [sign = static_cast<Precision>(1)](auto& element) mutable {
					element *= sign;
					sign *= -1;
				});

				// Third step: Transpose the matrix of cofactors to its adjugate

				for (auto row_index = std::size_t{ 0 }; row_index < rows; ++row_index)
				{
					for (auto column_index = std::size_t{ 0 }; column_index < row_index + 1; ++column_index)
					{
						if (row_index == column_index)
						{
							continue;
						}

						auto upper_element_index = index_2d_to_1d(columns, row_index, column_index);
						auto lower_element_index = index_2d_to_1d(columns, column_index, row_index);

						auto upper_element_it = std::next(result_data.begin(), upper_element_index);
						auto lower_element_it = std::next(result_data.begin(), lower_element_index);

						std::iter_swap(upper_element_it, lower_element_it);
					}
				}

				// Final step: Multiply by 1 / determinant of the original matrix

				// We already obtained the determinant when we calculated the matrix of
				// cofactors

				auto determinant = static_cast<Precision>(0);

				for (auto row_index = std::size_t{ 0 }; row_index < rows; ++row_index)
				{
					// The cofactors has been flipped by the previous step
					auto cofactor_index = index_2d_to_1d(columns, row_index, 0);

					determinant += result_data[cofactor_index] * static_cast<Precision>(data[row_index]);
				}

				auto multiplier = static_cast<Precision>(1) / determinant;

				result *= multiplier;
			}
		}

		template<std::size_t Rows, std::size_t Columns>
		[[nodiscard]] constexpr matrix_type get_matrix_type()
		{
			constexpr auto row_is_dynamic    = Rows == std::dynamic_extent;
			constexpr auto column_is_dynamic = Columns == std::dynamic_extent;

			if constexpr (!row_is_dynamic && !column_is_dynamic)
			{
				return matrix_type::fully_static;
			}
			else if constexpr (row_is_dynamic && column_is_dynamic)
			{
				return matrix_type::fully_dynamic;
			}
			else if constexpr (row_is_dynamic && !column_is_dynamic)
			{
				return matrix_type::dynamic_rows;
			}
			else
			{
				return matrix_type::dynamic_columns;
			}
		}

		inline auto same_size_add_op =
			[](const auto& lhs, const auto& rhs, std::size_t row_index, std::size_t column_index) {
				return lhs(row_index, column_index) + rhs(row_index, column_index);
			};

		inline auto same_size_minus_op =
			[](const auto& lhs, const auto& rhs, std::size_t row_index, std::size_t column_index) {
				return lhs(row_index, column_index) - rhs(row_index, column_index);
			};

		inline auto scalar_multiply_op =
			[](const auto& lhs, const auto& rhs, std::size_t row_index, std::size_t column_index) {
				return lhs(row_index, column_index) * rhs;
			};

		inline auto scalar_divide_op =
			[](const auto& lhs, const auto& rhs, std::size_t row_index, std::size_t column_index) {
				return lhs(row_index, column_index) / rhs;
			};

		inline auto matrix_multiply_op =
			[](const auto& lhs, const auto& rhs, std::size_t row_index, std::size_t column_index) {
				using result_t = typename std::remove_cvref_t<decltype(lhs)>::element_t;

				auto lhs_columns = lhs.columns();
				auto result      = result_t{ 0 };

				for (auto index = std::size_t{ 0 }; index < lhs_columns; ++index)
				{
					result += lhs(row_index, index) * rhs(index, column_index);
				}

				return result;
			};
	} // namespace detail

	template<arithmetic Element, std::size_t Rows = std::dynamic_extent, std::size_t Columns = std::dynamic_extent>
	class matrix : public detail::expr_base<matrix<Element, Rows, Columns>, Element>
	{
	public:
		using element_t = Element;
		using buffer_t  = std::conditional_t<Rows == std::dynamic_extent || Columns == std::dynamic_extent,
            std::vector<Element>,
            std::array<Element, Rows * Columns>>;

	protected:
		static constexpr auto _buffer_is_vector = detail::is_vector_v<buffer_t>;

		std::size_t _rows{ (Rows == std::dynamic_extent ? 0 : Rows) };
		std::size_t _columns{ (Columns == std::dynamic_extent ? 0 : Columns) };

		mutable buffer_t _buffer{};

		template<range_2d Range2D>
		/* constexpr */ matrix(Range2D&& rng, detail::perfect_forward_range_constructor_tag) :
			matrix(std::ranges::size(rng), detail::range_2d_columns(rng), matrix_reserve_only)
		{
			auto rng_rows    = std::ranges::size(rng);
			auto rng_columns = detail::range_2d_columns(rng);

			if (rng_rows > 1)
			{
				std::ranges::for_each(rng, [&](auto&& row_rng) {
					auto current_row_columns = std::ranges::size(row_rng);

					if (current_row_columns != rng_columns)
					{
						throw std::runtime_error("Matrix columns aren't the same size!");
					}
				});
			}

			std::ranges::for_each(rng, [&, row_index = 0](auto&& row_rng) mutable {
				auto buffer_begin = std::next(_buffer.begin(), row_index * _columns);

				if constexpr (_buffer_is_vector)
				{
					std::ranges::copy(row_rng, std::back_inserter(_buffer));
				}
				else
				{
					std::ranges::copy(row_rng, buffer_begin);
				}

				// Fill the remaining empty spot in the current row with 0 if necessary
				for (std::size_t column_index = rng_columns; column_index < _columns; ++column_index)
				{
					if constexpr (_buffer_is_vector)
					{
						_buffer.push_back(0);
					}
					else
					{
						auto column_index_1d = detail::index_2d_to_1d(_columns, row_index, column_index);

						_buffer[column_index_1d] = 0;
					}
				}

				++row_index;
			});
		}

	public:
		// @TODO: Uncomment out constexpr specifier when constexpr std::vector ships

		/* constexpr */ matrix(const matrix&) = default;
		/* constexpr */ matrix(matrix&&)      = default;

		/* constexpr */ matrix()
		{
			detail::validate_dimension(_rows, _columns);
		}

		/* constexpr */ explicit matrix(std::size_t rows, std::size_t columns, matrix_reserve_only_tag)
		{
			detail::validate_dimension(rows, columns);

			static constexpr bool rows_dynamic    = Rows == std::dynamic_extent;
			static constexpr bool columns_dynamic = Columns == std::dynamic_extent;

			if constexpr (!rows_dynamic)
			{
				if (Rows < rows)
				{
					throw std::runtime_error("Compile time specified rows smaller than rows of initializer!");
				}
			}

			if constexpr (!columns_dynamic)
			{
				if (Columns < columns)
				{
					throw std::runtime_error("Compile time specified columns smaller than "
											 "columns of initializer!");
				}
			}

			_rows    = rows_dynamic ? rows : Rows;
			_columns = columns_dynamic ? columns : Columns;

			if constexpr (_buffer_is_vector)
			{
				_buffer.reserve(_rows * _columns);
			}
		}

		/* constexpr */ explicit matrix(std::size_t rows, std::size_t columns, Element default_value) :
			matrix(rows, columns, matrix_reserve_only)
		{
			// Avoid going through the buffer twice by conditionally filling the value
			// inside the buffer
			for (auto row_index = std::size_t{ 0 }; row_index < _rows; ++row_index)
			{
				for (auto column_index = std::size_t{ 0 }; column_index < _columns; ++column_index)
				{
					auto within_dimension = row_index < rows && column_index < columns;
					auto value            = within_dimension ? default_value : Element{ 0 };

					if constexpr (_buffer_is_vector)
					{
						_buffer.push_back(std::move(value));
					}
					else
					{
						auto index = detail::index_2d_to_1d(_columns, row_index, column_index);

						_buffer[index] = std::move(value);
					}
				}
			}
		}

		/* constexpr */ matrix(std::initializer_list<std::initializer_list<Element>> init_list) :
			matrix(std::move(init_list), detail::perfect_forward_range_constructor)
		{
		}

		/* constexpr */ matrix(std::array<std::array<Element, Columns>, Rows> array_2d) requires(
			Rows != std::dynamic_extent && Columns != std::dynamic_extent) :
			// ^ requires clause here is to ensure that the compiler doesn't pick
			// the default dynamic size
			matrix(std::move(array_2d), detail::perfect_forward_range_constructor)
		{
		}

		template<range_2d Range2D>
		/* constexpr */ matrix(Range2D&& rng) :
			matrix(std::forward<Range2D>(rng), detail::perfect_forward_range_constructor)
		{
		}

		template<typename Type>
		/* constexpr */ matrix(const detail::expr_base<Type, Element>& expr) :
			matrix(expr.rows(), expr.columns(), matrix_reserve_only)
		{
			auto expr_rows    = expr.rows();
			auto expr_columns = expr.columns();

			// Force evaluation of the entire matrix expression
			for (auto row_index = std::size_t{ 0 }; row_index < _rows; ++row_index)
			{
				for (auto column_index = std::size_t{ 0 }; column_index < _columns; ++column_index)
				{
					auto buffer_index = detail::index_2d_to_1d(_columns, row_index, column_index);

					if (row_index < expr_rows && column_index < expr_columns)
					{
						if constexpr (_buffer_is_vector)
						{
							_buffer.push_back(expr(row_index, column_index));
						}
						else
						{
							_buffer[buffer_index] = expr(row_index, column_index);
						}
					}
					else
					{
						if constexpr (_buffer_is_vector)
						{
							_buffer.push_back(Element{ 0 });
						}
						else
						{
							_buffer[buffer_index] = Element{ 0 };
						}
					}
				}
			}
		}

		[[nodiscard]] /* constexpr */ buffer_t& data()
		{
			return _buffer;
		}

		[[nodiscard]] /* constexpr */ const buffer_t& data() const
		{
			return _buffer;
		}

		[[nodiscard]] /* constexpr */ decltype(auto) begin()
		{
			return _buffer.begin();
		}

		[[nodiscard]] /* constexpr */ decltype(auto) begin() const
		{
			return _buffer.cbegin();
		}

		[[nodiscard]] /* constexpr */ decltype(auto) end()
		{
			return _buffer.end();
		}

		[[nodiscard]] /* constexpr */ decltype(auto) end() const
		{
			return _buffer.cend();
		}

		[[nodiscard]] /* constexpr */ decltype(auto) cbegin()
		{
			return _buffer.cbegin();
		}

		[[nodiscard]] /* constexpr */ decltype(auto) cbegin() const
		{
			return _buffer.cbegin();
		}

		[[nodiscard]] /* constexpr */ decltype(auto) cend()
		{
			return _buffer.cend();
		}

		[[nodiscard]] /* constexpr */ decltype(auto) cend() const
		{
			return _buffer.cend();
		}

		[[nodiscard]] /* constexpr */ matrix_type type() const
		{
			return detail::get_matrix_type<Rows, Columns>();
		}

		[[nodiscard]] /* constexpr */ std::size_t rows() const
		{
			return _rows;
		}

		[[nodiscard]] /* constexpr */ std::size_t columns() const
		{
			return _columns;
		}

		[[nodiscard]] /* constexpr */ const Element& at(std::size_t row_index, std::size_t column_index) const
		{
			if (row_index >= _rows || column_index >= _columns)
			{
				throw std::out_of_range("Access out of bounds!");
			}

			auto index = detail::index_2d_to_1d(_columns, row_index, column_index);

			return _buffer[index];
		}

		[[nodiscard]] /* constexpr */ bool square() const
		{
			return _rows == _columns;
		}

		[[nodiscard]] /* constexpr */ decltype(auto) row(std::size_t row_index) const
		{
			if (row_index >= _rows)
			{
				throw std::out_of_range("Row index accesses out of bounds!");
			}

			auto row_begin_index = detail::index_2d_to_1d(_columns, row_index, 0);
			auto row_begin       = std::next(_buffer.begin(), row_begin_index);

			return std::views::counted(row_begin, _columns);
		}

		[[nodiscard]] /* constexpr */ decltype(auto) column(std::size_t column_index) const
		{
			if (column_index >= _columns)
			{
				throw std::out_of_range("Column index accesses out of bounds!");
			}

			return _buffer | std::views::filter([=, this, index = 0](const auto&) mutable {
				return index++ % _columns == column_index;
			});
		}

		template<typename Precision = Element>
		[[nodiscard]] /* constexpr */ auto determinant() const
		{
			if (!square())
			{
				throw std::runtime_error("Cannot find determinant of a non-square matrix!");
			}

			return detail::determinant_impl<Precision>(_buffer, _rows, _columns, 0, _columns - 1);
		}

		template<typename Precision = Element>
		[[nodiscard]] /* constexpr */ decltype(auto) inverse() const
		{
			if (!square())
			{
				throw std::runtime_error("Inverse of a non-square matrix doesn't exist!");
			}

			auto result      = matrix<Precision, Rows, Columns>{ _rows, _columns, matrix_reserve_only };
			auto determinant = detail::determinant_impl<Precision>(_buffer, _rows, _columns, 0, _columns - 1);

			if (determinant == static_cast<Precision>(0))
			{
				throw std::runtime_error("Inverse of a singular matrix doesn't exist!");
			}

			detail::inverse_impl<Precision, _buffer_is_vector>(result, _buffer, _rows, _columns, determinant);

			return result;
		}

		[[nodiscard]] /* constexpr */ decltype(auto) transpose() const
		{
			using matrix_t = matrix<Element, Columns, Rows>;

			auto result       = matrix_t{ _columns, _rows, matrix_reserve_only };
			auto& result_data = result.data();

			for (auto column_index = std::size_t{ 0 }; column_index < _columns; ++column_index)
			{
				for (auto row_index = std::size_t{ 0 }; row_index < _rows; ++row_index)
				{
					auto index = detail::index_2d_to_1d(_columns, row_index, column_index);
					auto value = _buffer[index];

					if constexpr (_buffer_is_vector)
					{
						result_data.push_back(std::move(value));
					}
					else
					{
						auto result_index = detail::index_2d_to_1d(_rows, column_index, row_index);

						result_data[result_index] = std::move(value);
					}
				}
			}

			return result;
		}

		[[nodiscard]] /* constexpr */ bool singular() const
		{
			return determinant() == 0.0L;
		}

		[[nodiscard]] /* constexpr */ decltype(auto) block(std::size_t top_row_index,
			std::size_t top_column_index,
			std::size_t bottom_row_index,
			std::size_t bottom_column_index) const
		{
			// Out of bounds checks

			if (top_row_index >= _rows)
			{
				throw std::invalid_argument("Top row index out of bounds!");
			}

			if (top_column_index >= _columns)
			{
				throw std::invalid_argument("Top column index out of bounds!");
			}

			if (bottom_row_index >= _rows)
			{
				throw std::invalid_argument("Bottom row index out of bounds!");
			}

			if (bottom_column_index >= _columns)
			{
				throw std::invalid_argument("Bottom column index out of bounds!");
			}

			// Overlapping checks

			if (top_row_index > bottom_row_index)
			{
				throw std::invalid_argument("Top row index bigger than bottom row index!");
			}

			if (top_column_index > bottom_column_index)
			{
				throw std::invalid_argument("Top column index bigger than bottom column index!");
			}

			auto new_block_rows    = bottom_row_index - top_row_index + 1;
			auto new_block_columns = bottom_column_index - top_column_index + 1;

			using matrix_t = matrix<Element, std::dynamic_extent, std::dynamic_extent>;
			auto result    = matrix_t{ new_block_rows, new_block_columns, 0 };

			for (auto row_index = std::size_t{ 0 }; row_index < new_block_rows; ++row_index)
			{
				auto row_range_begin_index =
					detail::index_2d_to_1d(_columns, top_row_index + row_index, top_column_index);
				auto block_buffer_begin_index = detail::index_2d_to_1d(new_block_columns, row_index, 0);

				auto row_range_begin    = std::next(_buffer.begin(), row_range_begin_index);
				auto block_buffer_begin = std::next(result.begin(), block_buffer_begin_index);

				std::ranges::copy_n(row_range_begin, new_block_columns, block_buffer_begin);
			}

			return result;
		}

		template<typename ConvertedType>
		[[nodiscard]] /* constexpr */ decltype(auto) cast() const
		{
			using matrix_t = matrix<ConvertedType, Rows, Columns>;

			auto result = matrix_t{ _rows, _columns, matrix_reserve_only };

			if constexpr (_buffer_is_vector)
			{
				std::ranges::transform(_buffer, std::back_inserter(result.data()), [](const auto& element) {
					return static_cast<ConvertedType>(element);
				});
			}
			else
			{
				std::ranges::transform(_buffer, result.begin(), [](const auto& element) {
					return static_cast<ConvertedType>(element);
				});
			}

			return result;
		}

		void print() const
		{
			constexpr auto type_precision = std::numeric_limits<Element>::max_digits10;
			auto precision_before         = std::cout.precision();

			std::cout.precision(type_precision);

			std::ranges::for_each(_buffer, [&, index = 1](const auto& element) mutable {
				std::cout << std::fixed << element << (index++ % _columns == 0 ? '\n' : ' ');
			});

			std::cout.precision(precision_before);
		}

		[[nodiscard]] /* constexpr */ Element& operator()(std::size_t row_index, std::size_t column_index)
		{
			auto index = detail::index_2d_to_1d(_columns, row_index, column_index);

			return _buffer[index];
		}

		[[nodiscard]] /* constexpr */ const Element& operator()(std::size_t row_index, std::size_t column_index) const
		{
			auto index = detail::index_2d_to_1d(_columns, row_index, column_index);

			return _buffer[index];
		}
	};

	// @TODO: Uncomment out constexpr specifier when constexpr std::vector ships

	template<typename LeftBase, typename RightBase, typename Element>
	[[nodiscard]] /* constexpr */ decltype(auto) operator+(const detail::expr_base<LeftBase, Element>& lhs,
		const detail::expr_base<RightBase, Element>& rhs)
	{
		detail::validate_matrices_same_size(lhs, rhs);

		return detail::expr_op{ lhs, rhs, detail::same_size_add_op, lhs.rows(), lhs.columns() };
	}

	template<typename LeftBase, typename RightBase, typename Element>
	[[nodiscard]] /* constexpr */ decltype(auto) operator-(const detail::expr_base<LeftBase, Element>& lhs,
		const detail::expr_base<RightBase, Element>& rhs)
	{
		detail::validate_matrices_same_size(lhs, rhs);

		return detail::expr_op{ lhs, rhs, detail::same_size_minus_op, lhs.rows(), lhs.columns() };
	}

	template<typename Base, typename Element>
	[[nodiscard]] /* constexpr */ decltype(auto) operator*(const detail::expr_base<Base, Element>& object,
		Element constant)
	{
		return detail::expr_constant_op{ object, std::move(constant), detail::scalar_multiply_op };
	}

	template<typename Base, typename Element>
	[[nodiscard]] /* constexpr */ decltype(auto) operator*(Element constant,
		const detail::expr_base<Base, Element>& object)
	{
		return detail::expr_constant_op{ object, std::move(constant), detail::scalar_multiply_op };
	}

	template<typename LeftBase, typename RightBase, typename Element>
	[[nodiscard]] /* constexpr */ decltype(auto) operator*(const detail::expr_base<LeftBase, Element>& lhs,
		const detail::expr_base<RightBase, Element>& rhs)
	{
		detail::validate_matrices_multipliable(lhs, rhs);

		return detail::expr_op{ lhs, rhs, detail::matrix_multiply_op, lhs.rows(), rhs.columns() };
	}

	template<typename Base, typename Element>
	[[nodiscard]] /* constexpr */ decltype(auto) operator/(const detail::expr_base<Base, Element>& object,
		Element constant)
	{
		return detail::expr_constant_op{ object, std::move(constant), detail::scalar_divide_op };
	}

	template<typename Base, typename Element>
	[[nodiscard]] /* constexpr */ decltype(auto) operator/(Element constant,
		const detail::expr_base<Base, Element>& object)
	{
		return detail::expr_constant_op{ object, std::move(constant), detail::scalar_divide_op };
	}

	template<typename Element,
		std::size_t LeftRows,
		std::size_t LeftColumns,
		std::size_t RightRows,
		std::size_t RightColumns>
	/* constexpr */ decltype(auto) operator+=(matrix<Element, LeftRows, LeftColumns>& lhs,
		const matrix<Element, RightRows, RightColumns>& rhs)
	{
		detail::validate_matrices_same_size(lhs, rhs);

		std::ranges::transform(lhs, rhs, lhs.begin(), std::plus<>{});

		return lhs;
	}

	template<typename Element,
		std::size_t LeftRows,
		std::size_t LeftColumns,
		std::size_t RightRows,
		std::size_t RightColumns>
	/* constexpr */ decltype(auto) operator-=(matrix<Element, LeftRows, LeftColumns>& lhs,
		const matrix<Element, RightRows, RightColumns>& rhs)
	{
		detail::validate_matrices_same_size(lhs, rhs);

		std::ranges::transform(lhs, rhs, lhs.begin(), std::minus<>{});

		return lhs;
	}

	template<typename Element, std::size_t Rows, std::size_t Columns>
	/* constexpr */ decltype(auto) operator*=(matrix<Element, Rows, Columns>& object, const Element& constant)
	{
		std::ranges::transform(object, object.begin(), std::bind_front(std::multiplies<>{}, constant));

		return object;
	}

	template<typename Element, std::size_t Rows, std::size_t Columns>
	/* constexpr */ decltype(auto) operator/=(matrix<Element, Rows, Columns>& object, const Element& constant)
	{
		std::ranges::transform(object, object.begin(), std::bind_front(std::divides<>{}, constant));

		return object;
	}

	// @TODO: Properly format this once ReferenceAlignment is implemented in clang-format
	// clang-format off
	template<range_2d Range2D>
	matrix(Range2D&&) -> matrix<std::ranges::range_value_t<std::ranges::range_value_t<Range2D>>>;
	// clang-format on

	template<arithmetic Element = int, std::size_t N = std::dynamic_extent>
	class identity_matrix : public matrix<Element, N, N>
	{
		using base = matrix<Element, N, N>;

		// @TODO: Uncomment out constexpr specifier when constexpr std::vector ships

		/* constexpr */ explicit identity_matrix(std::size_t side_length, detail::identity_matrix_impl_tag) :
			base(side_length, side_length, matrix_reserve_only)
		{
			for (auto row_index = std::size_t{ 0 }; row_index < side_length; ++row_index)
			{
				for (auto column_index = std::size_t{ 0 }; column_index < side_length; ++column_index)
				{
					auto index = detail::index_2d_to_1d(side_length, row_index, column_index);
					auto value = row_index == column_index ? Element{ 1 } : Element{ 0 };

					if constexpr (base::_buffer_is_vector)
					{
						base::_buffer.push_back(std::move(value));
					}
					else
					{
						base::_buffer[index] = std::move(value);
					}
				}
			}
		}

	public:
		// @TODO: Uncomment out constexpr specifier when constexpr std::vector ships

		/* constexpr */ identity_matrix(const identity_matrix&) = default;
		/* constexpr */ identity_matrix(identity_matrix&&)      = default;

		/* constexpr */ identity_matrix() requires(N != std::dynamic_extent) :
			identity_matrix(N, detail::identity_matrix_impl)
		{
		}

		/* constexpr */ explicit identity_matrix(std::size_t side_length) requires(N == std::dynamic_extent) :
			identity_matrix(side_length, detail::identity_matrix_impl)
		{
		}
	};

	template<arithmetic Element, std::size_t Rows = std::dynamic_extent, std::size_t Columns = std::dynamic_extent>
	class zero_matrix : public matrix<Element, Rows, Columns>
	{
		using base = matrix<Element, Rows, Columns>;

	public:
		// @TODO: Uncomment out constexpr specifier when constexpr std::vector ships

		/* constexpr */ zero_matrix(const zero_matrix&) = default;
		/* constexpr */ zero_matrix(zero_matrix&&)      = default;

		/* constexpr */ zero_matrix(std::size_t rows, std::size_t columns) : base(rows, columns, Element{ 0 }) {}

		/* constexpr */ zero_matrix() requires(Rows != std::dynamic_extent && Columns != std::dynamic_extent) :
			base(Rows, Columns, Element{ 0 })
		{
		}
	};
} // namespace matrixpp
