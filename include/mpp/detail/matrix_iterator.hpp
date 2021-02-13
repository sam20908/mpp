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

		explicit matrix_iterator(Iterator current, std::size_t cols) : _current(current), _cols(cols) {}

		matrix_iterator() = default;

		[[nodiscard]] auto operator*() -> reference
		{
			return *_current;
		}

		[[nodiscard]] auto operator*() const -> reference
		{
			return *_current;
		}

		auto operator++() -> matrix_iterator&
		{
			++_current;
			return *this;
		}

		auto operator++(int) -> matrix_iterator
		{
			auto old = matrix_iterator<Iterator>(_current, _cols);

			++_current;
			return old;
		}

		auto operator+=(difference_type n) -> matrix_iterator&
		{
			_current += n;
			return *this;
		}

		[[nodiscard]] friend auto operator+(const matrix_iterator& iter, difference_type n) -> matrix_iterator
		{
			return matrix_iterator(iter._current + n, iter._cols);
		}

		[[nodiscard]] friend auto operator+(difference_type n, const matrix_iterator& iter) -> matrix_iterator
		{
			return matrix_iterator(iter._current + n, iter._cols);
		}

		auto operator--() -> matrix_iterator&
		{
			--_current;
			return *this;
		}

		auto operator--(int) -> matrix_iterator
		{
			auto old = matrix_iterator<Iterator>(_current, _cols);

			--_current;
			return old;
		}

		auto operator-=(difference_type n) -> matrix_iterator&
		{
			_current -= n;
			return *this;
		}

		[[nodiscard]] friend auto operator-(const matrix_iterator& iter, difference_type n) -> matrix_iterator
		{
			return matrix_iterator(iter._current - n, iter._cols);
		}

		[[nodiscard]] auto operator[](difference_type n) -> reference
		{
			return *(*this + n);
		}

		[[nodiscard]] auto operator[](difference_type n) const -> reference
		{
			return *(*this + n);
		}

		template<typename LeftIterator, typename RightIterator>
		[[nodiscard]] friend auto operator-(const matrix_iterator<LeftIterator>& left,
			const matrix_iterator<RightIterator>& right) -> decltype(left._current - right._current);

		template<typename LeftIterator, typename RightIterator>
		[[nodiscard]] friend auto operator==(const matrix_iterator<LeftIterator>& left,
			const matrix_iterator<RightIterator>& right) -> bool;

		template<typename LeftIterator, typename RightIterator>
		[[nodiscard]] friend auto operator!=(const matrix_iterator<LeftIterator>& left,
			const matrix_iterator<RightIterator>& right) -> bool;

		template<typename LeftIterator, typename RightIterator>
		[[nodiscard]] friend auto operator<=>(const matrix_iterator<LeftIterator>& left,
			const matrix_iterator<RightIterator>& right) -> decltype(left._current <=> right._current);

		friend void swap(matrix_iterator<Iterator>& left, matrix_iterator<Iterator> right)
		{
			using std::swap;

			swap(left._current, right._current);
			swap(left._cols, right._cols);
		}

		/**
		 * Extra functionalities
		 */

		// [[nodiscard]] auto move_forward_rows(difference_type rows)
		// {
		// 	_current += _cols * rows;
		// }
	};

	template<typename LeftIterator, typename RightIterator>
	[[nodiscard]] auto operator-(const matrix_iterator<LeftIterator>& left, const matrix_iterator<RightIterator>& right)
		-> decltype(left._current - right._current)
	{
		return left._current - right._current;
	}

	template<typename LeftIterator, typename RightIterator>
	[[nodiscard]] auto operator==(const matrix_iterator<LeftIterator>& left,
		const matrix_iterator<RightIterator>& right) -> bool
	{
		return left._current == right._current;
	}

	template<typename LeftIterator, typename RightIterator>
	[[nodiscard]] auto operator!=(const matrix_iterator<LeftIterator>& left,
		const matrix_iterator<RightIterator>& right) -> bool
	{
		return left._current != right._current;
	}

	template<typename LeftIterator, typename RightIterator>
	[[nodiscard]] auto operator<=>(const matrix_iterator<LeftIterator>& left,
		const matrix_iterator<RightIterator>& right) -> decltype(left._current <=> right._current)
	{
		return left._current <=> right._current;
	}
} // namespace mpp::detail