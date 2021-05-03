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
	template<typename Iterator>
	class matrix_iterator
	{
		using traits = std::iterator_traits<Iterator>;

		Iterator current_;
		std::size_t columns_;

	public:
		using value_type        = typename traits::value_type;
		using difference_type   = typename traits::difference_type;
		using pointer           = typename traits::pointer;
		using reference         = typename traits::reference;
		using iterator_category = std::contiguous_iterator_tag; // STL doesn't use contiguous iterator tag explicitly,
																// but we do meet its requirements

		explicit matrix_iterator(Iterator current, std::size_t columns) noexcept(
			std::is_nothrow_copy_constructible_v<Iterator>) :
			current_(current),
			columns_(columns) // @TODO: ISSUE #20
		{
		}

		matrix_iterator() noexcept(std::is_nothrow_default_constructible_v<Iterator>) = default; // @TODO: ISSUE #20

		[[nodiscard]] auto operator*() -> reference // @TODO: ISSUE #20
		{
			return *current_;
		}

		[[nodiscard]] auto operator*() const -> reference // @TODO: ISSUE #20
		{
			return *current_;
		}

		auto operator++() noexcept -> matrix_iterator& // @TODO: ISSUE #20
		{
			++current_;
			return *this;
		}

		auto operator++(int) noexcept -> matrix_iterator // @TODO: ISSUE #20
		{
			auto old = matrix_iterator<Iterator>(current_, columns_);

			++current_;
			return old;
		}

		auto operator+=(difference_type n) noexcept -> matrix_iterator& // @TODO: ISSUE #20
		{
			current_ += n;
			return *this;
		}

		[[nodiscard]] friend auto operator+(const matrix_iterator& iter, difference_type n)
			-> matrix_iterator // @TODO: ISSUE #20
		{
			return matrix_iterator(iter.current_ + n, iter.columns_);
		}

		[[nodiscard]] friend auto operator+(difference_type n, const matrix_iterator& iter)
			-> matrix_iterator // @TODO: ISSUE #20
		{
			return matrix_iterator(iter.current_ + n, iter.columns_);
		}

		auto operator--() -> matrix_iterator& // @TODO: ISSUE #20
		{
			--current_;
			return *this;
		}

		auto operator--(int) -> matrix_iterator // @TODO: ISSUE #20
		{
			auto old = matrix_iterator<Iterator>(current_, columns_);

			--current_;
			return old;
		}

		auto operator-=(difference_type n) -> matrix_iterator& // @TODO: ISSUE #20
		{
			current_ -= n;
			return *this;
		}

		[[nodiscard]] friend auto operator-(const matrix_iterator& iter, difference_type n)
			-> matrix_iterator // @TODO: ISSUE #20
		{
			return matrix_iterator(iter.current_ - n, iter.columns_);
		}

		[[nodiscard]] auto operator[](difference_type n) -> reference // @TODO: ISSUE #20
		{
			return *(*this + n);
		}

		[[nodiscard]] auto operator[](difference_type n) const -> reference // @TODO: ISSUE #20
		{
			return *(*this + n);
		}

		[[nodiscard]] auto operator-(const matrix_iterator& right) const -> difference_type // @TODO: ISSUE #20
		{
			return current_ - right.current_;
		}

		[[nodiscard]] auto operator->() -> Iterator // @TODO: ISSUE #20
		{
			return current_;
		}

		[[nodiscard]] auto operator->() const -> Iterator // @TODO: ISSUE #20
		{
			return current_;
		}

		[[nodiscard]] auto operator==(const matrix_iterator&) const -> bool                  = default;
		[[nodiscard]] auto operator!=(const matrix_iterator&) const -> bool                  = default;
		[[nodiscard]] auto operator<=>(const matrix_iterator&) const -> std::strong_ordering = default;

		friend inline void swap(matrix_iterator<Iterator>& left,
			matrix_iterator<Iterator> right) noexcept // @TODO: ISSUE #20
		{
			using std::swap;

			swap(left.current_, right.current_);
			swap(left.columns_, right.columns_);
		}

		/**
		 * Extra functionalities
		 */

		auto move_forward_rows(difference_type rows) -> matrix_iterator& // @TODO: ISSUE #20
		{
			current_ += static_cast<difference_type>(columns_) * rows;
			return *this;
		}

		auto move_backward_rows(difference_type rows) -> matrix_iterator& // @TODO: ISSUE #20
		{
			current_ -= static_cast<difference_type>(columns_) * rows;
			return *this;
		}

		[[nodiscard]] friend auto operator+(const matrix_iterator& iter,
			const std::pair<difference_type, difference_type>& movement) -> matrix_iterator // @TODO: ISSUE #20
		{
			const auto offset = movement.first * static_cast<difference_type>(iter.columns_) + movement.second;
			return matrix_iterator(iter.current_ + offset, iter.columns_);
		}

		[[nodiscard]] friend auto operator+(const std::pair<difference_type, difference_type>& movement,
			const matrix_iterator& iter) -> matrix_iterator // @TODO: ISSUE #20
		{
			const auto offset = movement.first * static_cast<difference_type>(iter.columns_) + movement.second;
			return matrix_iterator(iter.current_ + offset, iter.columns_);
		}

		[[nodiscard]] friend auto operator-(const matrix_iterator& iter,
			const std::pair<difference_type, difference_type>& movement) -> matrix_iterator // @TODO: ISSUE #20
		{
			const auto offset = movement.first * static_cast<difference_type>(iter.columns_) + movement.second;
			return matrix_iterator(iter.current_ - offset, iter.columns_);
		}

		auto operator+=(const std::pair<difference_type, difference_type>& movement)
			-> matrix_iterator& // @TODO: ISSUE #20
		{
			current_ += movement.first * static_cast<difference_type>(columns_) + movement.second;
			return *this;
		}

		auto operator-=(const std::pair<difference_type, difference_type>& movement)
			-> matrix_iterator& // @TODO: ISSUE #20
		{
			current_ -= movement.first * static_cast<difference_type>(columns_) + movement.second;
			return *this;
		}
	};
} // namespace mpp::detail