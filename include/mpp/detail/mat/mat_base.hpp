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
#include <mpp/detail/mat/mat_iter.hpp>
#include <mpp/detail/utility/public.hpp>
#include <mpp/detail/util/util.hpp>
#include <mpp/util/traits.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <type_traits>

namespace mpp::detail
{
	/**
	 * Base matrix class to store internal data and define common member functions
	 */
	template<typename SubMat, typename Buf, typename Val, std::size_t Rows, std::size_t Cols, typename Alloc>
	class mat_base : public expr_base<SubMat, Val, Rows, Cols>, public traits<Val>
	{
		using base = expr_base<SubMat, Val, Rows, Cols>;

	protected:
		Buf buf_;
		std::size_t rows_{ Rows == dyn ? std::size_t{} : Rows };
		std::size_t cols_{ Cols == dyn ? std::size_t{} : Cols };

		template<typename... Args>
		mat_base(std::size_t rows, std::size_t cols, Args&&... args) noexcept(
			std::is_nothrow_constructible_v<Buf, Args...>) :
			buf_(std::forward<Args>(args)...),
			rows_{ rows },
			cols_{ cols } // @TODO: ISSUE #20
		{
		}

		void assign_rng_2d(auto&& range_2d)
		{
			// Preconditions:
			// range_2d is the same size as specified matrix size
			// range_2d's rows all are equal in length

			using range_2d_t = decltype(range_2d);

			const auto range_rows = std::ranges::size(range_2d);

			auto range_begin            = std::ranges::begin(range_2d);
			const auto range_cols       = range_rows > 0 ? std::ranges::size(*range_begin) : std::size_t{};
			const auto range_total_size = range_rows * range_cols;
			const auto buf_size         = buf_.size();

			constexpr auto range_is_moved = std::is_rvalue_reference_v<range_2d_t>;
			constexpr auto buf_is_vector  = is_vec<Buf>::value;
			constexpr auto range_has_same_value_type =
				std::is_same_v<range_2d_value_t<std::remove_cvref_t<range_2d_t>>, Val>;

			if constexpr (buf_is_vector)
			{
				buf_.reserve(range_total_size);
			}

			// Assign all elements (until the need to insert for dynamic matrices)
			const auto min_rows = (std::min)(range_rows, rows_);
			auto buf_begin      = buf_.begin();

			for (auto row = std::size_t{}; row < min_rows; ++row)
			{
				const auto rooms_available_from_current = buf_size - row * range_cols;

				if (rooms_available_from_current >= range_cols)
				{
					if constexpr (range_has_same_value_type)
					{
						if constexpr (range_is_moved)
						{
							std::ranges::move(*range_begin, buf_begin);
						}
						else
						{
							std::ranges::copy(*range_begin, buf_begin);
						}
					}
					else
					{
						// @TODO: Check if this *really* is perfect forwarding values
						std::ranges::transform(*range_begin, buf_begin, [](auto&& value) -> decltype(auto) {
							return static_cast<Val>(std::forward<decltype(value)>(value));
						});
					}
				}
				else
				{
					// Only dynamic matrices reach here
					// We've gotten to the point where some or rest of the elements needs to be inserted

					if constexpr (buf_is_vector)
					{
						const auto row_assign_begin = std::ranges::begin(*range_begin);
						const auto row_assign_end =
							row_assign_begin + static_cast<difference_type>(rooms_available_from_current);

						// Assign the elements to rest of available space
						if constexpr (range_has_same_value_type)
						{
							if constexpr (range_is_moved)
							{
								std::move(row_assign_begin, row_assign_end, buf_begin);
							}
							else
							{
								std::copy(row_assign_begin, row_assign_end, buf_begin);
							}
						}
						else
						{
							// @TODO: Check if this *really* is perfect forwarding values
							std::transform(row_assign_begin,
								row_assign_end,
								buf_begin,
								[](auto&& value) -> decltype(auto) {
									return static_cast<Val>(std::forward<decltype(value)>(value));
								});
						}

						const auto elements_to_insert = range_cols - rooms_available_from_current;
						const auto row_end = row_assign_end + static_cast<difference_type>(elements_to_insert);
						const auto buf_back_inserter = std::back_inserter(buf_);

						// Insert rest of the elements in the current row
						if constexpr (range_has_same_value_type)
						{
							if constexpr (range_is_moved)
							{
								std::move(row_assign_end, row_end, buf_back_inserter);
							}
							else
							{
								std::copy(row_assign_end, row_end, buf_back_inserter);
							}
						}
						else
						{
							// @TODO: Check if this *really* is perfect forwarding values
							std::transform(row_assign_end,
								row_end,
								buf_back_inserter,
								[](auto&& value) -> decltype(auto) {
									return static_cast<Val>(std::forward<decltype(value)>(value));
								});
						}

						rows_ = ++row; // Use the inserter loop to insert rest of the elements
						++range_begin;

						break;
					}
				}

				++range_begin;
				buf_begin += static_cast<difference_type>(range_cols);
			}

			// Insert every element after (only dynamic matrices reach this point)
			if constexpr (buf_is_vector)
			{
				const auto buf_back_inserter = std::back_inserter(buf_);

				for (auto row = rows_; row < range_rows; ++row)
				{
					if constexpr (range_has_same_value_type)
					{
						if constexpr (range_is_moved)
						{
							std::ranges::move(*range_begin, buf_back_inserter);
						}
						else
						{
							std::ranges::copy(*range_begin, buf_back_inserter);
						}
					}
					else
					{
						// @TODO: Check if this *really* is perfect forwarding values
						std::ranges::transform(*range_begin, buf_back_inserter, [](auto&& value) -> decltype(auto) {
							return static_cast<Val>(std::forward<decltype(value)>(value));
						});
					}

					++range_begin;
				}
			}

			if constexpr (buf_is_vector)
			{
				if (range_total_size < buf_size)
				{
					buf_.resize(range_total_size);
				}
			}

			rows_ = range_rows;
			cols_ = range_cols;
		}

		void assign_rng_1d(std::size_t rows, std::size_t cols, auto&& range)
		{
			// Preconditions depend on the type of matrix

			const auto buf_size   = buf_.size();
			const auto range_size = rows * cols;

			constexpr auto range_is_moved            = std::is_rvalue_reference_v<decltype(range)>;
			constexpr auto buf_is_vector             = is_vec<Buf>::value;
			constexpr auto range_has_same_value_type = std::is_same_v<std::ranges::range_value_t<decltype(range)>, Val>;

			if constexpr (buf_is_vector)
			{
				buf_.reserve(range_size);
			}

			const auto max_elements_to_assign = (std::min)(range_size, buf_size);
			const auto assign_begin           = std::ranges::begin(range);
			const auto assign_end             = assign_begin + static_cast<difference_type>(max_elements_to_assign);
			const auto buf_begin              = buf_.begin();

			// Try to assign the all elements it can
			if constexpr (range_has_same_value_type)
			{
				if constexpr (range_is_moved)
				{
					std::move(assign_begin, assign_end, buf_begin);
				}
				else
				{
					std::copy(assign_begin, assign_end, buf_begin);
				}
			}
			else
			{
				// @TODO: Check if this *really* is perfect forwarding values
				std::transform(assign_begin, assign_end, buf_.begin(), [](auto&& value) -> decltype(auto) {
					return static_cast<Val>(std::forward<decltype(value)>(value));
				});
			}

			const auto range_end         = std::ranges::end(range);
			const auto buf_back_inserter = std::back_inserter(buf_);

			// Insert the leftovers (only dynamic matrices needs this)
			if constexpr (buf_is_vector)
			{
				if constexpr (range_has_same_value_type)
				{
					if constexpr (range_is_moved)
					{
						std::move(assign_end, range_end, buf_back_inserter);
					}
					else
					{
						std::copy(assign_end, range_end, buf_back_inserter);
					}
				}
				else
				{
					// @TODO: Check if this *really* is perfect forwarding values
					std::transform(assign_end, range_end, buf_back_inserter, [](auto&& value) -> decltype(auto) {
						return static_cast<Val>(std::forward<decltype(value)>(value));
					});
				}

				if (range_size < buf_size)
				{
					buf_.resize(range_size);
				}
			}

			rows_ = rows;
			cols_ = cols;
		}

	public:
		using buffer_type = Buf;

		using value_type             = Val;
		using reference              = value_type&;
		using const_reference        = const value_type&;
		using pointer                = typename buffer_type::pointer;
		using const_pointer          = typename buffer_type::const_pointer;
		using iterator               = mat_iter<typename Buf::iterator>;
		using const_iterator         = mat_iter<typename Buf::const_iterator>;
		using reverse_iterator       = mat_iter<typename Buf::reverse_iterator>;
		using const_reverse_iterator = mat_iter<typename Buf::const_reverse_iterator>;
		using difference_type        = typename buffer_type::difference_type;
		using size_type              = std::size_t;
		using allocator_type         = Alloc;

		mat_base(const mat_base&) noexcept(std::is_nothrow_copy_constructible_v<Val>) = default; // @TODO: ISSUE #20
		mat_base(mat_base&&) noexcept(std::is_nothrow_move_constructible_v<Val>)      = default; // @TODO: ISSUE #20

		auto operator    =(const mat_base&) noexcept(std::is_nothrow_copy_assignable_v<Val>)
			-> mat_base& = default; // @TODO: ISSUE #20
		auto operator    =(mat_base&&) noexcept(std::is_nothrow_move_assignable_v<Val>)
			-> mat_base& = default; // @TODO: ISSUE #20

		[[nodiscard]] auto data() noexcept -> pointer // @TODO: ISSUE #20
		{
			return buf_.data();
		}

		[[nodiscard]] auto data() const noexcept -> const_pointer // @TODO: ISSUE #20
		{
			return buf_.data();
		}

		[[nodiscard]] auto begin() noexcept -> iterator // @TODO: ISSUE #20
		{
			return iterator(buf_.begin(), cols_);
		}

		[[nodiscard]] auto begin() const noexcept -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(buf_.cbegin(), cols_);
		}

		[[nodiscard]] auto end() noexcept -> iterator // @TODO: ISSUE #20
		{
			return iterator(buf_.end(), cols_);
		}

		[[nodiscard]] auto end() const noexcept -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(buf_.cend(), cols_);
		}

		[[nodiscard]] auto rbegin() noexcept -> reverse_iterator // @TODO: ISSUE #20
		{
			return reverse_iterator(buf_.rbegin(), cols_);
		}

		[[nodiscard]] auto rbegin() const noexcept -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(buf_.crbegin(), cols_);
		}

		[[nodiscard]] auto rend() noexcept -> reverse_iterator // @TODO: ISSUE #20
		{
			return reverse_iterator(buf_.rend(), cols_);
		}

		[[nodiscard]] auto rend() const noexcept -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(buf_.crend(), cols_);
		}

		[[nodiscard]] auto cbegin() noexcept -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(buf_.cbegin(), cols_);
		}

		[[nodiscard]] auto cbegin() const noexcept -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(buf_.cbegin(), cols_);
		}

		[[nodiscard]] auto cend() noexcept -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(buf_.cend(), cols_);
		}

		[[nodiscard]] auto cend() const noexcept -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(buf_.cend(), cols_);
		}

		[[nodiscard]] auto crbegin() noexcept -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(buf_.crbegin(), cols_);
		}

		[[nodiscard]] auto crbegin() const noexcept -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(buf_.crbegin(), cols_);
		}

		[[nodiscard]] auto crend() noexcept -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(buf_.crend(), cols_);
		}

		[[nodiscard]] auto crend() const noexcept -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(buf_.crend(), cols_);
		}

		[[nodiscard]] auto operator()(std::size_t row_index, std::size_t col_index) noexcept(
			noexcept(buf_[idx_1d(cols_, row_index, col_index)])) -> reference // @TODO: ISSUE #20
		{
			return buf_[idx_1d(cols_, row_index, col_index)];
		}

		[[nodiscard]] auto operator()(std::size_t row_index, std::size_t col_index) const
			noexcept(noexcept(buf_[idx_1d(cols_, row_index, col_index)])) -> const_reference // @TODO: ISSUE #20
		{
			return buf_[idx_1d(cols_, row_index, col_index)];
		}

		[[nodiscard]] auto operator[](std::size_t index) noexcept(noexcept(buf_[index])) -> reference
		{
			return buf_[index];
		}

		[[nodiscard]] auto operator[](std::size_t index) const noexcept(noexcept(buf_[index])) -> const_reference
		{
			return buf_[index];
		}

		[[nodiscard]] auto rows() const noexcept -> std::size_t // @TODO: ISSUE #20
		{
			return rows_;
		}

		[[nodiscard]] auto cols() const noexcept -> std::size_t // @TODO: ISSUE #20
		{
			return cols_;
		}

		[[nodiscard]] auto front() noexcept -> reference // @TODO: ISSUE #20
		{
			return buf_.front();
		}

		[[nodiscard]] auto front() const noexcept -> const_reference // @TODO: ISSUE #20
		{
			return buf_.front();
		}

		[[nodiscard]] auto back() noexcept -> reference // @TODO: ISSUE #20
		{
			return buf_.back();
		}

		[[nodiscard]] auto back() const noexcept -> const_reference // @TODO: ISSUE #20
		{
			return buf_.back();
		}

		[[nodiscard]] auto size() const noexcept -> std::size_t // @TODO: ISSUE #20
		{
			return buf_.size();
		}

		[[nodiscard]] auto max_size() const noexcept -> std::size_t // @TODO: ISSUE #20
		{
			return buf_.max_size();
		}

		[[nodiscard]] auto empty() const noexcept -> bool // @TODO: ISSUE #20
		{
			return buf_.empty();
		}

		template<rng_2d_with_value_convertible_to<Val> Rng>
		auto operator=(Rng&& rng) noexcept(noexcept(base::expr_mut().assign(std::forward<Rng>(rng))))
			-> mat_base& // @TODO: ISSUE #20
		{
			base::expr_mut().assign(std::forward<Rng>(rng));
			return *this;
		}

		// clang-format off
		template<matrix_with_value_convertible_to<Val> Mat>
			requires (!std::same_as<std::remove_cvref_t<Mat>, mat<Val, Rows, Cols, Alloc>>)
		auto operator=(Mat&& mat) noexcept(noexcept(base::expr_mut().assign(std::forward<Mat>(mat))))
			-> mat_base& // @TODO: ISSUE #20
		{
			base::expr_mut().assign(std::forward<Mat>(mat));
			return *this;
		}
		// clang-format on

		void swap(mat_base& b) noexcept // @TODO: ISSUE #20
		{
			// Don't swap with the same object
			if (&b != this)
			{
				using std::swap;

				swap(rows_, b.rows_);
				swap(cols_, b.cols_);
				swap(buf_, b.buf_);
			}
		}
	};

	/**
	 * Base matrix class for dynamic matrices. It defines common constructors and member functions for dynamic matrices.
	 */
	template<typename Mat, typename Buf, typename Val, std::size_t Rows, std::size_t Cols, typename Alloc>
	class mat_dyn_base : public mat_base<Mat, Buf, Val, Rows, Cols, Alloc>
	{
		using base = mat_base<Mat, Buf, Val, Rows, Cols, Alloc>;

	protected:
		// @FIXME: Add noexcept specification here
		template<typename... Args>
		mat_dyn_base(std::size_t rows, std::size_t cols, Args&&... args) : base(rows, cols, std::forward<Args>(args)...)
		{
		}

		void init_from_expr_dyn(std::size_t rows, std::size_t cols,
			const auto& expr) // @TODO: ISSUE #20
		{
			base::rows_ = rows;
			base::cols_ = cols;

			base::buf_.reserve(rows * cols);

			for (auto row = std::size_t{}; row < base::rows_; ++row)
			{
				for (auto col = std::size_t{}; col < base::cols_; ++col)
				{
					base::buf_.push_back(expr(row, col));
				}
			}
		}

		template<typename Fn>
		void init_from_fn_dyn(std::size_t rows,
			std::size_t cols,
			Fn&& callable) // @TODO: ISSUE #20
		{
			const auto total_size = rows * cols;

			base::buf_.reserve(total_size);

			for (auto index = std::size_t{}; index < total_size; ++index)
			{
				base::buf_.push_back(std::invoke(std::forward<Fn>(callable)));
			}
		}

	public:
		using base::operator=;

		[[nodiscard]] auto get_allocator() const noexcept -> typename base::allocator_type // @TODO: ISSUE #20
		{
			return base::buf_.get_allocator();
		}

		void clear() noexcept // @TODO: ISSUE #20
		{
			base::rows_ = 0;
			base::cols_ = 0;
			base::buf_.clear();
		}
	};
} // namespace mpp::detail
