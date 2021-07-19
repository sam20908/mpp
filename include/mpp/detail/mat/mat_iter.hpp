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

#include <compare>
#include <cstddef>
#include <iterator>

namespace mpp::detail
{
	/**
	 * Custom iterator with additional functionalities
	 *
	 * Satisfies contiguous_iterator
	 */
	template<typename It>
	class mat_iter
	{
		using traits = std::iterator_traits<It>;

		It current_;
		std::size_t cols_;

	public:
		using value_type        = typename traits::value_type;
		using difference_type   = typename traits::difference_type;
		using pointer           = typename traits::pointer;
		using reference         = typename traits::reference;
		using iterator_category = std::contiguous_iterator_tag; // STL doesn't use contiguous iterator tag explicitly,
																// but we do meet its requirements

		explicit mat_iter(It current, std::size_t cols) noexcept(std::is_nothrow_copy_constructible_v<It>) :
			current_(current),
			cols_(cols) // @TODO: ISSUE #20
		{
		}

		mat_iter() noexcept(std::is_nothrow_default_constructible_v<It>) = default; // @TODO: ISSUE #20

		[[nodiscard]] auto operator*() -> reference // @TODO: ISSUE #20
		{
			return *current_;
		}

		[[nodiscard]] auto operator*() const -> reference // @TODO: ISSUE #20
		{
			return *current_;
		}

		auto operator++() noexcept -> mat_iter& // @TODO: ISSUE #20
		{
			++current_;
			return *this;
		}

		auto operator++(int) noexcept -> mat_iter // @TODO: ISSUE #20
		{
			auto old = mat_iter<It>(current_, cols_);

			++current_;
			return old;
		}

		auto operator+=(difference_type n) noexcept -> mat_iter& // @TODO: ISSUE #20
		{
			current_ += n;
			return *this;
		}

		[[nodiscard]] friend auto operator+(const mat_iter& iter, difference_type n) -> mat_iter // @TODO: ISSUE #20
		{
			return mat_iter(iter.current_ + n, iter.cols_);
		}

		[[nodiscard]] friend auto operator+(difference_type n, const mat_iter& iter) -> mat_iter // @TODO: ISSUE #20
		{
			return mat_iter(iter.current_ + n, iter.cols_);
		}

		auto operator--() -> mat_iter& // @TODO: ISSUE #20
		{
			--current_;
			return *this;
		}

		auto operator--(int) -> mat_iter // @TODO: ISSUE #20
		{
			auto old = mat_iter<It>(current_, cols_);

			--current_;
			return old;
		}

		auto operator-=(difference_type n) -> mat_iter& // @TODO: ISSUE #20
		{
			current_ -= n;
			return *this;
		}

		[[nodiscard]] friend auto operator-(const mat_iter& iter, difference_type n) -> mat_iter // @TODO: ISSUE #20
		{
			return mat_iter(iter.current_ - n, iter.cols_);
		}

		[[nodiscard]] auto operator[](difference_type n) -> reference // @TODO: ISSUE #20
		{
			return *(*this + n);
		}

		[[nodiscard]] auto operator[](difference_type n) const -> reference // @TODO: ISSUE #20
		{
			return *(*this + n);
		}

		[[nodiscard]] auto operator-(const mat_iter& b) const -> difference_type // @TODO: ISSUE #20
		{
			return current_ - b.current_;
		}

		[[nodiscard]] auto operator->() -> It // @TODO: ISSUE #20
		{
			return current_;
		}

		[[nodiscard]] auto operator->() const -> It // @TODO: ISSUE #20
		{
			return current_;
		}

		[[nodiscard]] auto operator==(const mat_iter&) const -> bool                  = default;
		[[nodiscard]] auto operator!=(const mat_iter&) const -> bool                  = default;
		[[nodiscard]] auto operator<=>(const mat_iter&) const -> std::strong_ordering = default;

		friend inline void swap(mat_iter<It>& a,
			mat_iter<It> b) noexcept // @TODO: ISSUE #20
		{
			using std::swap;

			swap(a.current_, b.current_);
			swap(a.cols_, b.cols_);
		}

		/**
		 * Extra functionalities
		 */

		auto advance_fwd_rows(difference_type rows) -> mat_iter& // @TODO: ISSUE #20
		{
			current_ += static_cast<difference_type>(cols_) * rows;
			return *this;
		}

		auto advance_back_rows(difference_type rows) -> mat_iter& // @TODO: ISSUE #20
		{
			current_ -= static_cast<difference_type>(cols_) * rows;
			return *this;
		}

		[[nodiscard]] friend auto operator+(const mat_iter& iter, const std::pair<difference_type, difference_type>& p)
			-> mat_iter // @TODO: ISSUE #20
		{
			const auto offset = p.first * static_cast<difference_type>(iter.cols_) + p.second;
			return mat_iter(iter.current_ + offset, iter.cols_);
		}

		[[nodiscard]] friend auto operator+(const std::pair<difference_type, difference_type>& p,
			const mat_iter& iter) -> mat_iter // @TODO: ISSUE #20
		{
			const auto offset = p.first * static_cast<difference_type>(iter.cols_) + p.second;
			return mat_iter(iter.current_ + offset, iter.cols_);
		}

		[[nodiscard]] friend auto operator-(const mat_iter& iter, const std::pair<difference_type, difference_type>& p)
			-> mat_iter // @TODO: ISSUE #20
		{
			const auto offset = p.first * static_cast<difference_type>(iter.cols_) + p.second;
			return mat_iter(iter.current_ - offset, iter.cols_);
		}

		auto operator+=(const std::pair<difference_type, difference_type>& p) -> mat_iter& // @TODO: ISSUE #20
		{
			current_ += p.first * static_cast<difference_type>(cols_) + p.second;
			return *this;
		}

		auto operator-=(const std::pair<difference_type, difference_type>& p) -> mat_iter& // @TODO: ISSUE #20
		{
			current_ -= p.first * static_cast<difference_type>(cols_) + p.second;
			return *this;
		}
	};
} // namespace mpp::detail