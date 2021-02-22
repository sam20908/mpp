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

#include <iterator>

int main()
{
	// No need to use partially dynamic matrices since they still have dynamic buffer iterators

	using static_matrix  = mpp::matrix<int, 1, 1>;
	using dynamic_matrix = mpp::matrix<int>;
	using value_type     = int;

	using static_iterator        = typename static_matrix::iterator;
	using static_const_iterator  = typename static_matrix::const_iterator;
	using dynamic_iterator       = typename dynamic_matrix::iterator;
	using dynamic_const_iterator = typename dynamic_matrix::const_iterator;

	/**
	 * LegacyIterator (input iterators and output iterators satisfy it)
	 */

	static_assert(std::input_iterator<static_iterator>, "Mutable static iterators must meet LegacyInputIterator!");
	static_assert(std::input_iterator<static_const_iterator>,
		"Immutable static iterators must meet LegacyInputIterator!");
	static_assert(std::input_iterator<dynamic_iterator>, "Mutable dynamic iterators must meet LegacyInputIterator!");
	static_assert(std::input_iterator<dynamic_const_iterator>,
		"Immutable dynamic iterators must meet LegacyInputIterator!");

	static_assert(std::output_iterator<static_iterator, value_type>,
		"Mutable static iterators must meet LegacyOutputIterator!");
	static_assert(std::output_iterator<dynamic_iterator, value_type>,
		"Mutable dynamic iterators must meet LegacyOutputIterator!");

	/**
	 * LegacyForwardIterator
	 */

	static_assert(std::forward_iterator<static_iterator>, "Mutable static iterators must meet LegacyForwardIterator!");
	static_assert(std::forward_iterator<static_const_iterator>,
		"Immutable static iterators must meet LegacyForwardIterator!");
	static_assert(std::forward_iterator<dynamic_iterator>,
		"Mutable dynamic iterators must meet LegacyForwardIterator!");
	static_assert(std::forward_iterator<dynamic_const_iterator>,
		"Immutable dynamic iterators must meet LegacyForwardIterator!");

	/**
	 * LegacyBidirectionalIterator
	 */

	static_assert(std::bidirectional_iterator<static_iterator>,
		"Mutable static iterators must meet LegacyBidirectionalIterator!");
	static_assert(std::bidirectional_iterator<static_const_iterator>,
		"Immutable static iterators must meet LegacyBidirectionalIterator!");
	static_assert(std::bidirectional_iterator<dynamic_iterator>,
		"Mutable dynamic iterators must meet LegacyBidirectionalIterator!");
	static_assert(std::bidirectional_iterator<dynamic_const_iterator>,
		"Immutable dynamic iterators must meet LegacyBidirectionalIterator!");

	/**
	 * LegacyRandomAccessIterator
	 */

	static_assert(std::random_access_iterator<static_iterator>,
		"Mutable static iterators must meet LegacyRandomAccessIterator!");
	static_assert(std::random_access_iterator<static_const_iterator>,
		"Immutable static iterators must meet LegacyRandomAccessIterator!");
	static_assert(std::random_access_iterator<dynamic_iterator>,
		"Mutable dynamic iterators must meet LegacyRandomAccessIterator!");
	static_assert(std::random_access_iterator<dynamic_const_iterator>,
		"Immutable dynamic iterators must meet LegacyRandomAccessIterator!");

	/**
	 * LegacyContiguousIterator
	 */

	static_assert(std::contiguous_iterator<static_iterator>,
		"Mutable static iterators must meet LegacyContiguousIterator!");
	static_assert(std::contiguous_iterator<static_const_iterator>,
		"Immutable static iterators must meet LegacyContiguousIterator!");
	static_assert(std::contiguous_iterator<dynamic_iterator>,
		"Mutable dynamic iterators must meet LegacyContiguousIterator!");
	static_assert(std::contiguous_iterator<dynamic_const_iterator>,
		"Immutable dynamic iterators must meet LegacyContiguousIterator!");

	return 0;
}