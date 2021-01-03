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

namespace matrixpp
{
	template<detail::arithmetic Value, std::size_t RowsExtent>
	class matrix<Value, RowsExtent, std::dynamic_extent> :
		public detail::matrix_base<std::vector<Value>, Value, RowsExtent, std::dynamic_extent>
	{
		// Static rows dynamic columns matrix

		using base = detail::matrix_base<std::vector<Value>, Value, RowsExtent, std::dynamic_extent>;

	public:
		using base::base;

		explicit matrix(std::initializer_list<std::initializer_list<Value>> init_2d) // @TODO: ISSUE #20
		{
			auto [rows, cols] = detail::range_2d_dimensions(init_2d);

			if (rows != RowsExtent)
			{
				throw std::invalid_argument("2D initializer's rows does not match the provided row extent!");
			}

			if (detail::dimension_not_zero_and_non_zero(RowsExtent, cols))
			{
				throw std::invalid_argument("Cannot have one side being zero and other side being non-zero!");
			}

			base::init_buf_2d_dynamic(init_2d, RowsExtent, cols);
		}

		explicit matrix(detail::range_2d_with_type<Value> auto&& rng_2d) // @TODO: ISSUE #20
		{
			auto [rows, cols] = detail::range_2d_dimensions(rng_2d);

			if (rows != RowsExtent)
			{
				throw std::invalid_argument("2D initializer's rows does not match the provided row extent!");
			}

			if (detail::dimension_not_zero_and_non_zero(RowsExtent, cols))
			{
				throw std::invalid_argument("Cannot have one side being zero and other side being non-zero!");
			}

			using decayed_rng_2d_t = std::decay_t<decltype(rng_2d)>;
			base::init_buf_2d_dynamic(std::forward<decayed_rng_2d_t>(rng_2d), RowsExtent, cols);
		}

		matrix(std::size_t columns, Value value) // @TODO: ISSUE #20
		{
			base::_rows = RowsExtent;
			base::_cols = columns;

			detail::allocate_1d_buf_if_vector(base::_buf, RowsExtent, columns);
			std::ranges::fill(base::_buf, value);
		}
	};
} // namespace matrixpp