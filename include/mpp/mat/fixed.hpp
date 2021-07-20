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

#include <mpp/detail/mat/mat_base.hpp>
#include <mpp/detail/utility/utility.hpp>
#include <mpp/mat/matfwd.hpp>
#include <mpp/utility/cfg.hpp>

#include <initializer_list>
#include <type_traits>
#include <utility>

namespace mpp
{
	template<detail::arithmetic Val, std::size_t Rows, std::size_t Cols, typename Alloc>
	class mat :
		public detail::mat_base<mat<Val, Rows, Cols, Alloc>,
			typename cfg<override>::fixed_buf<Val, Rows, Cols, Alloc>,
			Val,
			Rows,
			Cols,
			Alloc>
	{
		using base = detail::mat_base<mat<Val, Rows, Cols, Alloc>,
			typename cfg<override>::fixed_buf<Val, Rows, Cols, Alloc>,
			Val,
			Rows,
			Cols,
			Alloc>;

		void fill_buffer_with_value(const auto& val) // @TODO: ISSUE #20
		{
			std::ranges::fill(base::buf_, val);
		}

	public:
		using base::operator=;

		mat() : base(Rows, Cols)
		{
			fill_buffer_with_value(Val{});
		}

		template<detail::matrix_with_value_convertible_to<Val> Mat>
		requires(!std::same_as<std::remove_cvref_t<Mat>, mat<Val, Rows, Cols, Alloc>>) explicit mat(Mat&& obj) :
			base(Rows, Cols) // @TODO: ISSUE #20
		{
			const auto rows = std::forward<Mat>(obj).rows();
			const auto cols = std::forward<Mat>(obj).cols();

			base::assign_rng_1d(rows, cols, std::forward<Mat>(obj));
		}

		template<detail::rng_1d_with_value_convertible_to<Val> Rng>
		explicit mat(std::size_t rows, std::size_t cols, Rng&& rng) : base(Rows, Cols) // @TODO: ISSUE #20
		{
			base::assign_rng_1d(rows, cols, std::forward<Rng>(rng));
		}

		template<std::convertible_to<Val> InitVal>
		explicit mat(std::initializer_list<std::initializer_list<InitVal>> init) : base(Rows, Cols) // @TODO: ISSUE #20
		{
			base::assign_rng_2d(init);
		}

		template<detail::rng_2d_with_value_convertible_to<Val> Rng>
		explicit mat(Rng&& rng) : base(Rows, Cols) // @TODO: ISSUE #20
		{
			base::assign_rng_2d(std::forward<Rng>(rng));
		}

		template<std::convertible_to<Val> ArrVal>
		explicit mat(const std::array<std::array<ArrVal, Cols>, Rows>& arr) : base(Rows, Cols)
		{
			base::assign_rng_2d<false, false, false>(arr);
		}

		template<std::convertible_to<Val> ArrVal>
		explicit mat(std::array<std::array<ArrVal, Cols>, Rows>&& arr) : base(Rows, Cols)
		{
			base::assign_rng_2d<false, false, false>(std::move(arr));
		}


		template<typename Expr, std::size_t ExprRows, std::size_t ExprCols>
		explicit mat(const detail::expr_base<Expr, Val, ExprRows, ExprCols>& expr) :
			base(Rows, Cols) // @TODO: ISSUE #20
		{
			for (auto row = std::size_t{}, index = std::size_t{}; row < Rows; ++row)
			{
				for (auto column = std::size_t{}; column < Cols; ++column)
				{
					base::buf_[index++] = expr(row, column);
				}
			}
		}

		explicit mat(const Val& val) : base(Rows, Cols) // @TODO: ISSUE #20
		{
			fill_buffer_with_value(val);
		}

		explicit mat(identity_tag, const Val& zero_val = Val{ 0 }, const Val& one_val = Val{ 1 }) :
			base(Rows, Cols) // @TODO: ISSUE #20
		{
			detail::init_identity_buf(base::buf_, Rows, Cols, zero_val, one_val);
		}

		// @FIXME: Allow callable's value return be convertible to value type
		template<detail::fn_with_return_type<Val> Fn>
		explicit mat(Fn&& fn) : base(Rows, Cols) // @TODO: ISSUE #20
		{
			std::ranges::generate(base::buf_, std::forward<Fn>(fn));
		}

		template<std::convertible_to<Val> InitVal>
		void assign(std::initializer_list<std::initializer_list<InitVal>> init) // @TODO: ISSUE #20
		{
			base::assign_rng_2d(init);
		}

		template<detail::rng_2d_with_value_convertible_to<Val> Rng>
		void assign(Rng&& range_2d) // @TODO: ISSUE #20
		{
			base::assign_rng_2d(std::forward<Rng>(range_2d));
		}

		template<std::convertible_to<Val> ArrVal>
		void assign(const std::array<std::array<ArrVal, Cols>, Rows>& arr)
		{
			base::assign_rng_2d(arr);
		}

		template<std::convertible_to<Val> ArrVal>
		void assign(std::array<std::array<ArrVal, Cols>, Rows>&& arr)
		{
			base::assign_rng_2d(std::move(arr));
		}

		template<detail::matrix_with_value_convertible_to<Val> Mat>
		requires(!std::same_as<std::remove_cvref_t<Mat>, mat<Val, Rows, Cols, Alloc>>) void assign(
			Mat&& obj) // @TODO: ISSUE #20
		{
			const auto rows = std::forward<Mat>(obj).rows();
			const auto cols = std::forward<Mat>(obj).cols();

			base::assign_rng_1d(rows, cols, std::forward<Mat>(obj));
		}
	};
} // namespace mpp
