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
#include <mpp/detail/util/public.hpp>
#include <mpp/detail/util/util.hpp>

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <vector>

namespace mpp
{
	template<detail::arithmetic T, typename Buf = std::vector<T>>
	class mat : public detail::expr_base<mat<T, Buf>, T>
	{
		Buf buf_; // stores flattened data in row major
		std::size_t rows_;
		std::size_t cols_;

		template<bool Moved>
		void assign_rng_2d(auto&& rng)
		{
			// Preconditions:
			// rng is the same size as specified matrix size (fixed matrices only)
			// rng's rows all are equal in length

			auto rng_begin      = rng.begin();
			auto rng_end        = rng.end();
			const auto rng_rows = rng.size();
			const auto rng_cols = rng_rows == 0 ? 0 : rng_begin->size(); // assumes all columns are same length

			detail::resize_buf_if_vec(buf_, rng_rows, rng_cols, T{});
			auto buf_begin = buf_.begin();

			using difference_type = typename Buf::difference_type;

			for (; rng_begin != rng_end; ++rng_begin, buf_begin += static_cast<difference_type>(rng_cols))
			{
				if constexpr (Moved)
				{
					std::ranges::move(*rng_begin, buf_begin);
				}
				else
				{
					std::ranges::copy(*rng_begin, buf_begin);
				}
			}

			rows_ = rng_rows;
			cols_ = rng_cols;
		}

		template<bool Moved>
		void assign_rng_1d(std::size_t rows, std::size_t cols, auto&& rng)
		{
			// Preconditions:
			// rng is the same size as the specified matrix size (fixed matrices only)

			detail::resize_buf_if_vec(buf_, rows, cols, T{});

			if constexpr (Moved)
			{
				std::ranges::move(rng, buf_.begin());
			}
			else
			{
				std::ranges::copy(rng, buf_.begin());
			}

			rows_ = rows;
			cols_ = cols;
		}

		template<typename Expr>
		void assign_from_expr(Expr&& expr)
		{
		}

	public:
		using value_type  = T;
		using buffer_type = Buf;

		using reference       = value_type&;
		using const_reference = const value_type&;
		using pointer         = typename Buf::pointer;
		using const_pointer   = typename Buf::const_pointer;

		using iterator               = detail::mat_iter<typename Buf::iterator>;
		using const_iterator         = detail::mat_iter<typename Buf::const_iterator>;
		using reverse_iterator       = detail::mat_iter<typename Buf::reverse_iterator>;
		using const_reverse_iterator = detail::mat_iter<typename Buf::const_reverse_iterator>;

		using difference_type = typename Buf::difference_type;
		using size_type       = std::size_t;

		mat() noexcept(std::is_nothrow_default_constructible_v<Buf>) : rows_(0), cols_(0) {} // @TODO: ISSUE #20
		mat(const mat&) noexcept(std::is_nothrow_copy_constructible_v<Buf>) = default;       // @TODO: ISSUE #20
		mat(mat&&) noexcept(std::is_nothrow_move_constructible_v<Buf>)      = default;       // @TODO: ISSUE #20

		template<detail::matrix_with_value_convertible_to<T> Mat>
		requires(!std::same_as<std::remove_cvref_t<Mat>, mat<T, Buf>>) explicit mat(Mat&& obj) // @TODO: ISSUE #20
		{
			const auto rows = std::forward<Mat>(obj).rows();
			const auto cols = std::forward<Mat>(obj).cols();

			assign_rng_1d<detail::is_moved<decltype(obj)>>(rows, cols, std::forward<Mat>(obj));
		}

		template<detail::rng_1d_with_value_convertible_to<T> Rng>
		mat(size_type rows, size_type cols, Rng&& rng) // @TODO: ISSUE #20
		{
			assign_rng_1d<detail::is_moved<decltype(rng)>>(rows, cols, std::forward<Rng>(rng));
		}


		template<std::convertible_to<T> T2>
		explicit mat(std::initializer_list<std::initializer_list<T2>> init) // @TODO: ISSUE #20
		{
			assign_rng_2d<false>(init);
		}

		template<detail::rng_2d_with_value_convertible_to<T> Rng>
		mat(Rng&& rng) // @TODO: ISSUE #20
		{
			assign_rng_2d<detail::is_moved<decltype(rng)>>(std::forward<Rng>(rng));
		}

		mat(size_type rows, size_type cols, const T& val = T{ 0 }) : rows_(rows), cols_(cols)
		{
			if constexpr (detail::is_vec<Buf>::value)
			{
				buf_.resize(rows * cols, val);
			}
			else
			{
				std::ranges::fill(buf_, val);
			}
		}

		mat(size_type rows, size_type cols, identity_tag, const T& zero_val = T{}, const T& one_val = T{ 1 }) :
			rows_(rows),
			cols_(cols) // @TODO: ISSUE #20
		{
			detail::init_identity_buf(buf_, rows, cols, zero_val, one_val);
		}

		// @FIXME: Allow fn's value return be convertible to value type
		template<detail::fn_with_return_type<T> Fn>
		mat(size_type rows, size_type cols, Fn&& fn) : rows_(rows), cols_(cols) // @TODO: ISSUE #20
		{
			detail::resize_buf_if_vec(buf_, rows, cols, T{});

			std::ranges::generate(buf_, std::forward<Fn>(fn));
		}

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

		[[nodiscard]] auto operator()(size_type row, size_type col) noexcept(
			noexcept(buf_[detail::idx_1d(cols_, row, col)])) -> reference // @TODO: ISSUE #20
		{
			return buf_[detail::idx_1d(cols_, row, col)];
		}

		[[nodiscard]] auto operator()(size_type row, size_type col) const
			noexcept(noexcept(buf_[detail::idx_1d(cols_, row, col)])) -> const_reference // @TODO: ISSUE #20
		{
			return buf_[detail::idx_1d(cols_, row, col)];
		}

		[[nodiscard]] auto operator[](size_type i) noexcept(noexcept(buf_[i])) -> reference
		{
			return buf_[i];
		}

		[[nodiscard]] auto operator[](size_type i) const noexcept(noexcept(buf_[i])) -> const_reference
		{
			return buf_[i];
		}

		[[nodiscard]] auto rows() const noexcept -> size_type // @TODO: ISSUE #20
		{
			return rows_;
		}

		[[nodiscard]] auto cols() const noexcept -> size_type // @TODO: ISSUE #20
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

		[[nodiscard]] auto size() const noexcept -> size_type // @TODO: ISSUE #20
		{
			return buf_.size();
		}

		[[nodiscard]] auto max_size() const noexcept -> size_type // @TODO: ISSUE #20
		{
			return buf_.max_size();
		}

		[[nodiscard]] auto empty() const noexcept -> bool // @TODO: ISSUE #20
		{
			return buf_.empty();
		}

		[[nodiscard]] auto buffer() const noexcept -> const buffer_type& // @TODO: ISSUE #20
		{
			return buf_;
		}

		[[nodiscard]] auto buffer() noexcept -> buffer_type& // @TODO: ISSUE #20
		{
			return buf_;
		}

		void clear() noexcept // @TODO: ISSUE #20
		{
			rows_ = 0;
			cols_ = 0;
			buf_.clear();
		}

		template<std::convertible_to<T> T2>
		void assign(std::initializer_list<std::initializer_list<T2>> init) // @TODO: ISSUE #20
		{
			assign_rng_2d<false>(init);
		}

		template<detail::rng_2d_with_value_convertible_to<T> Rng>
		void assign(Rng&& rng) // @TODO: ISSUE #20
		{
			assign_rng_2d<detail::is_moved<decltype(rng)>>(std::forward<Rng>(rng));
		}

		template<detail::matrix_with_value_convertible_to<T> Mat>
		requires(!std::same_as<std::remove_cvref_t<Mat>, mat<T, Buf>>) void assign(Mat&& obj) // @TODO: ISSUE #20
		{
			const auto rows = std::forward<Mat>(obj).rows();
			const auto cols = std::forward<Mat>(obj).cols();

			assign_rng_1d<detail::is_moved<decltype(obj)>>(rows, cols, std::forward<Mat>(obj));
		}

		auto operator=(const mat&) noexcept(std::is_nothrow_copy_assignable_v<Buf>)
			-> mat&  = default;                                                                   // @TODO: ISSUE #20
		auto operator=(mat&&) noexcept(std::is_nothrow_move_assignable_v<Buf>) -> mat& = default; // @TODO: ISSUE #20

		template<detail::rng_1d_with_value_convertible_to<T> Rng>
		auto operator=(Rng&& rng) -> mat&
		{
			assign_rng_1d<detail::is_moved<decltype(rng)>>(std::forward<Rng>(rng));
			return *this;
		}

		template<detail::matrix_with_value_convertible_to<T> Mat>
		requires(!std::same_as<std::remove_cvref_t<Mat>, mat<T, Buf>>) auto operator=(Mat&& obj)
			-> mat& // @TODO: ISSUE #20
		{
			assign(std::forward<Mat>(obj));
			return *this;
		}
	};
} // namespace mpp