/***
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

#define APPROVALS_UT
#include <ApprovalTests.hpp>

#include <mpp/matrix.hpp>

#include "../../include/approvals_string_maker.hpp"

#include <initializer_list>
#include <vector>

int main()
{
	// @NOTE: This covers copy and move assignment from rule of five (2/5)

	using namespace boost::ut::bdd;
	using namespace ApprovalTests;

	using MatrixApprover       = TApprovals<ToStringCompileTimeOptions<expr_string_maker<false>>>;
	using MatrixExtentApprover = TApprovals<ToStringCompileTimeOptions<expr_string_maker<true>>>;

	auto subdirectory_disposer = Approvals::useApprovalsSubdirectory("../approval_tests");

	// @NOTE: We need to define the range in the lambdas because the topmost lambda can't have captured notation,
	// otherwise ApprovalTests can't find the source file
	// Tracked by:https://github.com/approvals/ApprovalTests.cpp/issues/177

	scenario("Assigning from a bigger 2D range") = []() {
		// Test insertion machinery

		when("Given a 2D initializer list") = []() {
			const auto big_range_2d =
				std::initializer_list<std::initializer_list<int>>{ { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 11, 22, 33, 44, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };

			auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{};
			auto matrix_3 = mpp::matrix<int, 10, mpp::dynamic>{};
			auto matrix_4 = mpp::matrix<int, mpp::dynamic, 10>{};

			matrix_2 = big_range_2d;
			matrix_3 = big_range_2d;
			matrix_4 = big_range_2d;

			MatrixApprover::verify(matrix_2);
			MatrixApprover::verify(matrix_3);
			MatrixApprover::verify(matrix_4);
		};

		when("Given a 2D vector") = []() {
			const auto big_range_2d = std::vector<std::vector<int>>{ { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 11, 22, 33, 44, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } } };

			auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{};
			auto matrix_3 = mpp::matrix<int, 10, mpp::dynamic>{};
			auto matrix_4 = mpp::matrix<int, mpp::dynamic, 10>{};

			matrix_2 = big_range_2d;
			matrix_3 = big_range_2d;
			matrix_4 = big_range_2d;

			MatrixApprover::verify(matrix_2);
			MatrixApprover::verify(matrix_3);
			MatrixApprover::verify(matrix_4);
		};
	};

	// These shrinking tests are also assignment tests

	when("Shrinking a fully dynamic matrix") = []() {
		const auto big_range_2d = std::vector<std::vector<int>>{ { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 11, 22, 33, 44, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } } };

		const auto small_range_2d = std::vector<std::vector<int>>{};

		auto matrix = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ big_range_2d };
		matrix      = small_range_2d;

		MatrixApprover::verify(matrix);
	};

	when("Shrinking a dynamic rows matrix") = []() {
		const auto big_range_2d = std::vector<std::vector<int>>{ { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 11, 22, 33, 44, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } } };

		const auto small_range_2d = std::vector<std::vector<int>>{ { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };

		auto matrix = mpp::matrix<int, mpp::dynamic, 10>{ big_range_2d };
		matrix      = small_range_2d;

		MatrixApprover::verify(matrix);
	};

	when("Shrinking a dynamic columns matrix") = []() {
		const auto big_range_2d = std::vector<std::vector<int>>{ { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 11, 22, 33, 44, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } } };

		const auto small_range_2d =
			std::vector<std::vector<int>>{ { 1 }, { 2 }, { 3 }, { 4 }, { 5 }, { 6 }, { 7 }, { 8 }, { 9 }, { 10 } };

		auto matrix = mpp::matrix<int, 10, mpp::dynamic>{ big_range_2d };
		matrix      = small_range_2d;

		MatrixApprover::verify(matrix);
	};

	when("Assigning a fully static matrix") = []() {
		given("A 2D initializer list") = []() {
			const auto initializer_list_2d =
				std::initializer_list<std::initializer_list<int>>{ { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 11, 22, 33, 44, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };

			auto matrix = mpp::matrix<int, 10, 10>{};
			matrix      = initializer_list_2d;

			MatrixExtentApprover::verify(matrix);
		};

		given("A 2D array") = []() {
			auto range_2d = std::array<std::array<int, 10>, 10>{ { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 11, 22, 33, 44, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } } };

			then("We copy assign it") = [&]() {
				auto matrix = mpp::matrix<int, 10, 10>{};
				matrix      = range_2d;

				MatrixExtentApprover::verify(matrix);
			};

			then("We move assign it") = [&]() {
				auto matrix = mpp::matrix<int, 10, 10>{};
				matrix      = std::move(range_2d);

				MatrixExtentApprover::verify(matrix);
			};
		};

		given("A 2D vector") = []() {
			const auto range_2d = std::vector<std::vector<int>>{ { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 11, 22, 33, 44, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };

			auto matrix = mpp::matrix<int, 10, 10>{};
			matrix      = range_2d;

			MatrixExtentApprover::verify(matrix);
		};
	};

	feature("Copy assignment") = []() {
		const auto range_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

		when("Copy assigning a fully static matrix") = [&]() {
			const auto matrix_1 = mpp::matrix<int, 2, 3>{ range_2d };
			auto matrix_2       = mpp::matrix<int, 2, 3>{};

			matrix_2 = matrix_1;

			MatrixApprover::verify(matrix_2);
		};

		when("Copy assigning a fully dynamic matrix") = [&]() {
			const auto matrix_1 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ range_2d };
			auto matrix_2       = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{};

			matrix_2 = matrix_1;

			MatrixApprover::verify(matrix_2);
		};

		when("Copy assigning a dynamic rows matrix") = [&]() {
			const auto matrix_1 = mpp::matrix<int, mpp::dynamic, 3>{ range_2d };
			auto matrix_2       = mpp::matrix<int, mpp::dynamic, 3>{};

			matrix_2 = matrix_1;

			MatrixApprover::verify(matrix_2);
		};

		when("Copy assigning a dynamic columns matrix") = [&]() {
			const auto matrix_1 = mpp::matrix<int, 2, mpp::dynamic>{ range_2d };
			auto matrix_2       = mpp::matrix<int, 2, mpp::dynamic>{};

			matrix_2 = matrix_1;

			MatrixApprover::verify(matrix_2);
		};
	};

	feature("Move assignment") = []() {
		const auto range_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

		when("Move assigning a fully static matrix") = [&]() {
			auto matrix_1 = mpp::matrix<int, 2, 3>{ range_2d };
			auto matrix_2 = mpp::matrix<int, 2, 3>{};

			matrix_2 = std::move(matrix_1);

			MatrixApprover::verify(matrix_2);
		};

		when("Move assigning a fully dynamic matrix") = [&]() {
			auto matrix_1 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ range_2d };
			auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{};

			matrix_2 = std::move(matrix_1);

			MatrixApprover::verify(matrix_2);
		};

		when("Move assigning a dynamic rows matrix") = [&]() {
			auto matrix_1 = mpp::matrix<int, mpp::dynamic, 3>{ range_2d };
			auto matrix_2 = mpp::matrix<int, mpp::dynamic, 3>{};

			matrix_2 = std::move(matrix_1);

			MatrixApprover::verify(matrix_2);
		};

		when("Move assigning a dynamic columns matrix") = [&]() {
			auto matrix_1 = mpp::matrix<int, 2, mpp::dynamic>{ range_2d };
			auto matrix_2 = mpp::matrix<int, 2, mpp::dynamic>{};

			matrix_2 = std::move(matrix_1);

			MatrixApprover::verify(matrix_2);
		};
	};

	feature("Different matrix type assignment") = []() {
		const auto matrix_1 = mpp::matrix<int, 0, 0>{};
		const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{};
		const auto matrix_3 = mpp::matrix<int, 0, mpp::dynamic>{};
		const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 0>{};

		auto matrix_1_dummy = mpp::matrix<int, 0, 0>{ std::move(matrix_4) };
		auto matrix_2_dummy = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ std::move(matrix_3) };
		auto matrix_3_dummy = mpp::matrix<int, 0, mpp::dynamic>{ std::move(matrix_2) };
		auto matrix_4_dummy = mpp::matrix<int, mpp::dynamic, 0>{ std::move(matrix_1) };

		matrix_1_dummy = matrix_4;
		matrix_2_dummy = matrix_3;
		matrix_3_dummy = matrix_2;
		matrix_4_dummy = matrix_1;

		MatrixApprover::verify(matrix_1_dummy);
		MatrixApprover::verify(matrix_2_dummy);
		MatrixApprover::verify(matrix_3_dummy);
		MatrixApprover::verify(matrix_4_dummy);
	};

	return 0;
}
