"""
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
"""

import lit.Test
import lit.formats
import lit.util
import os
import pathlib
import tempfile


def _parse_expected_results(file_path):
    expected_results = {}

    # Parse the file as a map of file name and their expected result
    with open(file_path, 'r') as reader:
        lines = reader.readlines()
        for line in lines:
            test_source_relpath, expected_result = line.strip().split(' ')
            expected_results[test_source_relpath] = expected_result

    return expected_results


def _parse_tests_to_run(lit_config, test_dir, test_suite, path_in_suite, local_config):
    tests = []

    def _add_test_sources_to_tests(test_glob_path):
        test_suffix = local_config.test_suffix

        # @TODO: Support exclusion of tests
        for test_source in pathlib.Path(test_glob_path).rglob(test_suffix):
            test_source_path_tuple = path_in_suite + \
                (str(test_source.resolve()),)

            # Convert path to unix style in all cases
            test_source_relpath = os.path.relpath(
                test_source, test_dir).replace(os.path.sep, '/')

            test = lit.Test.Test(
                test_suite, test_source_path_tuple, local_config, test_source_relpath)

            tests.append(test)

    if 'COMPILE_TESTS_TO_RUN' in lit_config.params.keys():
        specified_tests = lit_config.params['COMPILE_TESTS_TO_RUN'].split(',')

        for specified_test in specified_tests:
            specified_test_dir = os.path.join(test_dir, specified_test)
            specified_test_exists = os.path.isdir(specified_test_dir)

            if not specified_test_exists:
                continue

            _add_test_sources_to_tests(specified_test_dir)

    else:
        _add_test_sources_to_tests(test_dir)

    return tests


class CompileTest(lit.formats.TestFormat):
    def getTestsInDirectory(self, test_suite, path_in_suite,
                            lit_config, local_config):
        expected_results_filepath = os.path.join(
            local_config.test_dir, 'expected_results.txt')

        self.test_dir = local_config.test_dir
        self.build_dir = local_config.build_dir
        self.expected_results = _parse_expected_results(
            expected_results_filepath)

        tests = _parse_tests_to_run(
            lit_config, self.test_dir, test_suite, path_in_suite, local_config)

        for test in tests:
            yield test

    def execute(self, test, lit_config):
        test_source_relpath = test.file_path
        test_source_dirname = os.path.dirname(
            test_source_relpath).replace(os.path.sep, '/')

        if not test_source_relpath in self.expected_results.keys():
            return lit.Test.UNSUPPORTED, "Test isn't specified in expected_results.txt"

        # Use a temporary directory to avoid leaving garbage CMake configuration
        # for test sources behind
        with tempfile.TemporaryDirectory() as temp_dir:
            # Switch the current working directory because we want the CMake
            # configuration files to be in the temporary directory to be cleaned
            os.chdir(temp_dir)

            # Use the configured CMakeLists in the test directory in the build folder
            # to configure for specific test source (we do this by the command line)
            # CMake doesn't allow / in target names, so we convert it to underscore
            test_source_dirname_cmake_friendly = test_source_dirname.replace(
                '/', '_')
            cmake_test_name_arg = f'-DTEST_NAME={test_source_dirname_cmake_friendly}'
            cmake_test_source_arg = f'-DTEST_SOURCE={test.getSourcePath()}'
            cmake_config_cmd = [
                'cmake', cmake_test_name_arg, cmake_test_source_arg, self.build_dir,
                '-B', 'build']
            config_out, config_err, config_exit_code = lit.util.executeCommand(
                cmake_config_cmd)

            if config_exit_code:
                # Test couldn't start because didn't get past configuration stage
                config_diagnostic = config_out + config_err
                return lit.Test.UNRESOLVED, config_diagnostic

            cmake_build_cmd = ['cmake', '--build', 'build']
            build_out, build_err, build_exit_code = lit.util.executeCommand(
                cmake_build_cmd)

            if build_exit_code:
                build_diagnostic = build_out + build_err

                # Check if the file was intended to FAIL. Return XFAIL if thats the case
                if self.expected_results[test_source_relpath] == 'FAIL':
                    return lit.Test.XFAIL, build_diagnostic

                return lit.Test.FAIL, build_diagnostic
            else:
                # Check if the file was intended to PASS
                if self.expected_results[test_source_relpath] == 'PASS':
                    return lit.Test.PASS, ''

                return lit.Test.XPASS, ''
