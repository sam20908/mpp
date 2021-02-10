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

from lit.Test import FAIL, UNSUPPORTED, PASS
from lit.util import executeCommand
from lit.formats import FileBasedTest

# pylint: disable=W0612,R0201
# NOTE: Copied from llvm-project!


class ExecutableTest(FileBasedTest):
    """ Test format that runs executables and check their exit code """

    def execute(self, test, _):
        """ Executes current executable passed by test runner """

        if test.config.unsupported:
            return UNSUPPORTED

        out, err, exit_code = executeCommand(test.getSourcePath())

        if not exit_code:
            return PASS, out

        return FAIL, out+err