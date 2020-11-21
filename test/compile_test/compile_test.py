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


class CompileTest(lit.formats.TestFormat):
    def getTestsInDirectory(self, test_suite, path_in_suite,
                            lit_config, local_config):
        self.expected_results = {}
        self.test_dir = local_config.test_dir
        self.build_dir = local_config.build_dir

        expected_results_txt_path = os.path.join(
            self.test_dir, 'expected_results.txt')

        # Parse the file as a map of file name and their expected result
        with open(expected_results_txt_path, 'r') as expected_results_reader:
            lines = expected_results_reader.readlines()
            for line in lines:
                test_source_relpath, expected_result = line.strip().split(' ')
                self.expected_results[test_source_relpath] = expected_result

        # Only run the specified tests and avoid recursive search of tests
        if 'COMPILE_TESTS_TO_RUN' in lit_config.params.keys():
            tests = lit_config.params['COMPILE_TESTS_TO_RUN'].split(',')

            for test in tests:
                # Check if the specified test really exists
                test_dir_path = os.path.join(self.test_dir, test)
                test_dir_exists = os.path.isdir(test_dir_path)

                if not test_dir_exists:
                    continue

                test_source_abspath = os.path.join(test_dir_path, 'test.cpp')
                test_source_exists = os.path.isfile(test_source_abspath)

                if not test_source_exists:
                    continue

                # lit.Test.Test expects the path for the suite to be a tuple
                # because it joins it later with a separator for all path getters
                test_source_path_tuple = path_in_suite + (test_source_abspath,)
                test_source_relpath = os.path.join(test, 'test.cpp')
                test = lit.Test.Test(
                    test_suite, test_source_path_tuple, local_config, test_source_relpath)

                yield test
        else:
            for test_source in pathlib.Path(self.test_dir).rglob('test.cpp'):
                test_source_relpath = os.path.relpath(
                    test_source, self.test_dir)

                # lit.Test.Test expects the path for the suite to be a tuple
                # because it joins it later with a separator for all path getters
                test_source_path_tuple = path_in_suite + (str(test_source),)
                test = lit.Test.Test(
                    test_suite, test_source_path_tuple, local_config, test_source_relpath)

                yield test

    def execute(self, test, lit_config):
        # @TODO: Convert type to unix style
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
