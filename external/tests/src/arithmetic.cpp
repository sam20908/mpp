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

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4459)
#endif

#define APPROVALS_UT
#include <ApprovalTests.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <mpp/arithmetic.hpp>
#include <mpp/matrix.hpp>

#include "../../include/approvals_string_maker.hpp"
//#include "../../include/utility.hpp"

using namespace boost::ut::bdd;
using namespace boost::ut;
using namespace ApprovalTests;

template<typename LeftValue,
	typename RightValue,
	std::size_t LeftRows,
	std::size_t LeftColumns,
	std::size_t RightRows,
	std::size_t RightColumns,
	typename Approver>
void test_mat_op(const auto& rng, const auto& binary_op)
{
	const auto mat_1 = mpp::matrix<LeftValue, LeftRows, LeftColumns>{ rng };
	const auto mat_2 = mpp::matrix<RightValue, RightRows, RightColumns>{ rng };

	Approver::verify(binary_op(mat_1, mat_2));
}

template<typename Value, std::size_t Rows, std::size_t Columns, typename Approver>
void test_num_op(const auto& rng, const auto& number, const auto& binary_op)
{
	const auto matrix = mpp::matrix<Value, Rows, Columns>{ rng };

	Approver::verify(binary_op(matrix, number));
}

template<typename LeftValue,
	typename RightValue,
	std::size_t LeftRows,
	std::size_t LeftColumns,
	std::size_t RightRows,
	std::size_t RightColumns,
	typename Approver>
void test_mat_mul_op(const auto& left_rng, const auto& right_rng, const auto& binary_op)
{
	const auto mat_1 = mpp::matrix<LeftValue, LeftRows, LeftColumns>{ left_rng };
	const auto mat_2 = mpp::matrix<RightValue, RightRows, RightColumns>{ right_rng };

	Approver::verify(binary_op(mat_1, mat_2));
}

int main()
{
	using MatrixApprover       = TApprovals<ToStringCompileTimeOptions<expr_string_maker<false>>>;
	using MatrixExtentApprover = TApprovals<ToStringCompileTimeOptions<expr_string_maker<true>>>;

	auto subdirectory_disposer = Approvals::useApprovalsSubdirectory("../test_data/approval_tests");

	// @NOTE: We need to define the range in the lambdas because the topmost lambda can't have captured notation,
	// otherwise ApprovalTests can't find the source file
	// Tracked by:https://github.com/approvals/ApprovalTests.cpp/issues/177

	feature("Adding matrices") = []() {
		const auto rng = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

		when("We don't evaluate the full result") = [&]() {
			auto op = [](const auto& left, const auto& right) {
				return left + right;
			};

			test_mat_op<int, int, 2, 3, mpp::dynamic, mpp::dynamic, MatrixExtentApprover>(rng, op);
			test_mat_op<int, int, mpp::dynamic, mpp::dynamic, 2, 3, MatrixExtentApprover>(rng, op);
			test_mat_op<int, int, mpp::dynamic, 3, 2, mpp::dynamic, MatrixExtentApprover>(rng, op);
			test_mat_op<int, int, 2, mpp::dynamic, mpp::dynamic, 3, MatrixExtentApprover>(rng, op);
		};

		when("We do evaluate the full result") = [&]() {
			given("Static matrix extents should promote") = [&]() {
				auto op = [](const auto& left, const auto& right) {
					return mpp::matrix{ left + right };
				};

				test_mat_op<int, int, 2, 3, mpp::dynamic, mpp::dynamic, MatrixExtentApprover>(rng, op);
				test_mat_op<int, int, mpp::dynamic, mpp::dynamic, 2, 3, MatrixExtentApprover>(rng, op);
				test_mat_op<int, int, mpp::dynamic, 3, 2, mpp::dynamic, MatrixExtentApprover>(rng, op);
				test_mat_op<int, int, 2, mpp::dynamic, mpp::dynamic, 3, MatrixExtentApprover>(rng, op);
			};
		};
	};

	feature("Subtracting matrices") = []() {
		const auto rng = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

		when("We don't evaluate the full result") = [&]() {
			auto op = [](const auto& left, const auto& right) {
				return left - right;
			};

			test_mat_op<int, int, 2, 3, mpp::dynamic, mpp::dynamic, MatrixExtentApprover>(rng, op);
			test_mat_op<int, int, mpp::dynamic, mpp::dynamic, 2, 3, MatrixExtentApprover>(rng, op);
			test_mat_op<int, int, mpp::dynamic, 3, 2, mpp::dynamic, MatrixExtentApprover>(rng, op);
			test_mat_op<int, int, 2, mpp::dynamic, mpp::dynamic, 3, MatrixExtentApprover>(rng, op);
		};

		when("We do evaluate the full result") = [&]() {
			given("Static matrix extents should promote") = [&]() {
				auto op = [](const auto& left, const auto& right) {
					return mpp::matrix{ left - right };
				};

				test_mat_op<int, int, 2, 3, mpp::dynamic, mpp::dynamic, MatrixExtentApprover>(rng, op);
				test_mat_op<int, int, mpp::dynamic, mpp::dynamic, 2, 3, MatrixExtentApprover>(rng, op);
				test_mat_op<int, int, mpp::dynamic, 3, 2, mpp::dynamic, MatrixExtentApprover>(rng, op);
				test_mat_op<int, int, 2, mpp::dynamic, mpp::dynamic, 3, MatrixExtentApprover>(rng, op);
			};
		};
	};

	feature("Multiplying matrices with a number") = []() {
		const auto rng = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

		given("We don't evaluate the full result") = [&]() {
			auto op = [](const auto& left, const auto& right) {
				return left * right;
			};

			test_num_op<int, 2, 3, MatrixApprover>(rng, 2, op);
			test_num_op<int, mpp::dynamic, mpp::dynamic, MatrixApprover>(rng, 2, op);
			test_num_op<int, mpp::dynamic, 3, MatrixApprover>(rng, 2, op);
			test_num_op<int, 2, mpp::dynamic, MatrixApprover>(rng, 2, op);
		};

		given("We do evaluate the full result") = [&]() {
			auto op = [](const auto& left, const auto& right) {
				return mpp::matrix{ left * right };
			};

			test_num_op<int, 2, 3, MatrixApprover>(rng, 2, op);
			test_num_op<int, mpp::dynamic, mpp::dynamic, MatrixApprover>(rng, 2, op);
			test_num_op<int, mpp::dynamic, 3, MatrixApprover>(rng, 2, op);
			test_num_op<int, 2, mpp::dynamic, MatrixApprover>(rng, 2, op);
		};
	};

	feature("Multiplying matrices with another matrix") = []() {
		const auto left_rng  = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };
		const auto right_rng = std::vector<std::vector<int>>{ { 3 }, { 6 }, { 9 } };

		given("We don't evaluate the full result") = [&]() {
			auto op = [](const auto& left, const auto& right) {
				return left * right;
			};

			test_mat_mul_op<int, int, 2, 3, mpp::dynamic, mpp::dynamic, MatrixApprover>(left_rng, right_rng, op);
			test_mat_mul_op<int, int, mpp::dynamic, mpp::dynamic, 3, 1, MatrixApprover>(left_rng, right_rng, op);
			test_mat_mul_op<int, int, mpp::dynamic, 3, 3, mpp::dynamic, MatrixApprover>(left_rng, right_rng, op);
			test_mat_mul_op<int, int, 2, mpp::dynamic, mpp::dynamic, 1, MatrixApprover>(left_rng, right_rng, op);
		};

		given("We do evaluate the full result") = [&]() {
			auto op = [](const auto& left, const auto& right) {
				return mpp::matrix{ left * right };
			};

			test_mat_mul_op<int, int, 2, 3, mpp::dynamic, mpp::dynamic, MatrixApprover>(left_rng, right_rng, op);
			test_mat_mul_op<int, int, mpp::dynamic, mpp::dynamic, 3, 1, MatrixApprover>(left_rng, right_rng, op);
			test_mat_mul_op<int, int, mpp::dynamic, 3, 3, mpp::dynamic, MatrixApprover>(left_rng, right_rng, op);
			test_mat_mul_op<int, int, 2, mpp::dynamic, mpp::dynamic, 1, MatrixApprover>(left_rng, right_rng, op);
		};
	};

	feature("Dividing matrices with a number") = []() {
		const auto rng = std::vector<std::vector<int>>{ { 2, 4, 6 }, { 8, 10, 12 } };

		given("We don't evaluate the full result") = [&]() {
			auto op = [](const auto& left, const auto& right) {
				return left / right;
			};

			test_num_op<int, 2, 3, MatrixApprover>(rng, 2, op);
			test_num_op<int, mpp::dynamic, mpp::dynamic, MatrixApprover>(rng, 2, op);
			test_num_op<int, mpp::dynamic, 3, MatrixApprover>(rng, 2, op);
			test_num_op<int, 2, mpp::dynamic, MatrixApprover>(rng, 2, op);
		};

		given("We do evaluate the full result") = [&]() {
			auto op = [](const auto& left, const auto& right) {
				return mpp::matrix{ left / right };
			};

			test_num_op<int, 2, 3, MatrixApprover>(rng, 2, op);
			test_num_op<int, mpp::dynamic, mpp::dynamic, MatrixApprover>(rng, 2, op);
			test_num_op<int, mpp::dynamic, 3, MatrixApprover>(rng, 2, op);
			test_num_op<int, 2, mpp::dynamic, MatrixApprover>(rng, 2, op);
		};
	};

	return 0;
}
