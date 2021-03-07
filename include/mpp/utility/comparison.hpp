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

#include <mpp/matrix.hpp>

#include "mpp/detail/utility.hpp"

#include <compare>
#include <cstddef>
#include <span>
#include <type_traits>
#include <utility>

namespace mpp
{
	namespace detail
	{
		struct size_compare_t
		{
			friend inline void tag_invoke(size_compare_t, ...) // @TODO: ISSUE #20
			{
				static_assert(R"(Custom overload of "size_compare" is required for custom types!)");
			}

			template<typename LeftValue,
				typename RightValue,
				std::size_t LeftRowsExtent,
				std::size_t LeftColumnsExtent,
				std::size_t RightRowsExtent,
				std::size_t RightColumnsExtent>
			[[nodiscard]] friend inline auto tag_invoke(size_compare_t,
				const matrix<LeftValue, LeftRowsExtent, LeftColumnsExtent>& left,
				const matrix<RightValue, RightRowsExtent, RightColumnsExtent>& right,
				bool compare_rows,
				bool compare_columns) -> std::pair<std::partial_ordering, std::partial_ordering> // @TODO: ISSUE #20
			{
				return std::pair{ compare_rows ? left.rows() <=> right.rows() : std::partial_ordering::unordered,
					compare_columns ? left.columns() <=> right.columns() : std::partial_ordering::unordered };
			}

			template<typename... Args>
			[[nodiscard]] auto operator()(Args&&... args) const
				-> detail::tag_invoke_impl::tag_invoke_result_t<size_compare_t, Args...> // @TODO: ISSUE #20
			{
				return detail::tag_invoke_cpo(*this, std::forward<Args>(args)...);
			}
		};

		struct elements_exact_compare_t
		{
			friend inline void tag_invoke(elements_exact_compare_t, ...) // @TODO: ISSUE #20
			{
				static_assert(R"(Custom overload of "elements_exact_compare" is required for custom types!)");
			}

			template<typename Value,
				std::size_t LeftRowsExtent,
				std::size_t LeftColumnsExtent,
				std::size_t RightRowsExtent,
				std::size_t RightColumnsExtent>
			[[nodiscard]] friend inline auto tag_invoke(elements_exact_compare_t,
				const matrix<Value, LeftRowsExtent, LeftColumnsExtent>& left,
				const matrix<Value, RightRowsExtent, RightColumnsExtent>& right)
				-> std::strong_ordering // @TODO: ISSUE #20
			{
				return left <=> right;
			}

			template<typename... Args>
			[[nodiscard]] auto operator()(Args&&... args) const
				-> detail::tag_invoke_impl::tag_invoke_result_t<elements_exact_compare_t, Args...> // @TODO: ISSUE #20
			{
				return detail::tag_invoke_cpo(*this, std::forward<Args>(args)...);
			}
		};

		struct elements_approximate_equal_t
		{
			friend inline void tag_invoke(elements_approximate_equal_t, ...) // @TODO: ISSUE #20
			{
				static_assert(R"(Custom overload of "elements_approximate_equal" is required for custom types!)");
			}

			template<typename Value,
				std::size_t LeftRowsExtent,
				std::size_t LeftColumnsExtent,
				std::size_t RightRowsExtent,
				std::size_t RightColumnsExtent>
			[[nodiscard]] friend inline auto tag_invoke(elements_approximate_equal_t,
				const matrix<Value, LeftRowsExtent, LeftColumnsExtent>& left,
				const matrix<Value, RightRowsExtent, RightColumnsExtent>& right) -> decltype(auto) // @TODO: ISSUE #20
			{
				const auto end = left.cend();

				for (auto left_it = left.cbegin(), right_it = right.cbegin(); left_it != end; ++left_it, ++right_it)
				{
					const auto ordering = accurate_equals(*left_it, *right_it);

					if (ordering != 0)
					{
						return ordering;
					}
				}

				if constexpr (std::is_floating_point_v<Value>)
				{
					return std::partial_ordering::equivalent;
				}
				else
				{
					// Every ordering type has "equivalent" member constant, so it's safe to use it for potentially
					// different ordering types
					using left_matrix_type  = matrix<Value, LeftRowsExtent, LeftColumnsExtent>;
					using right_matrix_type = matrix<Value, RightRowsExtent, RightColumnsExtent>;
					using compare_three_way_result =
						std::compare_three_way_result_t<left_matrix_type, right_matrix_type>;

					return typename compare_three_way_result::equivalent{};
				}
			}

			template<typename... Args>
			[[nodiscard]] auto operator()(Args&&... args) const
				-> detail::tag_invoke_impl::tag_invoke_result_t<elements_approximate_equal_t,
					Args...> // @TODO: ISSUE #20
			{
				return detail::tag_invoke_cpo(*this, std::forward<Args>(args)...);
			}
		};

	} // namespace detail

	inline constexpr auto size_compare               = detail::size_compare_t{};
	inline constexpr auto elements_exact_equal       = detail::elements_exact_compare_t{};
	inline constexpr auto elements_approximate_equal = detail::elements_approximate_equal_t{};
} // namespace mpp
