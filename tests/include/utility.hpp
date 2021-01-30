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

#include <string>

namespace matrixpp_test::detail
{
	struct value_type_names
	{
		template<typename T>
		static std::string GetName(int)
		{
			using value_type = typename T::value_type;

			if constexpr (std::is_same<value_type, int>())
			{
				return "Int";
			}
			else if (std::is_same<value_type, unsigned int>())
			{
				return "UnsignedInt";
			}
			else if (std::is_same<value_type, double>())
			{
				return "Double";
			}
			else if (std::is_same<value_type, long>())
			{
				return "Long";
			}
			else if (std::is_same<value_type, long double>())
			{
				return "LongDouble";
			}
			else if (std::is_same<value_type, float>())
			{
				return "Float";
			}
			// @TODO: Clang Format does not format [[likely]] and [[unlikely]] properly
			// clang-format off
			else [[unlikely]]
			{
				return "";
			}
			// clang-format on
		}
	};
} // namespace matrixpp_test::detail