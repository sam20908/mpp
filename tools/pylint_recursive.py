#! /usr/bin/env python

'''
Module that runs pylint on all python scripts found in a directory tree..

Originally from https://gist.github.com/MJJoyce/5835693. Edited with the following changes:
- Adapted to mpp project's pylintrc
- Only scan for .py files in whitelisted directories
- Display which files failed to meet required score and exit with non-zero code
'''

import os
import re
import sys
import pathlib

TOTAL = 0.0
COUNT = 0

WHITELISTED_DIRS = ["benchmarks", "tests"]
FAILED_MODULES = []


def check(cfg_dir, module):
    '''
    apply pylint to the file specified if it is a *.py or *.py.in file
    '''
    global TOTAL, COUNT, FAILED_MODULES  # pylint: disable=W0603

    if module.endswith(".py"):

        print("CHECKING " + module)
        pout = os.popen("pylint %s %s" % (cfg_dir, module), "r")
        for line in pout:
            # NOTE (bugfix): Don't use match because that only matches the beginning of the string
            if re.search(r"E....:", line):
                print(line)
                FAILED_MODULES.append(module)

            if "Your code has been rated at" in line:
                print(line)

                # Since we disabled persistent comparison, grab the current score
                score = re.findall(r"\d+.\d\d", line)[0]
                TOTAL += float(score)
                COUNT += 1


if __name__ == "__main__":
    try:
        BASE_DIRECTORY = sys.argv[1]
        print("Setting base directory to" + BASE_DIRECTORY)
    except IndexError:
        BASE_DIRECTORY = os.getcwd()
        print("No base directory passed. Defaulting to current directory...")

    try:
        CONFIG_DIRECTORY = "--rcfile=" + sys.argv[2]
        print("Using config file at" + sys.argv[2])
    except IndexError:
        CONFIG_DIRECTORY = ""
        print("No config file passed. Defaulting to PEP8...")

    print("Looking for *.py files in subdirectories of " + BASE_DIRECTORY)

    for whitelisted_dir in WHITELISTED_DIRS:
        for file in pathlib.Path(whitelisted_dir).rglob("*.py"):
            check(CONFIG_DIRECTORY, str(file))

    print("==" * 50)
    print("%d modules found" % COUNT)
    print("AVERAGE SCORE = %.02f" % (TOTAL / COUNT))

    if len(FAILED_MODULES) > 0:
        print("==" * 50)
        print("The following modules need to be fixed:\n")
        print(*FAILED_MODULES, sep="\n")
        sys.exit(1)
