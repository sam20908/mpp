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
	 * Satisfies LegacyRandomAccessIterator
	 */
	template<typename Iterator>
	class matrix_iterator
	{
		Iterator _current;
		std::size_t _cols;

		using traits = std::iterator_traits<Iterator>;

	public:
		using value_type        = typename traits::value_type;
		using difference_type   = typename traits::difference_type;
		using pointer           = typename traits::pointer;
		using reference         = typename traits::reference;
		using iterator_category = typename traits::iterator_category;

		explicit matrix_iterator(Iterator current, std::size_t cols) :
			_current(current),
			_cols(cols) {} // @TODO: ISSUE #20

		matrix_iterator() = default; // @TODO: ISSUE #20

		[[nodiscard]] auto operator*() -> reference // @TODO: ISSUE #20
		{
			return *_current;
		}

		[[nodiscard]] auto operator*() const -> reference // @TODO: ISSUE #20
		{
			return *_current;
		}

		auto operator++() -> matrix_iterator& // @TODO: ISSUE #20
		{
			++_current;
			return *this;
		}

		auto operator++(int) -> matrix_iterator // @TODO: ISSUE #20
		{
			auto old = matrix_iterator<Iterator>(_current, _cols);

			++_current;
			return old;
		}

		auto operator+=(difference_type n) -> matrix_iterator& // @TODO: ISSUE #20
		{
			_current += n;
			return *this;
		}

		[[nodiscard]] friend auto operator+(const matrix_iterator& iter, difference_type n)
			-> matrix_iterator // @TODO: ISSUE #20
		{
			return matrix_iterator(iter._current + n, iter._cols);
		}

		[[nodiscard]] friend auto operator+(difference_type n, const matrix_iterator& iter)
			-> matrix_iterator // @TODO: ISSUE #20
		{
			return matrix_iterator(iter._current + n, iter._cols);
		}

		auto operator--() -> matrix_iterator& // @TODO: ISSUE #20
		{
			--_current;
			return *this;
		}

		auto operator--(int) -> matrix_iterator // @TODO: ISSUE #20
		{
			auto old = matrix_iterator<Iterator>(_current, _cols);

			--_current;
			return old;
		}

		auto operator-=(difference_type n) -> matrix_iterator& // @TODO: ISSUE #20
		{
			_current -= n;
			return *this;
		}

		[[nodiscard]] friend auto operator-(const matrix_iterator& iter, difference_type n)
			-> matrix_iterator // @TODO: ISSUE #20
		{
			return matrix_iterator(iter._current - n, iter._cols);
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
			return _current - right._current;
		}

		[[nodiscard]] auto operator==(const matrix_iterator&) const -> bool = default;
		[[nodiscard]] auto operator!=(const matrix_iterator&) const -> bool = default;
		[[nodiscard]] auto operator<=>(const matrix_iterator&) const        = default;

		friend inline void swap(matrix_iterator<Iterator>& left, matrix_iterator<Iterator> right) // @TODO: ISSUE #20
		{
			using std::swap;

			swap(left._current, right._current);
			swap(left._cols, right._cols);
		}

		/**
		 * Extra functionalities
		 */

		auto move_forward_rows(difference_type rows) -> matrix_iterator& // @TODO: ISSUE #20
		{
			_current += static_cast<difference_type>(_cols) * rows;
			return *this;
		}

		auto move_backward_rows(difference_type rows) -> matrix_iterator& // @TODO: ISSUE #20
		{
			_current -= static_cast<difference_type>(_cols) * rows;
			return *this;
		}
	};
} // namespace mpp::detail