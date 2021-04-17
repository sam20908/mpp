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

	// scenario("Comparing matrices") = [&]() {
	// auto test_fn = [&]<typename LeftValue,
	// typename RightValue,
	// std::size_t LeftRows,
	// std::size_t LeftColumns,
	// std::size_t RightRows,
	// std::size_t RightColumns>() {
	// const auto matrix_1   = mpp::matrix<LeftValue, LeftRows, LeftColumns>{ range_2d };
	// const auto matrix_1_1 = mpp::matrix<RightValue, RightRows, RightColumns>{ range_2d };
	// compare_matrix_to_matrix(matrix_1, matrix_1_1);

	// const auto matrix_1_2 = mpp::matrix<RightValue, mpp::dynamic, mpp::dynamic>{ range_2d };
	// compare_matrix_to_matrix(matrix_1, matrix_1_2);

	// const auto matrix_1_3 = mpp::matrix<RightValue, RightRows, mpp::dynamic>{ range_2d };
	// compare_matrix_to_matrix(matrix_1, matrix_1_3);

	// const auto matrix_1_4 = mpp::matrix<RightValue, mpp::dynamic, RightColumns>{ range_2d };
	// compare_matrix_to_matrix(matrix_1, matrix_1_4);
	//};
	// test_fn.template operator()<int, int, 2, 3, 2, 3>();
	// test_fn.template operator()<double, short, 2, 3, 2, 3>(); // Test different value type comparison
	//};

	//// @NOTE: Copy and move assigning matrices is already covered in above assignment test

	// scenario("Matrices should be destructible") = [&]() {
	// expect(constant<std::is_destructible_v<mpp::matrix<int, 2, 3>>>);
	// expect(constant<std::is_destructible_v<mpp::matrix<int, mpp::dynamic, mpp::dynamic>>>);
	// expect(constant<std::is_destructible_v<mpp::matrix<int, mpp::dynamic, 3>>>);
	// expect(constant<std::is_destructible_v<mpp::matrix<int, 2, mpp::dynamic>>>);
	//};
	//};

	// scenario("Test member functions") = [&]() {
	// scenario("Element access functions") = [&]() {
	// auto test_fn = [&]<typename Value10, std::size_t Rows10, std::size_t Columns10>() {
	// auto matrix = mpp::matrix<Value10, Rows10, Columns10>{ range_2d };

	// expect(*matrix.data() == 1_i);
	//*matrix.data() = 2;
	// expect(*matrix.data() == 2_i);

	// expect(matrix.at(0, 0) == 2_i); // Modified above
	// expect(matrix(1, 2) == 6_i);
	//};

	// test_fn.template operator()<int, 2, 3>();
	// test_fn.template operator()<int, mpp::dynamic, mpp::dynamic>();
	// test_fn.template operator()<int, 2, mpp::dynamic>();
	// test_fn.template operator()<int, mpp::dynamic, 3>();
	//};

	// scenario("More element access functions") = [&]() {
	// auto test_fn = [&]<typename Value11, std::size_t Rows11, std::size_t Columns11>() {
	// auto matrix         = mpp::matrix<Value11, Rows11, Columns11>{ range_2d };
	// const auto matrix_2 = mpp::matrix<Value11, Rows11, Columns11>{ range_2d };

	// expect(matrix_2.front() == 1_i);
	// expect(matrix_2.back() == 6_i);

	// matrix.front() = 2;
	// matrix.back()  = 2;

	// expect(matrix.front() == 2_i);
	// expect(matrix.back() == 2_i);

	// expect(matrix.size() == 6_ul);
	// expect(!matrix.empty());

	//(void)matrix.max_size(); // Due to outside conditions, max_size is very hard to test, but call it
	//// for test coverage
	//};

	// test_fn.template operator()<int, 2, 3>();
	// test_fn.template operator()<int, mpp::dynamic, mpp::dynamic>();
	// test_fn.template operator()<int, 2, mpp::dynamic>();
	// test_fn.template operator()<int, mpp::dynamic, 3>();
	//};

	// scenario("Swapping matrices") = [&]() {
	// auto test_fn = [&]<typename Value12, std::size_t Rows12, std::size_t Columns12>() {
	// auto matrix   = mpp::matrix<Value12, Rows12, Columns12>{ range_2d };
	// auto matrix_2 = mpp::matrix<Value12, Rows12, Columns12>{ range_2d };

	// matrix.swap(matrix_2);

	// compare_matrix_to_range_2d(matrix, range_2d, 2, 3);
	// compare_matrix_to_range_2d(matrix_2, range_2d, 2, 3);
	//};

	// test_fn.template operator()<int, 2, 3>();
	// test_fn.template operator()<int, mpp::dynamic, mpp::dynamic>();
	// test_fn.template operator()<int, 2, mpp::dynamic>();
	// test_fn.template operator()<int, mpp::dynamic, 3>();
	//};

	// scenario("Getting allocator for dynamic matrices") = []() {
	// const auto allocator = custom_allocator<int>{};

	// const auto matrix_1 = mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{ allocator };
	// const auto matrix_2 = mpp::matrix<int, 2, mpp::dynamic, custom_allocator<int>>{ allocator };
	// const auto matrix_3 = mpp::matrix<int, mpp::dynamic, 3, custom_allocator<int>>{ allocator };

	// expect(matrix_1.get_allocator() == allocator);
	// expect(matrix_2.get_allocator() == allocator);
	// expect(matrix_3.get_allocator() == allocator);
	//};

	// scenario("Clearing dynamic matrices") = [&]() {
	// auto matrix_1 = mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{ range_2d };
	// auto matrix_2 = mpp::matrix<int, 2, mpp::dynamic, custom_allocator<int>>{ range_2d };
	// auto matrix_3 = mpp::matrix<int, mpp::dynamic, 3, custom_allocator<int>>{ range_2d };

	// matrix_1.clear();
	// matrix_2.clear();
	// matrix_3.clear();

	// expect(matrix_1.empty());
	// expect(matrix_2.empty());
	// expect(matrix_3.empty());
	//};
	//};
	//};
}
