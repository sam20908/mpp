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

#include <mpp/detail/types/algo_types.hpp>
#include <mpp/detail/utility/algorithm_helpers.hpp>
#include <mpp/detail/utility/buffer_manipulators.hpp>
#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/utility/comparison.hpp>
#include <mpp/utility/square.hpp>
#include <mpp/matrix.hpp>

#include <concepts>
#include <future>
#include <type_traits>

namespace mpp
{
	namespace detail
	{
		template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] inline auto inverse_lu_decomp(const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> matrix<To, RowsExtent, ColumnsExtent> // @TODO: ISSUE #20
		{
			const auto rows    = obj.rows();
			const auto columns = obj.columns();

			using inverse_matrix_t = matrix<To, RowsExtent, ColumnsExtent>;

			// Handle special cases - avoid LU Decomposition
			if (rows == 0)
			{
				return inverse_matrix_t{};
			}

			auto inverse_matrix_buffer = typename inverse_matrix_t::buffer_type{};
			allocate_buffer_if_vector(inverse_matrix_buffer, rows, columns, To{});

			using default_floating_type_ordering_type =
				std::compare_three_way_result_t<default_floating_type, default_floating_type>;

			if (rows == 1)
			{
				const auto elem = static_cast<To>(obj(0, 0));

				using to_ordering_type = std::compare_three_way_result_t<To, To>;

				if (floating_point_compare(elem, To{}) == to_ordering_type::equivalent)
				{
					throw std::runtime_error("Inverse of a singular matrix doesn't exist!");
				}

				inverse_matrix_buffer[0] = To{ 1 } / elem;
			}

			auto det = default_floating_type{ 1 };

			if (rows == 2)
			{
				const auto element_1 = static_cast<default_floating_type>(obj(1, 1));
				const auto element_2 = static_cast<default_floating_type>(obj(0, 1));
				const auto element_3 = static_cast<default_floating_type>(obj(1, 0));
				const auto element_4 = static_cast<default_floating_type>(obj(0, 0));

				const auto ad = element_4 * element_1;
				const auto bc = element_2 * element_3;

				det = ad - bc;

				if (floating_point_compare(det, default_floating_type{}) ==
					default_floating_type_ordering_type::equivalent)
				{
					throw std::runtime_error("Inverse of a singular matrix doesn't exist!");
				}

				const auto multiplier = default_floating_type{ 1 } / det;

				inverse_matrix_buffer[0] = static_cast<To>(multiplier * element_1);
				inverse_matrix_buffer[1] = static_cast<To>(multiplier * element_2 * -1);
				inverse_matrix_buffer[2] = static_cast<To>(multiplier * element_3 * -1);
				inverse_matrix_buffer[3] = static_cast<To>(multiplier * element_4);
			}

			if (rows >= 3)
			{
				using lu_decomp_matrix_t = matrix<default_floating_type, RowsExtent, ColumnsExtent>;
				using lu_decomp_buffer_t = typename lu_decomp_matrix_t::buffer_type;

				auto l_buffer = lu_decomp_buffer_t{};
				auto u_buffer = lu_decomp_buffer_t{};

				allocate_buffer_if_vector(u_buffer, rows, columns, default_floating_type{});
				std::ranges::copy(obj, u_buffer.begin());

				allocate_buffer_if_vector(l_buffer, rows, columns, default_floating_type{});
				// @TODO: Allow the user to control one_value and zero_value here?
				make_identity_buffer<false>(l_buffer, rows, columns, default_floating_type{}, default_floating_type{});

				det = lu_decomposition_and_compute_determinant_in_place<default_floating_type, true>(rows,
					columns,
					l_buffer,
					u_buffer);

				if (floating_point_compare(det, default_floating_type{}) ==
					default_floating_type_ordering_type::equivalent)
				{
					throw std::runtime_error("inverseerse of a singular matrix doesn't exist!");
				}

				if (std::is_constant_evaluated())
				{
					forward_substitution_in_place(l_buffer, columns);
					back_substitution_in_place(u_buffer, columns);
				}
				else
				{
					// Compute inv(L) and inv(U) in parallel because they don't share data

					auto l_inverse_future = std::async(std::launch::async, [&l_buffer, columns]() {
						forward_substitution_in_place(l_buffer, columns);
					});
					auto u_inverse_future = std::async(std::launch::async, [&u_buffer, columns]() {
						back_substitution_in_place(u_buffer, columns);
					});

					l_inverse_future.wait();
					u_inverse_future.wait();
				}

				matrix_multiplication_on_buffers<To, default_floating_type>(inverse_matrix_buffer,
					std::move(u_buffer),
					std::move(l_buffer),
					rows,
					columns,
					rows,
					columns);
			}

			return inverse_matrix_t{ rows, columns, std::move(inverse_matrix_buffer) };
		}

		template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] inline auto inverse_common(const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> matrix<To, RowsExtent, ColumnsExtent> // @TODO: ISSUE #20
		{
			if (!square(obj))
			{
				throw std::runtime_error("inverseerse of a non-square matrix doesn't exist!");
			}

			return inverse_lu_decomp<To>(obj);
		}
	} // namespace detail

	struct inverse_t : public detail::cpo_base<inverse_t>
	{
		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(inverse_t, const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> matrix<detail::default_floating_type, RowsExtent, ColumnsExtent> // @TODO: ISSUE #20
		{
			return detail::inverse_common<detail::default_floating_type>(obj);
		}

		template<std::floating_point To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto
		tag_invoke(inverse_t, std::type_identity<To>, const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> matrix<To, RowsExtent, ColumnsExtent> // @TODO: ISSUE #20
		{
			return detail::inverse_common<To>(obj);
		}
	};

	inline constexpr auto inverse = inverse_t{};
} // namespace mpp
