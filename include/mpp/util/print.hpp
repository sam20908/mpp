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
#include <mpp/mat/matfwd.hpp>

#include <cstddef>
#include <iosfwd>
#include <sstream>

namespace mpp
{
	struct print_t : public detail::cpo_base<print_t>
	{
		template<typename Val, std::size_t Rows, std::size_t Cols, typename Alloc>
		friend inline auto tag_invoke(print_t, const mat<Val, Rows, Cols, Alloc>& obj) -> void
		{
			auto message_stream = std::stringstream{};
			detail::append_expr_to_stream(message_stream, obj, "");

			std::cout << message_stream.str();
		}
	};

	template<typename Val, std::size_t Rows, std::size_t Cols, typename Alloc>
	auto operator<<(std::ostream& os, const mat<Val, Rows, Cols, Alloc>& obj) -> std::ostream&
	{
		detail::append_expr_to_stream(os, obj, "");
		return os;
	}

	inline constexpr auto print = print_t{};
} // namespace mpp
