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

# pylint: skip-file


from lit.formats import ShTest
from os.path import dirname


has_binary_dir = hasattr(config, 'binary_dir')
has_source_dir = hasattr(config, 'source_dir')
if not has_binary_dir or not has_source_dir:
    # The user probably tried to run lit in this directory, which means we can't
    # access some of the properties defined by the lit.site.cfg.py file in the
    # binary dir
    lit_config.fatal(
        'Lit must be ran in <build directory>/bin/tests/compile_tests!')

config.name = 'Compile Test'
config.suffixes = ['.cpp']
config.test_format = ShTest()
config.test_source_root = dirname(__file__)
config.test_exec_root = config.binary_dir

config.excludes = ['__pycache__', 'build']
config.substitutions.append(('%binary_dir', config.binary_dir))
