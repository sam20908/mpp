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
#include <mpp/detail/util/util.hpp>
#include <mpp/mat/matfwd.hpp>
#include <mpp/util/cfg.hpp>

#include <initializer_list>
#include <type_traits>
#include <utility>

namespace mpp
{
	template<detail::arithmetic Val, typename Alloc>
	class mat<Val, dyn, dyn, Alloc> :
		public detail::mat_dyn_base<mat<Val, dyn, dyn, Alloc>,
			typename cfg<override>::dyn_buf<Val, dyn, dyn, Alloc>,
			Val,
			dyn,
			dyn,
			Alloc>
	{
		using base = detail::mat_dyn_base<mat<Val, dyn, dyn, Alloc>,
			typename cfg<override>::dyn_buf<Val, dyn, dyn, Alloc>,
			Val,
			dyn,
			dyn,
			Alloc>;

	public:
		using base::operator=;

		mat() noexcept(std::is_default_constructible_v<Alloc>) : base(0, 0, Alloc{}) {} // @TODO: ISSUE #20

		explicit mat(const Alloc& alloc) noexcept : base(0, 0, alloc) {} // @TODO: ISSUE #20

		mat(const mat& right, const Alloc& alloc) :
			base(right.rows_, right.cols_, right.buf_, alloc) // @TODO: ISSUE #20
		{
		}

		mat(mat&& right, const Alloc& alloc) :
			base(std::move(right.rows_),
				std::move(right.cols_),
				std::move(right.buf_),
				alloc) // @TODO: ISSUE #20
		{
		}

		template<detail::matrix_with_value_convertible_to<Val> Mat>
		requires(!std::same_as<std::remove_cvref_t<Mat>, mat<Val, dyn, dyn, Alloc>>) explicit mat(Mat&& obj,
			const Alloc& alloc = Alloc{}) :
			base(0, 0, alloc) // @TODO: ISSUE #20
		{
			const auto rows = std::forward<Mat>(obj).rows();
			const auto cols = std::forward<Mat>(obj).cols();

			base::assign_rng_1d(rows, cols, std::forward<Mat>(obj));
		}

		template<detail::rng_1d_with_value_convertible_to<Val> Rng>
		explicit mat(std::size_t rows, std::size_t cols, Rng&& range, const Alloc& alloc = Alloc{}) :
			base(0, 0, alloc) // @TODO: ISSUE #20
		{
			base::assign_rng_1d(rows, cols, std::forward<Rng>(range));
		}

		template<std::convertible_to<Val> InitVal>
		explicit mat(std::initializer_list<std::initializer_list<InitVal>> init, const Alloc alloc = Alloc{}) :
			base(0, 0, alloc) // @TODO: ISSUE #20
		{
			base::assign_rng_2d(init);
		}

		template<detail::rng_2d_with_value_convertible_to<Val> Rng>
		explicit mat(Rng&& rng, const Alloc alloc = Alloc{}) : base(0, 0, alloc) // @TODO: ISSUE #20
		{
			base::assign_rng_2d(std::forward<Rng>(rng));
		}

		template<typename Expr, std::size_t ExprRowsExtent, std::size_t ExprCols>
		explicit mat(const detail::expr_base<Expr, Val, ExprRowsExtent, ExprCols>& expr, const Alloc alloc = Alloc{}) :
			base(0, 0, alloc) // @TODO: ISSUE #20
		{
			base::init_from_expr_dyn(expr.rows(), expr.cols(), expr);
		}

		mat(std::size_t rows, std::size_t cols, const Alloc& alloc = Alloc{}) :
			base(rows, cols, rows * cols, Val{}, alloc) // @TODO: ISSUE #20
		{
		}

		mat(std::size_t rows, std::size_t cols, const Val& val, const Alloc& alloc = Alloc{}) :
			base(rows, cols, rows * cols, val, alloc) // @TODO: ISSUE #20
		{
		}

		mat(std::size_t rows,
			std::size_t cols,
			identity_tag,
			const Val& zero_val = Val{ 0 },
			const Val& one_val  = Val{ 1 },
			const Alloc& alloc  = Alloc{}) :
			base(rows, cols, alloc) // @TODO: ISSUE #20
		{
			detail::init_identity_buf(base::buf_, rows, cols, zero_val, one_val);
		}

		// @FIXME: Allow fn's value return be convertible to value type
		template<detail::fn_with_return_type<Val> Fn>
		mat(std::size_t rows, std::size_t cols, Fn&& fn, const Alloc& alloc = Alloc{}) :
			base(rows, cols, alloc) // @TODO: ISSUE #20
		{
			base::init_from_fn_dyn(rows, cols, std::forward<Fn>(fn));
		}

		template<std::convertible_to<Val> InitVal>
		void assign(std::initializer_list<std::initializer_list<InitVal>> init) // @TODO: ISSUE #20
		{
			base::assign_rng_2d(init);
		}

		template<detail::rng_2d_with_value_convertible_to<Val> Rng>
		void assign(Rng&& rng) // @TODO: ISSUE #20
		{
			base::assign_rng_2d(std::forward<Rng>(rng));
		}

		template<detail::matrix_with_value_convertible_to<Val> Mat>
		requires(!std::same_as<std::remove_cvref_t<Mat>, mat<Val, dyn, dyn, Alloc>>) void assign(
			Mat&& obj) // @TODO: ISSUE #20
		{
			const auto rows = std::forward<Mat>(obj).rows();
			const auto cols = std::forward<Mat>(obj).cols();

			base::assign_rng_1d(rows, cols, std::forward<Mat>(obj));
		}
	};
} // namespace mpp
