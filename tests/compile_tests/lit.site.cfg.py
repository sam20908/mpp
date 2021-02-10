"""
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
"""

# pylint: disable=line-too-long


from os import getenv, pathsep
from os.path import dirname, exists
from pathlib import Path
from lit.formats import ShTest
from lit import LitConfig, TestingConfig


lit_config: LitConfig
config: TestingConfig


def get_multi_command_separator():
    """ Returns the separator used to execute multiple commands for the current terminal """
    if lit_config.isWindows:
        is_powershell = len(getenv('PSModulePath', '').split(pathsep)) >= 3

        # With powershell, we can use ;
        # With others, we can use &&
        if is_powershell:
            return ';'

        return '&&'

    # Both MacOS and Linux uses &&
    return '&&'


def get_compilers_from_cache():
    """ Gets cached compilers by CMake """
    does_cache_exists = False
    cache_c_compiler_ = ''
    cache_cxx_compiler_ = ''

    if exists('build'):
        does_cache_exists = True

        # CMakeCache.txt can have multiple lines stating
        c_compiler_found = False
        cxx_compiler_found = False

        # Since the compiler is propagated through all compile tests, it is safe to just find
        # CMakeCache.txt from one of the compile tests
        for cache in Path('build').rglob('*.txt'):
            with open(cache, 'r') as cache_reader:
                for line in cache_reader:
                    line = line.rstrip()  # Account for different line endings

                    if not c_compiler_found and 'CMAKE_C_COMPILER:STRING' in line:
                        cache_c_compiler_ = line.split('=')[1]
                        c_compiler_found = True

                    elif not cxx_compiler_found and 'CMAKE_CXX_COMPILER:STRING' in line:
                        cache_cxx_compiler_ = line.split('=')[1]
                        cxx_compiler_found = True

            if c_compiler_found and cxx_compiler_found:
                break  # We don't need to find other CMakeCache.txt anymore

    return (does_cache_exists, cache_c_compiler_, cache_cxx_compiler_)


has_binary_dir = hasattr(config, 'binary_dir')
has_source_dir = hasattr(config, 'source_dir')
has_c_compiler = hasattr(config, 'c_compiler')
has_cxx_compiler = hasattr(config, 'cxx_compiler')
if not has_binary_dir or not has_source_dir or not has_c_compiler or not has_cxx_compiler:
    # The user probably tried to run lit in this directory, which means we can't
    # access some of the properties defined by the lit.site.cfg.py file in the
    # binary dir
    lit_config.fatal(
        'Lit must be ran in <build directory>/bin/tests/compile_tests!')

c_compiler = config.c_compiler
cxx_compiler = config.cxx_compiler
lit_config.note('C compiler is ' + c_compiler +
                ' (propagated from mpp CMakeLists)')
lit_config.note('CXX compiler is ' + cxx_compiler +
                ' (propagated from mpp CMakeLists)')

(cache_exists, cache_c_compiler, cache_cxx_compiler) = get_compilers_from_cache()

if cache_exists:
    lit_config.note('Existing cache has C compiler: ' + cache_c_compiler)
    lit_config.note('Existing cache has CXX compiler: ' + cache_cxx_compiler)

    if cache_c_compiler != c_compiler or cache_cxx_compiler != cxx_compiler:
        # There is a conflict between the existing cache and the configuration. Error
        # the user about it
        lit_config.error(
            'There will be conflicting compilers when configuring for compile tests! Remove the build folder to avoid conflict')
    else:
        lit_config.note(
            'No conflicts between compilers from existing cache and propagated compilers from mpp CMakeLists')

config.name = 'Compile Test'
config.suffixes = ['.cpp']
config.test_format = ShTest()
config.test_source_root = dirname(__file__)
config.test_exec_root = config.binary_dir

CD_BINARY_CMD = 'cd {binary_dir}'.format(binary_dir=config.binary_dir)
CMAKE_CONFIGURE_CMD = 'cmake -DTEST_NAME=%basename_t -DTEST_SOURCE=%s -B build/%basename_t -DCMAKE_C_COMPILER={c_compiler} -DCMAKE_CXX_COMPILER={cxx_compiler}'.format(
    c_compiler=c_compiler, cxx_compiler=cxx_compiler)
CMAKE_BUILD_CMD = 'cmake --build build/%basename_t --target %basename_t'

config.substitutions.append(('%build_and_run', '{cd_binary} {separator} {cmake_configure} {separator} {cmake_build}'.format(
    cd_binary=CD_BINARY_CMD, separator=get_multi_command_separator(), cmake_configure=CMAKE_CONFIGURE_CMD, cmake_build=CMAKE_BUILD_CMD)))
