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

namespace mpp::detail
{
	static inline constexpr auto DIMENSIONS_NOT_THE_SAME     = "Matrices are not the same size!";
	static inline constexpr auto DIMENSIONS_NOT_MULTIPLIABLE = "Matrices are not multipliable!";
	static inline constexpr auto DIMENSIONS_NOT_SQUARE       = "Dimensions are not square!";

	static inline constexpr auto IDENTITY_RANK_OF_ZERO = "Identity matrix cannot have a rank of 0!";

	static inline constexpr auto INITIALIZER_UNEQUAL_ROWS = "Initializer does not have equal columns in rows!";
	static inline constexpr auto INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS =
		"Initializer does not have compatible dimensions with matrix extents!";

	static inline constexpr auto BLOCK_TOP_ROW_INDEX_OUT_OF_BOUNDS       = "Top row index out of bounds!";
	static inline constexpr auto BLOCK_TOP_COLUMN_INDEX_OUT_OF_BOUNDS    = "Top column index out of bounds!";
	static inline constexpr auto BLOCK_BOTTOM_ROW_INDEX_OUT_OF_BOUNDS    = "Bottom row index out of bounds!";
	static inline constexpr auto BLOCK_BOTTOM_COLUMN_INDEX_OUT_OF_BOUNDS = "Bottom column index out of bounds!";
	static inline constexpr auto BLOCK_TOP_ROW_INDEX_BIGGER_THAN_BOTTOM_ROW_INDEX =
		"Top row index bigger than bottom row index!";
	static inline constexpr auto BLOCK_TOP_COLUMN_INDEX_BIGGER_THAN_BOTTOM_COLUMN_INDEX =
		"Top column index bigger than bottom column index!";

	static inline constexpr auto BOUNDS_OUT_OF_RANGE = "Bounds are out of range!";
} // namespace mpp::detail