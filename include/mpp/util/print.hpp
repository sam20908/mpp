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

#include <mpp/detail/util/cpo_base.hpp>
#include <mpp/detail/util/print_impl.hpp>
#include <mpp/mat.hpp>

#include <cstddef>
#include <iosfwd>
#include <sstream>

namespace mpp
{
	struct print_t : public detail::cpo_base<print_t>
	{
		template<typename T, typename Buf>
		friend inline auto tag_invoke(print_t, const mat<T, Buf>& obj) -> void
		{
			auto msg = std::stringstream{};
			detail::append_expr_to_stream(msg, obj, "");

			std::cout << msg.str();
		}
	};

	template<typename T, typename Buf>
	auto operator<<(std::ostream& os, const mat<T, Buf>& obj) -> std::ostream&
	{
		detail::append_expr_to_stream(os, obj, "");
		return os;
	}

	inline constexpr auto print = print_t{};
} // namespace mpp
