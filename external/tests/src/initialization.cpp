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

#include <boost/ut.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <mpp/matrix.hpp>

#include "../../include/custom_allocator.hpp"
#include "../../include/test_parsers.hpp"
#include "../../include/utility.hpp"

#include <array>
#include <cstddef>
#include <initializer_list>
#include <string>
#include <vector>

static auto parse_vec2d_out(const std::string& file)
{
	const auto result = parse_mats_out<temp_types<vec2d_t>, types<int>>(get_filepath(file), std::tuple{ vec2d_fn });
	const auto out    = std::get<0>(result);

	return out;
}

int main()
{
	// @NOTE: Construction from expression object will be covered in lazy/eager arithmetic tests
	// @NOTE: This covers copy and move construction from rule of five (2/5)

	using namespace boost::ut::bdd;

	const auto empty_rng = parse_vec2d_out("initialization/default.txt");
	const auto rng       = parse_vec2d_out("initialization/2x3_rng.txt");

	feature("Default initialization") = [&]() {
		when("We don't specify a custom allocator as a type") = [&]() {
			const auto tup = create_mats<int, 0, 0, all_mats_t>(args(fwd_args));

			for_each_in_tup(tup, [&](const auto& mat) {
				cmp_mat_to_rng(mat, empty_rng);
			});
		};

		when("We do specify a custom allocator as a type") = [&]() {
			const auto tup = create_mats<int, 0, 0, all_mats_t, custom_allocator<int>>(args(fwd_args));

			for_each_in_tup(tup, [&](const auto& mat) {
				cmp_mat_to_rng(mat, empty_rng);
			});
		};
	};

	feature("Initialize with custom allocator (dynamic matrices only)") = [&]() {
		const auto allocator = custom_allocator<int>{};
		const auto tup       = create_mats<int, 0, 0, dyn_mats_t, custom_allocator<int>>(args(fwd_args, allocator));

		for_each_in_tup(tup, [&](const auto& mat) {
			cmp_mat_to_rng(mat, empty_rng);
		});
	};

	feature("2D range initialization") = [&]() {
		given("We're using a 2D initializer list") = [&]() {
			const auto init_rng = std::initializer_list<std::initializer_list<float>>{ { 1, 2, 3 }, { 4, 5, 6 } };

			when("We don't use unsafe") = [&]() {
				const auto tup = create_mats<int, 2, 3, all_mats_t>(args(fwd_args, init_rng));

				for_each_in_tup(tup, [&](const auto& mat) {
					cmp_mat_to_rng(mat, rng);
				});
			};

			when("We do use unsafe") = [&]() {
				const auto tup = create_mats<int, 2, 3, all_mats_t>(args(fwd_args, init_rng, mpp::unsafe));

				for_each_in_tup(tup, [&](const auto& mat) {
					cmp_mat_to_rng(mat, rng);
				});
			};
		};

		given("We're using a 2D vector") = [&]() {
			// Use float to test compilation
			const auto init_rng = std::vector<std::vector<float>>{ { 1, 2, 3 }, { 4, 5, 6 } };

			when("We don't use unsafe") = [&]() {
				const auto tup = create_mats<int, 2, 3, all_mats_t>(args(fwd_args, init_rng));

				for_each_in_tup(tup, [&](const auto& mat) {
					cmp_mat_to_rng(mat, rng);
				});
			};

			when("We do use unsafe") = [&]() {
				const auto tup = create_mats<int, 2, 3, all_mats_t>(args(fwd_args, init_rng, mpp::unsafe));

				for_each_in_tup(tup, [&](const auto& mat) {
					cmp_mat_to_rng(mat, rng);
				});
			};
		};

		given("We're using a 2D array for fully static matrices") = [&]() {
			auto init_rng = std::array<std::array<float, 3>, 2>{ { { 1, 2, 3 }, { 4, 5, 6 } } };

			then("We copy initialize it") = [&]() {
				const auto mat = mpp::matrix<int, 2, 3>{ init_rng };

				cmp_mat_to_rng(mat, rng);
			};

			then("We move initialize it") = [&]() {
				const auto mat = mpp::matrix<int, 2, 3>{ std::move(init_rng) };

				cmp_mat_to_rng(mat, rng);
			};
		};
	};

	feature("1D range initialization") = [&]() {
		const auto init_rng = std::vector<int>{ 1, 2, 3, 4, 5, 6 };

		when("We don't use unsafe") = [&]() {
			const auto tup =
				create_mats<int, 2, 3, all_mats_t>(args(fwd_args, std::size_t{ 2 }, std::size_t{ 3 }, init_rng));

			for_each_in_tup(tup, [&](const auto& mat) {
				cmp_mat_to_rng(mat, rng);
			});
		};

		when("We do use unsafe") = [&]() {
			const auto tup = create_mats<int, 2, 3, all_mats_t>(
				args(fwd_args, std::size_t{ 2 }, std::size_t{ 3 }, init_rng, mpp::unsafe));

			for_each_in_tup(tup, [&](const auto& mat) {
				cmp_mat_to_rng(mat, rng);
			});
		};
	};

	feature("Callable for initialization") = []() {
		auto iota_fn = []() {
			return [i = 1]() mutable {
				return i++;
			};
		};

		const auto out = parse_vec2d_out("initialization/2x3_iota.txt");
		const auto tup =
			create_mats<int, 2, 3, all_mats_t>(args(overloaded{ [&](types<mpp::matrix<int, 2, 3>>) {
																   return mpp::matrix<int, 2, 3>{ iota_fn() };
															   },
				[&](types<mpp::matrix<int, mpp::dynamic, mpp::dynamic>>) {
					return mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ 2, 3, iota_fn() };
				},
				[&](types<mpp::matrix<int, mpp::dynamic, 3>>) {
					return mpp::matrix<int, mpp::dynamic, 3>{ 2, iota_fn() };
				},
				[&](types<mpp::matrix<int, 2, mpp::dynamic>>) {
					return mpp::matrix<int, 2, mpp::dynamic>{ 3, iota_fn() };
				} }));

		for_each_in_tup(tup, [&](const auto& mat) {
			cmp_mat_to_rng(mat, out);
		});
	};

	feature("Value initialization") = []() {
		const auto out = parse_vec2d_out("initialization/2x3_val_init.txt");
		const auto tup = create_mats<int, 2, 3, all_mats_t>(args(overloaded{ [&](types<mpp::matrix<int, 2, 3>>) {
																				return mpp::matrix<int, 2, 3>{ 1 };
																			},
			[&](types<mpp::matrix<int, mpp::dynamic, mpp::dynamic>>) {
				return mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ 2, 3, 1 };
			},
			[&](types<mpp::matrix<int, mpp::dynamic, 3>>) {
				return mpp::matrix<int, mpp::dynamic, 3>{ 2, 1 };
			},
			[&](types<mpp::matrix<int, 2, mpp::dynamic>>) {
				return mpp::matrix<int, 2, mpp::dynamic>{ 3, 1 };
			} }));

		for_each_in_tup(tup, [&](const auto& mat) {
			cmp_mat_to_rng(mat, out);
		});
	};

	feature("Constructing identity matrices") = []() {
		const auto out = parse_vec2d_out("initialization/3x3_identity.txt");

		when("We don't use unsafe") = [&]() {
			const auto tup =
				create_mats<int, 3, 3, all_mats_t>(args(overloaded{ [&](types<mpp::matrix<int, 3, 3>>) {
																	   return mpp::matrix<int, 3, 3>{ mpp::identity };
																   },
					[&](types<mpp::matrix<int, mpp::dynamic, mpp::dynamic>>) {
						return mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ 3, 3, mpp::identity };
					},
					[&](types<mpp::matrix<int, mpp::dynamic, 3>>) {
						return mpp::matrix<int, mpp::dynamic, 3>{ 3, mpp::identity };
					},
					[&](types<mpp::matrix<int, 3, mpp::dynamic>>) {
						return mpp::matrix<int, 3, mpp::dynamic>{ 3, mpp::identity };
					} }));

			for_each_in_tup(tup, [&](const auto& mat) {
				cmp_mat_to_rng(mat, out);
			});
		};

		when("We do use unsafe") = [&]() {
			const auto tup = create_mats<int, 3, 3, all_mats_t>(
				args(overloaded{ [&](types<mpp::matrix<int, 3, 3>>) {
									return mpp::matrix<int, 3, 3>{ mpp::identity, mpp::unsafe };
								},
					[&](types<mpp::matrix<int, mpp::dynamic, mpp::dynamic>>) {
						return mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ 3, 3, mpp::identity, mpp::unsafe };
					},
					[&](types<mpp::matrix<int, mpp::dynamic, 3>>) {
						return mpp::matrix<int, mpp::dynamic, 3>{ 3, mpp::identity, mpp::unsafe };
					},
					[&](types<mpp::matrix<int, 3, mpp::dynamic>>) {
						return mpp::matrix<int, 3, mpp::dynamic>{ 3, mpp::identity, mpp::unsafe };
					} }));

			for_each_in_tup(tup, [&](const auto& mat) {
				cmp_mat_to_rng(mat, out);
			});
		};
	};

	feature("Copy initialization") = [&]() {
		when("Without custom allocator") = [&]() {
			const auto tup      = create_mats<int, 2, 3, all_mats_t>(args(fwd_args, rng));
			const auto tup_copy = create_mats<int, 2, 3, all_mats_t>(
				args(overloaded{ [&](types<mpp::matrix<int, 2, 3>>) {
									return mpp::matrix<int, 2, 3>{ std::get<0>(tup) };
								},
					[&](types<mpp::matrix<int, mpp::dynamic, mpp::dynamic>>) {
						return mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ std::get<1>(tup) };
					},
					[&](types<mpp::matrix<int, mpp::dynamic, 3>>) {
						return mpp::matrix<int, mpp::dynamic, 3>{ std::get<2>(tup) };
					},
					[&](types<mpp::matrix<int, 2, mpp::dynamic>>) {
						return mpp::matrix<int, 2, mpp::dynamic>{ std::get<3>(tup) };
					} }));

			for_each_in_tup(tup_copy, [&](const auto& mat) {
				cmp_mat_to_rng(mat, rng);
			});
		};

		when("With custom allocators (dyamic matrices only)") = [&]() {
			const auto allocator = custom_allocator<int>{};
			const auto tup       = create_mats<int, 2, 3, dyn_mats_t, custom_allocator<int>>(args(fwd_args, rng));
			const auto tup_copy  = create_mats<int, 2, 3, dyn_mats_t, custom_allocator<int>>(args(overloaded{
                [&](types<mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>>) {
                    return mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{ std::get<0>(tup),
                        allocator };
                },
                [&](types<mpp::matrix<int, mpp::dynamic, 3, custom_allocator<int>>>) {
                    return mpp::matrix<int, mpp::dynamic, 3, custom_allocator<int>>{ std::get<1>(tup), allocator };
                },
                [&](types<mpp::matrix<int, 2, mpp::dynamic, custom_allocator<int>>>) {
                    return mpp::matrix<int, 2, mpp::dynamic, custom_allocator<int>>{ std::get<2>(tup), allocator };
                } }));

			for_each_in_tup(tup_copy, [&](const auto& mat) {
				cmp_mat_to_rng(mat, rng);
			});
		};
	};

	feature("Move initialization") = [&]() {
		when("Without custom allocator") = [&]() {
			auto tup            = create_mats<int, 2, 3, all_mats_t>(args(fwd_args, rng));
			const auto tup_copy = create_mats<int, 2, 3, all_mats_t>(
				args(overloaded{ [&](types<mpp::matrix<int, 2, 3>>) {
									return mpp::matrix<int, 2, 3>{ std::move(std::get<0>(tup)) };
								},
					[&](types<mpp::matrix<int, mpp::dynamic, mpp::dynamic>>) {
						return mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ std::move(std::get<1>(tup)) };
					},
					[&](types<mpp::matrix<int, mpp::dynamic, 3>>) {
						return mpp::matrix<int, mpp::dynamic, 3>{ std::move(std::get<2>(tup)) };
					},
					[&](types<mpp::matrix<int, 2, mpp::dynamic>>) {
						return mpp::matrix<int, 2, mpp::dynamic>{ std::move(std::get<3>(tup)) };
					} }));

			for_each_in_tup(tup_copy, [&](const auto& mat) {
				cmp_mat_to_rng(mat, rng);
			});
		};

		when("With custom allocators (dyamic matrices only)") = [&]() {
			const auto allocator = custom_allocator<int>{};
			auto tup             = create_mats<int, 2, 3, dyn_mats_t, custom_allocator<int>>(args(fwd_args, rng));
			const auto tup_copy  = create_mats<int, 2, 3, dyn_mats_t, custom_allocator<int>>(
                args(overloaded{ [&](types<mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>>) {
                                    return mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{
                                        std::move(std::get<0>(tup)),
                                        allocator
                                    };
                                },
                    [&](types<mpp::matrix<int, mpp::dynamic, 3, custom_allocator<int>>>) {
                        return mpp::matrix<int, mpp::dynamic, 3, custom_allocator<int>>{ std::move(std::get<1>(tup)),
                            allocator };
                    },
                    [&](types<mpp::matrix<int, 2, mpp::dynamic, custom_allocator<int>>>) {
                        return mpp::matrix<int, 2, mpp::dynamic, custom_allocator<int>>{ std::move(std::get<2>(tup)),
                            allocator };
                    } }));

			for_each_in_tup(tup_copy, [&](const auto& mat) {
				cmp_mat_to_rng(mat, rng);
			});
		};
	};

	feature("Different matrix type initialization") = [&]() {
		when("Without custom allocator") = [&]() {
			const auto tup      = create_mats<int, 2, 3, all_mats_t>(args(fwd_args, rng));
			const auto tup_copy = create_mats<int, 2, 3, all_mats_t>(
				args(overloaded{ [&](types<mpp::matrix<int, 2, 3>>) {
									return mpp::matrix<int, 2, 3>{ std::get<3>(tup) };
								},
					[&](types<mpp::matrix<int, mpp::dynamic, mpp::dynamic>>) {
						return mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ std::get<2>(tup) };
					},
					[&](types<mpp::matrix<int, mpp::dynamic, 3>>) {
						return mpp::matrix<int, mpp::dynamic, 3>{ std::get<1>(tup) };
					},
					[&](types<mpp::matrix<int, 2, mpp::dynamic>>) {
						return mpp::matrix<int, 2, mpp::dynamic>{ std::get<0>(tup) };
					} }));

			for_each_in_tup(tup_copy, [&](const auto& mat) {
				cmp_mat_to_rng(mat, rng);
			});
		};

		when("With custom allocators (dyamic matrices only)") = [&]() {
			const auto allocator = custom_allocator<int>{};
			const auto tup       = create_mats<int, 2, 3, dyn_mats_t, custom_allocator<int>>(args(fwd_args, rng));
			const auto tup_copy  = create_mats<int, 2, 3, dyn_mats_t, custom_allocator<int>>(args(overloaded{
                [&](types<mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>>) {
                    return mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{ std::get<2>(tup),
                        allocator };
                },
                [&](types<mpp::matrix<int, mpp::dynamic, 3, custom_allocator<int>>>) {
                    return mpp::matrix<int, mpp::dynamic, 3, custom_allocator<int>>{ std::get<0>(tup), allocator };
                },
                [&](types<mpp::matrix<int, 2, mpp::dynamic, custom_allocator<int>>>) {
                    return mpp::matrix<int, 2, mpp::dynamic, custom_allocator<int>>{ std::get<1>(tup), allocator };
                } }));

			for_each_in_tup(tup_copy, [&](const auto& mat) {
				cmp_mat_to_rng(mat, rng);
			});
		};
	};

	return 0;
}
