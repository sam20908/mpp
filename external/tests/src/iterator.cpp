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

#include <boost/ut.hpp>

#include <mpp/matrix.hpp>

#include "../../include/test_utilities.hpp"

using namespace boost::ut::literals;
using namespace boost::ut::bdd;
using namespace boost::ut;
using namespace mpp;

void test_iter_semantics(const auto& mat)
{
	auto begin  = mat.begin();
	auto cbegin = mat.cbegin();
	auto end    = mat.end() - 1;
	auto cend   = mat.cend() - 1;

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

	auto rbegin  = mat.rbegin();
	auto crbegin = mat.crbegin();
	auto rend    = mat.rend() - 1;
	auto crend   = mat.crend() - 1;

	expect(*rbegin == 6);
	expect(*crbegin == 6);
	expect(*rend == 1);
	expect(*crend == 1);

	auto begin_2 = mat.begin();
	begin_2 += { 1, 2 };
	expect(*begin_2 == 6);

	begin_2 -= { 1, 0 };
	expect(*begin_2 == 3);

	auto begin_3 = begin_2 + std::pair{ 0, 0 };
	expect(begin_3 == begin_2);
}

void test_iter_semantics_write(auto& mat)
{
	auto cbegin  = mat.cbegin();
	auto cend    = mat.cend() - 1;
	auto crbegin = mat.crbegin();
	auto crend   = mat.crend() - 1;

	expect(*cbegin == 1_i);
	expect(*cend == 6_i);
	expect(*crbegin == 6_i);
	expect(*crend == 1_i);

	auto begin  = mat.begin();
	auto end    = mat.end() - 1;
	auto rbegin = mat.rbegin();
	auto rend   = mat.rend() - 1;

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
	const auto mats_types = all_mats<int, 0, 0>{};

	const auto range_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };
	const auto mats     = std::tuple{ matrix<int, 2, 3>{ range_2d },
        matrix<int, dynamic, dynamic>{ range_2d },
        matrix<int, 2, dynamic>{ range_2d },
        matrix<int, dynamic, 3>{ range_2d } };

	feature("Standard conforming iterators") = []<typename Mat>(std::type_identity<Mat>) {
		using mat_iter               = typename Mat::iterator;
		using mat_reverse_iter       = typename Mat::iterator;
		using mat_const_iter         = typename Mat::const_iterator;
		using mat_const_reverse_iter = typename Mat::const_reverse_iterator;

		given("Mutable and immutable iterators should meet contiguous_iterator") = [&]() {
			expect(constant<std::contiguous_iterator<mat_iter>>);
			expect(constant<std::contiguous_iterator<mat_const_iter>>);
		};

		given("Mutable and immutable reverse iterators should meet bidirectional_iterator") = [&]() {
			expect(constant<std::bidirectional_iterator<mat_reverse_iter>>);
			expect(constant<std::bidirectional_iterator<mat_const_reverse_iter>>);
		};

		given("Mutable iterators should meet output_iterator") = [&]() {
			expect(constant<std::output_iterator<mat_iter, int>>);
		};
	} | mats_types;

	feature("Iterator semantics") = [&]() {
		given("Mutable and immutable iterators should have right semantics") = [](const auto& mat) {
			test_iter_semantics(mat);
		} | mats;

		given("Mutable iterators should have right mutable semantics") = [](const auto& mat) {
			// Explicitly allow mutating because we're not actually writing through const_* version of iterators but the
			// test somehow always pass the matrix as a const object
			test_iter_semantics_write(mut(mat));
		} | mats;
	};

	return 0;
}
