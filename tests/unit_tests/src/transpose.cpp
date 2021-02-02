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

#include <gtest/gtest.h>

#include <matrixpp/algorithm/transpose.hpp>
#include <matrixpp/matrix.hpp>

#include <span>

namespace
{
	TEST(Transpose, Test)
	{
		auto from = matrixpp::matrix<int, 3, 2>{ { 1, 2 }, { 3, 4 }, { 5, 6 } };
		auto to   = matrixpp::transpose(from);

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 3);

		EXPECT_EQ(to(0, 0), 1);
		EXPECT_EQ(to(0, 1), 3);
		EXPECT_EQ(to(0, 2), 5);
		EXPECT_EQ(to(1, 0), 2);
		EXPECT_EQ(to(1, 1), 4);
		EXPECT_EQ(to(1, 2), 6);
	}
} // namespace