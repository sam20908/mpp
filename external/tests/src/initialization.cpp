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

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4459)
#endif

#define APPROVALS_UT
#include <ApprovalTests.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <mpp/matrix.hpp>

#include "../../include/approvals_string_maker.hpp"
#include "../../include/custom_allocator.hpp"

#include <array>
#include <cstddef>
#include <initializer_list>
#include <vector>

int main()
{
	// @NOTE: Construction from expression object will be covered in lazy/eager arithmetic tests
	// @NOTE: This covers copy and move construction from rule of five (2/5)

	using namespace boost::ut::bdd;
	using namespace ApprovalTests;

	using MatrixApprover = TApprovals<ToStringCompileTimeOptions<expr_string_maker<false>>>;

	auto subdirectory_disposer = Approvals::useApprovalsSubdirectory("../approval_tests");

	// @NOTE: We need to define the range in the lambdas because the topmost lambda can't have captured notation,
	// otherwise ApprovalTests can't find the source file
	// Tracked by:https://github.com/approvals/ApprovalTests.cpp/issues/177

	feature("Default initialization") = []() {
		when("We do specify a custom allocator as a type") = []() {
			const auto matrix_1 = mpp::matrix<int, 0, 0>{};
			const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{};
			const auto matrix_3 = mpp::matrix<int, 0, mpp::dynamic>{};
			const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 0>{};

			MatrixApprover::verify(matrix_1);
			MatrixApprover::verify(matrix_2);
			MatrixApprover::verify(matrix_3);
			MatrixApprover::verify(matrix_4);
		};

		when("We do specify a custom allocator as a type") = []() {
			const auto matrix_1 = mpp::matrix<int, 0, 0, custom_allocator<int>>{};
			const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{};
			const auto matrix_3 = mpp::matrix<int, 0, mpp::dynamic, custom_allocator<int>>{};
			const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 0, custom_allocator<int>>{};

			MatrixApprover::verify(matrix_1);
			MatrixApprover::verify(matrix_2);
			MatrixApprover::verify(matrix_3);
			MatrixApprover::verify(matrix_4);
		};
	};

	feature("Initialize with custom allocator") = []() {
		const auto allocator = custom_allocator<int>{};

		const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{ allocator };
		const auto matrix_3 = mpp::matrix<int, 0, mpp::dynamic, custom_allocator<int>>{ allocator };
		const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 0, custom_allocator<int>>{ allocator };

		MatrixApprover::verify(matrix_2);
		MatrixApprover::verify(matrix_3);
		MatrixApprover::verify(matrix_4);
	};

	feature("2D range initialization") = []() {
		given("We're using a 2D initializer list") = [&]() {
			const auto range_2d = std::initializer_list<std::initializer_list<float>>{ { 1, 2, 3 }, { 4, 5, 6 } };

			when("We don't use unsafe") = [&]() {
				const auto matrix_1 = mpp::matrix<int, 2, 3>{ range_2d };
				const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ range_2d };
				const auto matrix_3 = mpp::matrix<int, 2, mpp::dynamic>{ range_2d };
				const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 3>{ range_2d };

				MatrixApprover::verify(matrix_1);
				MatrixApprover::verify(matrix_2);
				MatrixApprover::verify(matrix_3);
				MatrixApprover::verify(matrix_4);
			};

			when("We do use unsafe") = [&]() {
				const auto matrix_1 = mpp::matrix<int, 2, 3>{ range_2d, mpp::unsafe };
				const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ range_2d, mpp::unsafe };
				const auto matrix_3 = mpp::matrix<int, 2, mpp::dynamic>{ range_2d, mpp::unsafe };
				const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 3>{ range_2d, mpp::unsafe };

				MatrixApprover::verify(matrix_1);
				MatrixApprover::verify(matrix_2);
				MatrixApprover::verify(matrix_3);
				MatrixApprover::verify(matrix_4);
			};
		};

		given("We're using a 2D vector") = [&]() {
			const auto range_2d = std::vector<std::vector<float>>{ { 1, 2, 3 }, { 4, 5, 6 } };

			when("We don't use unsafe") = [&]() {
				const auto matrix_1 = mpp::matrix<int, 2, 3>{ range_2d };
				const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ range_2d };
				const auto matrix_3 = mpp::matrix<int, 2, mpp::dynamic>{ range_2d };
				const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 3>{ range_2d };

				MatrixApprover::verify(matrix_1);
				MatrixApprover::verify(matrix_2);
				MatrixApprover::verify(matrix_3);
				MatrixApprover::verify(matrix_4);
			};

			when("We do use unsafe") = [&]() {
				const auto matrix_1 = mpp::matrix<int, 2, 3>{ range_2d, mpp::unsafe };
				const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ range_2d, mpp::unsafe };
				const auto matrix_3 = mpp::matrix<int, 2, mpp::dynamic>{ range_2d, mpp::unsafe };
				const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 3>{ range_2d, mpp::unsafe };

				MatrixApprover::verify(matrix_1);
				MatrixApprover::verify(matrix_2);
				MatrixApprover::verify(matrix_3);
				MatrixApprover::verify(matrix_4);
			};
		};

		given("We're using a 2D array for fully static matrices") = []() {
			auto range_2d = std::array<std::array<float, 3>, 2>{ { { 1, 2, 3 }, { 4, 5, 6 } } };

			then("We copy initialize it") = [&]() {
				auto matrix = mpp::matrix<int, 2, 3>{};
				matrix      = range_2d;

				MatrixApprover::verify(matrix);
			};

			then("We move initialize it") = [&]() {
				auto matrix = mpp::matrix<int, 2, 3>{};
				matrix      = std::move(range_2d);

				MatrixApprover::verify(matrix);
			};
		};
	};

	feature("1D range initialization") = []() {
		const auto range_1d = std::vector<int>{ 1, 2, 3, 4, 5, 6 };

		when("We don't use unsafe") = [&]() {
			const auto matrix_1 = mpp::matrix<int, 2, 3>{ 2, 3, range_1d };
			const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ 2, 3, range_1d };
			const auto matrix_3 = mpp::matrix<int, 2, mpp::dynamic>{ 2, 3, range_1d };
			const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 3>{ 2, 3, range_1d };

			MatrixApprover::verify(matrix_1);
			MatrixApprover::verify(matrix_2);
			MatrixApprover::verify(matrix_3);
			MatrixApprover::verify(matrix_4);
		};

		when("We do use unsafe") = [&]() {
			const auto matrix_1 = mpp::matrix<int, 2, 3>{ 2, 3, range_1d, mpp::unsafe };
			const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ 2, 3, range_1d, mpp::unsafe };
			const auto matrix_3 = mpp::matrix<int, 2, mpp::dynamic>{ 2, 3, range_1d, mpp::unsafe };
			const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 3>{ 2, 3, range_1d, mpp::unsafe };

			MatrixApprover::verify(matrix_1);
			MatrixApprover::verify(matrix_2);
			MatrixApprover::verify(matrix_3);
			MatrixApprover::verify(matrix_4);
		};
	};

	feature("Callable for initialization") = []() {
		auto iota_fn = []() {
			return [i = 1]() mutable {
				return i++;
			};
		};

		const auto matrix_1 = mpp::matrix<int, 2, 3>{ iota_fn() };
		const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ 2, 3, iota_fn() };
		const auto matrix_3 = mpp::matrix<int, 2, mpp::dynamic>{ 3, iota_fn() };
		const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 3>{ 2, iota_fn() };

		MatrixApprover::verify(matrix_1);
		MatrixApprover::verify(matrix_2);
		MatrixApprover::verify(matrix_3);
		MatrixApprover::verify(matrix_4);
	};

	feature("Value initialization") = []() {
		const auto matrix_1 = mpp::matrix<int, 2, 3>{ 1 };
		const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ 2, 3, 1 };
		const auto matrix_3 = mpp::matrix<int, 2, mpp::dynamic>{ 3, 1 };
		const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 3>{ 2, 1 };

		MatrixApprover::verify(matrix_1);
		MatrixApprover::verify(matrix_2);
		MatrixApprover::verify(matrix_3);
		MatrixApprover::verify(matrix_4);
	};

	feature("Constructing identity matrices") = []() {
		when("We don't use unsafe") = []() {
			const auto matrix_1 = mpp::matrix<int, 3, 3>{ mpp::identity };
			const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ 3, 3, mpp::identity };
			const auto matrix_3 = mpp::matrix<int, 3, mpp::dynamic>{ 3, mpp::identity };
			const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 3>{ 3, mpp::identity };

			MatrixApprover::verify(matrix_1);
			MatrixApprover::verify(matrix_2);
			MatrixApprover::verify(matrix_3);
			MatrixApprover::verify(matrix_4);
		};

		when("We do use unsafe") = []() {
			const auto matrix_1 = mpp::matrix<int, 3, 3>{ mpp::identity, mpp::unsafe };
			const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ 3, 3, mpp::identity, mpp::unsafe };
			const auto matrix_3 = mpp::matrix<int, 3, mpp::dynamic>{ 3, mpp::identity, mpp::unsafe };
			const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 3>{ 3, mpp::identity, mpp::unsafe };

			MatrixApprover::verify(matrix_1);
			MatrixApprover::verify(matrix_2);
			MatrixApprover::verify(matrix_3);
			MatrixApprover::verify(matrix_4);
		};
	};

	feature("Copy initialization") = []() {
		when("Without custom allocator") = []() {
			const auto matrix_1 = mpp::matrix<int, 0, 0>{};
			const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{};
			const auto matrix_3 = mpp::matrix<int, 0, mpp::dynamic>{};
			const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 0>{};

			const auto matrix_1_copy = mpp::matrix<int, 0, 0>{ matrix_1 };
			const auto matrix_2_copy = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ matrix_2 };
			const auto matrix_3_copy = mpp::matrix<int, 0, mpp::dynamic>{ matrix_3 };
			const auto matrix_4_copy = mpp::matrix<int, mpp::dynamic, 0>{ matrix_4 };

			MatrixApprover::verify(matrix_1_copy);
			MatrixApprover::verify(matrix_2_copy);
			MatrixApprover::verify(matrix_3_copy);
			MatrixApprover::verify(matrix_4_copy);
		};

		when("With custom allocators (dyamic matrices only)") = []() {
			const auto allocator = custom_allocator<int>{};

			const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{};
			const auto matrix_3 = mpp::matrix<int, 0, mpp::dynamic, custom_allocator<int>>{};
			const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 0, custom_allocator<int>>{};

			const auto matrix_2_copy =
				mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{ matrix_2, allocator };
			const auto matrix_3_copy = mpp::matrix<int, 0, mpp::dynamic, custom_allocator<int>>{ matrix_3, allocator };
			const auto matrix_4_copy = mpp::matrix<int, mpp::dynamic, 0, custom_allocator<int>>{ matrix_4, allocator };

			MatrixApprover::verify(matrix_2_copy);
			MatrixApprover::verify(matrix_3_copy);
			MatrixApprover::verify(matrix_4_copy);
		};
	};

	feature("Move initialization") = []() {
		when("Without custom allocator") = []() {
			auto matrix_1 = mpp::matrix<int, 0, 0>{};
			auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{};
			auto matrix_3 = mpp::matrix<int, 0, mpp::dynamic>{};
			auto matrix_4 = mpp::matrix<int, mpp::dynamic, 0>{};

			const auto matrix_1_move = mpp::matrix<int, 0, 0>{ std::move(matrix_1) };
			const auto matrix_2_move = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ std::move(matrix_2) };
			const auto matrix_3_move = mpp::matrix<int, 0, mpp::dynamic>{ std::move(matrix_3) };
			const auto matrix_4_move = mpp::matrix<int, mpp::dynamic, 0>{ std::move(matrix_4) };

			MatrixApprover::verify(matrix_1_move);
			MatrixApprover::verify(matrix_2_move);
			MatrixApprover::verify(matrix_3_move);
			MatrixApprover::verify(matrix_4_move);
		};

		when("With custom allocators (dyamic matrices only)") = []() {
			const auto allocator = custom_allocator<int>{};

			auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{};
			auto matrix_3 = mpp::matrix<int, 0, mpp::dynamic, custom_allocator<int>>{};
			auto matrix_4 = mpp::matrix<int, mpp::dynamic, 0, custom_allocator<int>>{};

			const auto matrix_2_move =
				mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{ std::move(matrix_2), allocator };
			const auto matrix_3_move =
				mpp::matrix<int, 0, mpp::dynamic, custom_allocator<int>>{ std::move(matrix_3), allocator };
			const auto matrix_4_move =
				mpp::matrix<int, mpp::dynamic, 0, custom_allocator<int>>{ std::move(matrix_4), allocator };

			MatrixApprover::verify(matrix_2_move);
			MatrixApprover::verify(matrix_3_move);
			MatrixApprover::verify(matrix_4_move);
		};
	};

	feature("Different matrix type initialization") = []() {
		const auto matrix_1 = mpp::matrix<int, 0, 0>{};
		const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{};
		const auto matrix_3 = mpp::matrix<int, 0, mpp::dynamic>{};
		const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 0>{};

		const auto matrix_1_dummy = mpp::matrix<int, 0, 0>{ std::move(matrix_4) };
		const auto matrix_2_dummy = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ std::move(matrix_3) };
		const auto matrix_3_dummy = mpp::matrix<int, 0, mpp::dynamic>{ std::move(matrix_2) };
		const auto matrix_4_dummy = mpp::matrix<int, mpp::dynamic, 0>{ std::move(matrix_1) };

		MatrixApprover::verify(matrix_1_dummy);
		MatrixApprover::verify(matrix_2_dummy);
		MatrixApprover::verify(matrix_3_dummy);
		MatrixApprover::verify(matrix_4_dummy);
	};

	return 0;
}
