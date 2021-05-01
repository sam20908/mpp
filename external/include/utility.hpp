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

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4459)
#endif

#include <boost/ut.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif


#include <mpp/utility/comparison.hpp>
#include <mpp/matrix.hpp>

#include "utility.hpp"

#include <compare>
#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>

using namespace boost::ut;

template<class... Ts>
struct overloaded : Ts...
{
	using Ts::operator()...;
};

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template<typename Fn, typename... BackArgs>
struct bind_back_wrap
{
	/**
	 * @NOTE: Cannot use abbreviated function template parameters
	 * Issue tracked at https://github.com/microsoft/vscode-cpptools/issues/7446
	 */

	const Fn& fn;
	std::tuple<BackArgs...> back_args;

	template<typename... Args>
	constexpr bind_back_wrap(const Fn& fn, Args&&... args) : fn(fn), back_args{ std::forward<Args>(args)... }
	{
	}

	template<typename... Args>
	constexpr auto operator()(Args&&... args) const
	{
		return std::apply(fn, std::tuple_cat(std::forward_as_tuple(std::forward<Args>(args)...), back_args));
	}
};

template<typename Fn, typename... Args>
constexpr auto bind_back(const Fn& fn, Args&&... args)
{
	return bind_back_wrap<Fn, Args...>(fn, std::forward<Args>(args)...);
}

template<typename Val, std::size_t Rows, std::size_t Columns, typename... Ts>
using all_mats_t = std::tuple<mpp::matrix<Val, Rows, Columns, Ts...>,
	mpp::matrix<Val, mpp::dynamic, mpp::dynamic, Ts...>,
	mpp::matrix<Val, Rows, mpp::dynamic, Ts...>,
	mpp::matrix<Val, mpp::dynamic, Columns, Ts...>>;

template<typename Val, std::size_t Rows, std::size_t Columns, typename... Ts>
using dyn_mats_t = std::tuple<mpp::matrix<Val, mpp::dynamic, mpp::dynamic, Ts...>,
	mpp::matrix<Val, Rows, mpp::dynamic, Ts...>,
	mpp::matrix<Val, mpp::dynamic, Columns, Ts...>>;

inline auto fwd_args = []<typename T>(types<T>, auto&&... args) {
	return T{ args... };
};

inline auto args = [](const auto& fn, auto&&... args) {
	return bind_back(fn, std::forward<decltype(args)>(args)...);
};

void cmp_mat_to_rng(const auto& mat, const auto& rng)
{
	using mat_val_t = typename std::remove_cvref_t<decltype(mat)>::value_type;
	using rng_val_t = typename std::remove_cvref_t<decltype(rng)>::value_type::value_type;
	using ordering  = std::compare_three_way_result_t<mat_val_t, rng_val_t>;

	const auto expected_rows    = rng.size();
	const auto expected_columns = expected_rows == 0 ? 0 : rng[0].size();

	const auto rows_is_eq    = mat.rows() == expected_rows;
	const auto columns_is_eq = mat.columns() == expected_columns;

	expect(rows_is_eq);
	expect(columns_is_eq);

	if (!rows_is_eq || !columns_is_eq)
	{
		return; // Avoid accessing out of bounds
	}

	for (auto row = std::size_t{}; row < expected_rows; ++row)
	{
		for (auto column = std::size_t{}; column < expected_columns; ++column)
		{
			if (mpp::floating_point_compare(mat(row, column), rng[row][column]) != ordering::equivalent)
			{
				expect(false); // Elements are not equal
				break;
			}
		}
	}

	expect(true);
}


void cmp_mat_to_expr_like(const auto& mat, const auto& expr)
{
	using mat_val_t  = typename std::remove_cvref_t<decltype(mat)>::value_type;
	using expr_val_t = typename std::remove_cvref_t<decltype(expr)>::value_type;
	using ordering   = std::compare_three_way_result_t<mat_val_t, expr_val_t>;

	const auto rows_is_eq    = mat.rows() == expr.rows();
	const auto columns_is_eq = mat.columns() == expr.columns();

	expect(rows_is_eq);
	expect(columns_is_eq);

	if (!rows_is_eq || !columns_is_eq)
	{
		return; // Avoid accessing out of bounds
	}

	for (auto row = std::size_t{}; row < expr.rows(); ++row)
	{
		for (auto column = std::size_t{}; column < expr.columns(); ++column)
		{
			if (mpp::floating_point_compare(mat(row, column), expr(row, column)) != ordering::equivalent)
			{
				expect(false); // Elements are not equal
				break;
			}
		}
	}

	expect(true);
}

template<typename... Ts>
void for_each_in_tup(const std::tuple<Ts...>& tup, const auto& fn)
{
	[&]<std::size_t... Idx>(std::index_sequence<Idx...>)
	{
		(fn(std::get<Idx>(tup)), ...);
	}
	(std::index_sequence_for<Ts...>{});
}

template<typename Val,
	std::size_t Rows,
	std::size_t Columns,
	template<typename, std::size_t, std::size_t, typename...>
	typename mats_t,
	typename... Ts>
auto create_mats(const auto& fn)
{
	using tup_t = mats_t<Val, Rows, Columns, Ts...>;

	const auto tup = [&]<std::size_t... Idx>(std::index_sequence<Idx...>)
	{
		return std::tuple{ fn(types<std::tuple_element_t<Idx, tup_t>>{})... };
	}
	(std::make_index_sequence<std::tuple_size_v<tup_t>>{});

	return tup;
}