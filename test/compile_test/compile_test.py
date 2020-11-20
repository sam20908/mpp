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
import tempfile


class CompileTest(lit.formats.TestFormat):
    def __init__(self, test_dir):
        self.test_dir = test_dir

    def getTestsInDirectory(self, test_suite, path_in_suite,
                            lit_config, local_config):
        self.expected_results = {}
        self.test_directory = local_config.test_directory

        expected_results_txt_path = os.path.join(
            self.test_dir, 'expected_results.txt')

        # Parse the file as a map of file name and their expected result
        with open(expected_results_txt_path, 'r') as expected_results_reader:
            current_line = expected_results_reader.readline()

            filename, expected_result = current_line.split(' ')
            self.expected_results[filename] = expected_result

        for entry in os.scandir(self.test_dir):
            if not entry.is_dir():
                continue

            test_source_path = os.path.join(entry.path, 'test.cpp')

            if not os.path.isfile(test_source_path):
                continue

            # lit.Test.Test expects the path for the suite to be a tuple
            # because it joins it later with a separator for all path getters
            test_source_path_tuple = path_in_suite + (entry.path, 'test.cpp')

            test = lit.Test.Test(
                test_suite, test_source_path_tuple, local_config, test_source_path)

            yield test

    def execute(self, test, lit_config):
        build_dir = self.test_directory

        test_source_path_separated = test.file_path.split(os.path.sep)

        # We only need the last two elements, which is the test directory name
        # and the test source name
        test_directory_name, test_source_name = test_source_path_separated[-2:]
        test_source_relative_path = test_directory_name + '/' + test_source_name

        if not test_source_relative_path in self.expected_results.keys():
            return lit.Test.UNSUPPORTED, "Test isn't specified in expected_results.txt"

        # We need a temporary location to store the CMake configuration for
        # the test sources
        with tempfile.TemporaryDirectory() as temp_dir:
            # Switch the current working directory because we want the CMake
            # configuration files to be cleaned up after the test (the output
            # and error messages are returned by lit.util.executeCommand)
            os.chdir(temp_dir)

            # Use the configured CMakeLists in the test directory in the build folder
            # to configure for specific test source (we do this by the command line)
            cmake_test_name_arg = f'-DTEST_NAME={test_directory_name}'
            cmake_test_source_arg = f'-DTEST_SOURCE={test.file_path}'
            cmake_config_cmd = [
                'cmake', cmake_test_name_arg, cmake_test_source_arg, '-DMATRIXPP_BUILD_TEST=FALSE',
                build_dir, '-B', 'build']
            config_out, config_err, config_exit_code = lit.util.executeCommand(
                cmake_config_cmd)

            if config_exit_code:
                # We don't check for expected results here because this has nothing
                # to do with the test source
                config_diagnostic = config_out + config_err
                return lit.Test.FAIL, config_diagnostic

            cmake_build_cmd = ['cmake', '--build', 'build']
            build_out, build_err, build_exit_code = lit.util.executeCommand(
                cmake_build_cmd)

            if build_exit_code:
                build_diagnostic = build_out + build_err

                # Check if the file was intended to FAIL. Return XFAIL if thats the case
                if self.expected_results[test_source_relative_path] == 'FAIL':
                    return lit.Test.XFAIL, build_diagnostic

                return lit.Test.FAIL, build_diagnostic
            else:
                # Check if the file was intended to PASS
                if self.expected_results[test_source_relative_path] == 'PASS':
                    return lit.Test.PASS, ''

                return lit.Test.XPASS, ''
