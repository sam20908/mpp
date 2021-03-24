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

#include "../../thirdparty/ut.hpp"

using namespace boost::ut::literals;
using namespace boost::ut::operators::terse;
using namespace boost::ut::bdd;
using namespace boost::ut;

template<std::size_t RowsExtent, std::size_t ColumnsExtent>
void test_iter_semantics(const auto& range_2d, auto... dimension_args)
{
	const auto matrix = mpp::matrix<int, RowsExtent, ColumnsExtent>{ range_2d, dimension_args... };

	auto begin  = matrix.begin();
	auto cbegin = matrix.cbegin();
	auto end    = matrix.end() - 1;
	auto cend   = matrix.cend() - 1;

	expect(*begin == 1);
	expect(*cbegin == 1);
	expect(*end == 6);
	expect(*cend == 6);

	auto begin_old  = begin++;
	auto cbegin_old = cbegin++;

	expect(*begin == 2);
	expect(*cbegin == 2);
	expect(*begin_old == 1);
	expect(*cbegin_old == 1);

	++begin_old;
	++cbegin_old;

	expect(begin == begin_old);
	expect(cbegin == cbegin_old);

	begin += 2;
	cbegin += 2;
	end -= 2;
	cend -= 2;

	expect(begin == end);
	expect(cbegin == cend);

	auto rbegin  = matrix.rbegin();
	auto crbegin = matrix.crbegin();
	auto rend    = matrix.rend() - 1;
	auto crend   = matrix.crend() - 1;

	expect(*rbegin == 6);
	expect(*crbegin == 6);
	expect(*rend == 1);
	expect(*crend == 1);

	auto begin_2 = matrix.begin();
	begin_2 += { 1, 2 };
	expect(*begin_2 == 6);

	begin_2 -= { 1, 0 };
	expect(*begin_2 == 3);

	auto begin_3 = begin_2 + std::pair{ 0, 0 };
	expect(begin_3 == begin_2);
}

template<std::size_t RowsExtent, std::size_t ColumnsExtent>
void test_iter_semantics_write(const auto& range_2d)
{
	auto matrix = mpp::matrix<int, RowsExtent, ColumnsExtent>{ range_2d };

	auto begin  = matrix.begin();
	auto end    = matrix.end() - 1;
	auto rbegin = matrix.rbegin();
	auto rend   = matrix.rend() - 1;

	*begin = 2;
	*end   = 2;
	expect(*begin == 2);
	expect(*end == 2);

	*rbegin = 3;
	*rend   = 4;
	expect(*end == 3);
	expect(*begin == 4);
}

int main()
{
	feature("matrix iterators") = []() {
		given("it should meet named iterator requirements") = []() {
			// No need to use partially dynamic matrices since they still have dynamic buffer iterators

			using static_matrix  = mpp::matrix<int, 1, 1>;
			using dynamic_matrix = mpp::matrix<int>;

			using static_iterator        = typename static_matrix::iterator;
			using static_const_iterator  = typename static_matrix::const_iterator;
			using dynamic_iterator       = typename dynamic_matrix::iterator;
			using dynamic_const_iterator = typename dynamic_matrix::const_iterator;

			expect(constant<std::contiguous_iterator<static_iterator>>);
			expect(constant<std::contiguous_iterator<static_const_iterator>>);
			expect(constant<std::contiguous_iterator<dynamic_iterator>>);
			expect(constant<std::contiguous_iterator<dynamic_const_iterator>>);

			expect(constant<std::output_iterator<static_iterator, int>>);
			expect(constant<std::output_iterator<dynamic_iterator, int>>);
		};

		given("it should have the right iterator semantics") = []() {
			// @NOTE: Every test is referencing the below range_2d for convenience
			const auto range_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

			test_iter_semantics<2, 3>(range_2d);
			test_iter_semantics<std::dynamic_extent, std::dynamic_extent>(range_2d);
			test_iter_semantics<std::dynamic_extent, 3>(range_2d);
			test_iter_semantics<2, std::dynamic_extent>(range_2d);

			test_iter_semantics_write<2, 3>(range_2d);
			test_iter_semantics_write<std::dynamic_extent, std::dynamic_extent>(range_2d);
			test_iter_semantics_write<std::dynamic_extent, 3>(range_2d);
			test_iter_semantics_write<2, std::dynamic_extent>(range_2d);
		};
	};

	return 0;
}