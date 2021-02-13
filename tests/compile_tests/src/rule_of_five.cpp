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

// RUN: %build_and_run

#include <mpp/matrix.hpp>

#include <span>
#include <type_traits>

template<typename Type>
constexpr auto has_rule_of_five_v = std::is_default_constructible_v<Type>&& std::is_copy_constructible_v<Type>&&
	std::is_copy_assignable_v<Type>&& std::is_move_constructible_v<Type>&& std::is_move_assignable_v<Type>;

int main()
{
	static_assert(has_rule_of_five_v<mpp::matrix<int, 1, 1>>, "Fully static matrices should follow rule of five!");

	static_assert(has_rule_of_five_v<mpp::matrix<int>>, "Fully dynamic matrices should follow rule of five!");

	static_assert(has_rule_of_five_v<mpp::matrix<int, std::dynamic_extent, 1>>,
		"Dynamic rows matrices should follow rule of five!");

	static_assert(has_rule_of_five_v<mpp::matrix<int, 1, std::dynamic_extent>>,
		"Dynamic columns matrices should follow rule of five!");

	return 0;
}