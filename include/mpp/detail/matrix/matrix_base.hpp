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

#include <mpp/detail/expr/expr_base.hpp>
#include <mpp/detail/matrix/matrix_iterator.hpp>
#include <mpp/detail/types/constraints.hpp>
#include <mpp/detail/types/type_traits.hpp>
#include <mpp/detail/utility/buffer_manipulators.hpp>
#include <mpp/detail/utility/utility.hpp>
#include <mpp/utility/configuration.hpp>
#include <mpp/utility/traits.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <type_traits>

namespace mpp::detail
{
	/**
	 * Base matrix class to store internal data and define common member functions
	 */
	template<typename Derived,
		typename Buffer,
		typename Value,
		std::size_t RowsExtent,
		std::size_t ColumnsExtent,
		typename Allocator>
	class matrix_base : public expr_base<Derived, Value, RowsExtent, ColumnsExtent>, public traits<Value>
	{
		using base = expr_base<Derived, Value, RowsExtent, ColumnsExtent>;

	protected:
		Buffer buffer_;
		std::size_t rows_{ RowsExtent == dynamic ? std::size_t{} : RowsExtent };
		std::size_t columns_{ ColumnsExtent == dynamic ? std::size_t{} : ColumnsExtent };

		template<typename... Args>
		matrix_base(std::size_t rows, std::size_t columns, Args&&... args) noexcept(
			std::is_nothrow_constructible_v<Buffer, Args...>) :
			buffer_(std::forward<Args>(args)...),
			rows_{ rows },
			columns_{ columns } // @TODO: ISSUE #20
		{
		}

		void assign_and_insert_from_2d_range(auto&& range_2d)
		{
			// Preconditions:
			// range_2d is the same size as specified matrix size
			// range_2d's rows all are equal in length

			using range_2d_t = decltype(range_2d);

			const auto range_rows = std::ranges::size(range_2d);

			auto range_begin            = std::ranges::begin(range_2d);
			const auto range_columns    = range_rows > 0 ? std::ranges::size(*range_begin) : std::size_t{};
			const auto range_total_size = range_rows * range_columns;
			const auto buffer_size      = buffer_.size();

			constexpr auto range_is_moved   = std::is_rvalue_reference_v<range_2d_t>;
			constexpr auto buffer_is_vector = is_vector<Buffer>::value;
			constexpr auto range_has_same_value_type =
				std::is_same_v<range_2d_value_t<std::remove_cvref_t<range_2d_t>>, Value>;

			if constexpr (buffer_is_vector)
			{
				buffer_.reserve(range_total_size);
			}

			// Assign all elements (until the need to insert for dynamic matrices)
			const auto min_rows = (std::min)(range_rows, rows_);
			auto buffer_begin   = buffer_.begin();

			for (auto row = std::size_t{}; row < min_rows; ++row)
			{
				const auto rooms_available_from_current = buffer_size - row * range_columns;

				if (rooms_available_from_current >= range_columns)
				{
					if constexpr (range_has_same_value_type)
					{
						if constexpr (range_is_moved)
						{
							std::ranges::move(*range_begin, buffer_begin);
						}
						else
						{
							std::ranges::copy(*range_begin, buffer_begin);
						}
					}
					else
					{
						// @TODO: Check if this *really* is perfect forwarding values
						std::ranges::transform(*range_begin, buffer_begin, [](auto&& value) -> decltype(auto) {
							return static_cast<Value>(std::forward<decltype(value)>(value));
						});
					}
				}
				else
				{
					// Only dynamic matrices reach here
					// We've gotten to the point where some or rest of the elements needs to be inserted

					if constexpr (buffer_is_vector)
					{
						const auto row_assign_begin = std::ranges::begin(*range_begin);
						const auto row_assign_end =
							row_assign_begin + static_cast<difference_type>(rooms_available_from_current);

						// Assign the elements to rest of available space
						if constexpr (range_has_same_value_type)
						{
							if constexpr (range_is_moved)
							{
								std::move(row_assign_begin, row_assign_end, buffer_begin);
							}
							else
							{
								std::copy(row_assign_begin, row_assign_end, buffer_begin);
							}
						}
						else
						{
							// @TODO: Check if this *really* is perfect forwarding values
							std::transform(row_assign_begin,
								row_assign_end,
								buffer_begin,
								[](auto&& value) -> decltype(auto) {
									return static_cast<Value>(std::forward<decltype(value)>(value));
								});
						}

						const auto elements_to_insert = range_columns - rooms_available_from_current;
						const auto row_end = row_assign_end + static_cast<difference_type>(elements_to_insert);
						const auto buffer_back_inserter = std::back_inserter(buffer_);

						// Insert rest of the elements in the current row
						if constexpr (range_has_same_value_type)
						{
							if constexpr (range_is_moved)
							{
								std::move(row_assign_end, row_end, buffer_back_inserter);
							}
							else
							{
								std::copy(row_assign_end, row_end, buffer_back_inserter);
							}
						}
						else
						{
							// @TODO: Check if this *really* is perfect forwarding values
							std::transform(row_assign_end,
								row_end,
								buffer_back_inserter,
								[](auto&& value) -> decltype(auto) {
									return static_cast<Value>(std::forward<decltype(value)>(value));
								});
						}

						rows_ = ++row; // Use the inserter loop to insert rest of the elements
						++range_begin;

						break;
					}
				}

				++range_begin;
				buffer_begin += static_cast<difference_type>(range_columns);
			}

			// Insert every element after (only dynamic matrices reach this point)
			if constexpr (buffer_is_vector)
			{
				const auto buffer_back_inserter = std::back_inserter(buffer_);

				for (auto row = rows_; row < range_rows; ++row)
				{
					if constexpr (range_has_same_value_type)
					{
						if constexpr (range_is_moved)
						{
							std::ranges::move(*range_begin, buffer_back_inserter);
						}
						else
						{
							std::ranges::copy(*range_begin, buffer_back_inserter);
						}
					}
					else
					{
						// @TODO: Check if this *really* is perfect forwarding values
						std::ranges::transform(*range_begin, buffer_back_inserter, [](auto&& value) -> decltype(auto) {
							return static_cast<Value>(std::forward<decltype(value)>(value));
						});
					}

					++range_begin;
				}
			}

			if constexpr (buffer_is_vector)
			{
				if (range_total_size < buffer_size)
				{
					buffer_.resize(range_total_size);
				}
			}

			rows_    = range_rows;
			columns_ = range_columns;
		}

		void assign_and_insert_from_1d_range(std::size_t rows, std::size_t columns, auto&& range)
		{
			// Preconditions depend on the type of matrix

			const auto buffer_size = buffer_.size();
			const auto range_size  = rows * columns;

			constexpr auto range_is_moved   = std::is_rvalue_reference_v<decltype(range)>;
			constexpr auto buffer_is_vector = is_vector<Buffer>::value;
			constexpr auto range_has_same_value_type =
				std::is_same_v<std::ranges::range_value_t<decltype(range)>, Value>;

			if constexpr (buffer_is_vector)
			{
				buffer_.reserve(range_size);
			}

			const auto max_elements_to_assign = (std::min)(range_size, buffer_size);
			const auto assign_begin           = std::ranges::begin(range);
			const auto assign_end             = assign_begin + static_cast<difference_type>(max_elements_to_assign);
			const auto buffer_begin           = buffer_.begin();

			// Try to assign the all elements it can
			if constexpr (range_has_same_value_type)
			{
				if constexpr (range_is_moved)
				{
					std::move(assign_begin, assign_end, buffer_begin);
				}
				else
				{
					std::copy(assign_begin, assign_end, buffer_begin);
				}
			}
			else
			{
				// @TODO: Check if this *really* is perfect forwarding values
				std::transform(assign_begin, assign_end, buffer_.begin(), [](auto&& value) -> decltype(auto) {
					return static_cast<Value>(std::forward<decltype(value)>(value));
				});
			}

			const auto range_end            = std::ranges::end(range);
			const auto buffer_back_inserter = std::back_inserter(buffer_);

			// Insert the leftovers (only dynamic matrices needs this)
			if constexpr (buffer_is_vector)
			{
				if constexpr (range_has_same_value_type)
				{
					if constexpr (range_is_moved)
					{
						std::move(assign_end, range_end, buffer_back_inserter);
					}
					else
					{
						std::copy(assign_end, range_end, buffer_back_inserter);
					}
				}
				else
				{
					// @TODO: Check if this *really* is perfect forwarding values
					std::transform(assign_end, range_end, buffer_back_inserter, [](auto&& value) -> decltype(auto) {
						return static_cast<Value>(std::forward<decltype(value)>(value));
					});
				}

				if (range_size < buffer_size)
				{
					buffer_.resize(range_size);
				}
			}

			rows_    = rows;
			columns_ = columns;
		}

	public:
		using buffer_type = Buffer;

		using value_type             = Value;
		using reference              = value_type&;
		using const_reference        = const value_type&;
		using pointer                = typename buffer_type::pointer;
		using const_pointer          = typename buffer_type::const_pointer;
		using iterator               = matrix_iterator<typename Buffer::iterator>;
		using const_iterator         = matrix_iterator<typename Buffer::const_iterator>;
		using reverse_iterator       = matrix_iterator<typename Buffer::reverse_iterator>;
		using const_reverse_iterator = matrix_iterator<typename Buffer::const_reverse_iterator>;
		using difference_type        = typename buffer_type::difference_type;
		using size_type              = std::size_t;

		matrix_base(const matrix_base&) noexcept(
			std::is_nothrow_copy_constructible_v<Value>)                                 = default; // @TODO: ISSUE #20
		matrix_base(matrix_base&&) noexcept(std::is_nothrow_move_constructible_v<Value>) = default; // @TODO: ISSUE #20

		auto operator       =(const matrix_base&) noexcept(std::is_nothrow_copy_assignable_v<Value>)
			-> matrix_base& = default; // @TODO: ISSUE #20
		auto operator       =(matrix_base&&) noexcept(std::is_nothrow_move_assignable_v<Value>)
			-> matrix_base& = default; // @TODO: ISSUE #20

		[[nodiscard]] auto data() noexcept -> pointer // @TODO: ISSUE #20
		{
			return buffer_.data();
		}

		[[nodiscard]] auto data() const noexcept -> const_pointer // @TODO: ISSUE #20
		{
			return buffer_.data();
		}

		[[nodiscard]] auto begin() noexcept -> iterator // @TODO: ISSUE #20
		{
			return iterator(buffer_.begin(), columns_);
		}

		[[nodiscard]] auto begin() const noexcept -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(buffer_.cbegin(), columns_);
		}

		[[nodiscard]] auto end() noexcept -> iterator // @TODO: ISSUE #20
		{
			return iterator(buffer_.end(), columns_);
		}

		[[nodiscard]] auto end() const noexcept -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(buffer_.cend(), columns_);
		}

		[[nodiscard]] auto rbegin() noexcept -> reverse_iterator // @TODO: ISSUE #20
		{
			return reverse_iterator(buffer_.rbegin(), columns_);
		}

		[[nodiscard]] auto rbegin() const noexcept -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(buffer_.crbegin(), columns_);
		}

		[[nodiscard]] auto rend() noexcept -> reverse_iterator // @TODO: ISSUE #20
		{
			return reverse_iterator(buffer_.rend(), columns_);
		}

		[[nodiscard]] auto rend() const noexcept -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(buffer_.crend(), columns_);
		}

		[[nodiscard]] auto cbegin() noexcept -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(buffer_.cbegin(), columns_);
		}

		[[nodiscard]] auto cbegin() const noexcept -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(buffer_.cbegin(), columns_);
		}

		[[nodiscard]] auto cend() noexcept -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(buffer_.cend(), columns_);
		}

		[[nodiscard]] auto cend() const noexcept -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(buffer_.cend(), columns_);
		}

		[[nodiscard]] auto crbegin() noexcept -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(buffer_.crbegin(), columns_);
		}

		[[nodiscard]] auto crbegin() const noexcept -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(buffer_.crbegin(), columns_);
		}

		[[nodiscard]] auto crend() noexcept -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(buffer_.crend(), columns_);
		}

		[[nodiscard]] auto crend() const noexcept -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(buffer_.crend(), columns_);
		}

		[[nodiscard]] auto operator()(std::size_t row_index, std::size_t col_index) noexcept(
			noexcept(buffer_[index_2d_to_1d(columns_, row_index, col_index)])) -> reference // @TODO: ISSUE #20
		{
			return buffer_[index_2d_to_1d(columns_, row_index, col_index)];
		}

		[[nodiscard]] auto operator()(std::size_t row_index, std::size_t col_index) const noexcept(
			noexcept(buffer_[index_2d_to_1d(columns_, row_index, col_index)])) -> const_reference // @TODO: ISSUE #20
		{
			return buffer_[index_2d_to_1d(columns_, row_index, col_index)];
		}

		[[nodiscard]] auto operator[](std::size_t index) noexcept(noexcept(buffer_[index])) -> reference
		{
			return buffer_[index];
		}

		[[nodiscard]] auto operator[](std::size_t index) const noexcept(noexcept(buffer_[index])) -> const_reference
		{
			return buffer_[index];
		}

		[[nodiscard]] auto rows() const noexcept -> std::size_t // @TODO: ISSUE #20
		{
			return rows_;
		}

		[[nodiscard]] auto columns() const noexcept -> std::size_t // @TODO: ISSUE #20
		{
			return columns_;
		}

		[[nodiscard]] auto front() noexcept -> reference // @TODO: ISSUE #20
		{
			return buffer_.front();
		}

		[[nodiscard]] auto front() const noexcept -> const_reference // @TODO: ISSUE #20
		{
			return buffer_.front();
		}

		[[nodiscard]] auto back() noexcept -> reference // @TODO: ISSUE #20
		{
			return buffer_.back();
		}

		[[nodiscard]] auto back() const noexcept -> const_reference // @TODO: ISSUE #20
		{
			return buffer_.back();
		}

		[[nodiscard]] auto size() const noexcept -> std::size_t // @TODO: ISSUE #20
		{
			return buffer_.size();
		}

		[[nodiscard]] auto max_size() const noexcept -> std::size_t // @TODO: ISSUE #20
		{
			return buffer_.max_size();
		}

		[[nodiscard]] auto empty() const noexcept -> bool // @TODO: ISSUE #20
		{
			return buffer_.empty();
		}

		template<range_2d_with_value_type_convertible_to<Value> Range2D>
		auto operator=(Range2D&& rng) noexcept(noexcept(base::expr_mutable_obj().assign(std::forward<Range2D>(rng))))
			-> matrix_base& // @TODO: ISSUE #20
		{
			base::expr_mutable_obj().assign(std::forward<Range2D>(rng));
			return *this;
		}

		// clang-format off
		template<matrix_with_value_convertible_to<Value> Matrix>
			requires (!std::same_as<std::remove_cvref_t<Matrix>, matrix<Value, RowsExtent, ColumnsExtent, Allocator>>)
		auto operator=(Matrix&& mat) noexcept(noexcept(base::expr_mutable_obj().assign(std::forward<Matrix>(mat))))
			-> matrix_base& // @TODO: ISSUE #20
		{
			base::expr_mutable_obj().assign(std::forward<Matrix>(mat));
			return *this;
		}
		// clang-format on

		void swap(matrix_base& right) noexcept // @TODO: ISSUE #20
		{
			// Don't swap with the same object
			if (&right != this)
			{
				using std::swap;

				swap(rows_, right.rows_);
				swap(columns_, right.columns_);
				swap(buffer_, right.buffer_);
			}
		}
	};

	/**
	 * Base matrix class for dynamic matrices only. It defines common constructors and member functions for dynamic
	 * matrices
	 */
	template<typename Derived,
		typename Buffer,
		typename Value,
		std::size_t RowsExtent,
		std::size_t ColumnsExtent,
		typename Allocator>
	class matrix_dynamic_base : public matrix_base<Derived, Buffer, Value, RowsExtent, ColumnsExtent, Allocator>
	{
		using base = matrix_base<Derived, Buffer, Value, RowsExtent, ColumnsExtent, Allocator>;

	protected:
		// @FIXME: Add noexcept specification here
		template<typename... Args>
		matrix_dynamic_base(std::size_t rows, std::size_t columns, Args&&... args) :
			base(rows, columns, std::forward<Args>(args)...)
		{
		}

		void initialize_from_expression_unchecked(std::size_t rows,
			std::size_t columns,
			const auto& expr) // @TODO: ISSUE #20
		{
			base::rows_    = rows;
			base::columns_ = columns;

			base::buffer_.reserve(rows * columns);

			for (auto row = std::size_t{}; row < base::rows_; ++row)
			{
				for (auto column = std::size_t{}; column < base::columns_; ++column)
				{
					base::buffer_.push_back(expr(row, column));
				}
			}
		}

		template<typename Callable>
		void initialize_buffer_from_callable_unchecked(std::size_t rows,
			std::size_t columns,
			Callable&& callable) // @TODO: ISSUE #20
		{
			const auto total_size = rows * columns;

			base::buffer_.reserve(total_size);

			for (auto index = std::size_t{}; index < total_size; ++index)
			{
				base::buffer_.push_back(std::invoke(std::forward<Callable>(callable)));
			}
		}

	public:
		using allocator_type = Allocator;

		using base::operator=;

		[[nodiscard]] auto get_allocator() const noexcept -> allocator_type // @TODO: ISSUE #20
		{
			return base::buffer_.get_allocator();
		}

		void clear() noexcept // @TODO: ISSUE #20
		{
			base::rows_    = 0;
			base::columns_ = 0;
			base::buffer_.clear();
		}
	};
} // namespace mpp::detail
