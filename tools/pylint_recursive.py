#! /usr/bin/env python3

'''
Module that runs pylint on all python scripts found in a directory tree..

Originally from https://gist.github.com/MJJoyce/5835693. Edited with the following changes:
- Adapted to mpp project's pylintrc
- Only scan for .py files in whitelisted directories (relative to the root directory)
- Display which files failed to meet required score and exit with non-zero code
- Fixed missing space in "Using config file at" and the configuration filename
- Added color output
- Don't print previous score because we are not using presistent comparison
- Changed shebang to python3
- Fixed dividing by 0 when no modules are found
'''

# pylint: disable=C0301

import os
import re
import sys
import pathlib
import colorama

TOTAL = 0.0
COUNT = 0

WHITELISTED_DIRS = ["benchmarks", "tests", "tools"]
FAILED_MODULES = []

CATEGORY_COLORS = {"E": colorama.Fore.LIGHTRED_EX, "F": colorama.Fore.RED,
                   "W": colorama.Fore.YELLOW, "C": colorama.Fore.CYAN,
                   "R": colorama.Fore.LIGHTYELLOW_EX,
                   "I": colorama.Style.DIM}


def check(cfg_dir, module):
    '''
    apply pylint to the file specified if it is a *.py or *.py.in file
    '''
    global TOTAL, COUNT, FAILED_MODULES  # pylint: disable=W0603

    if module.endswith(".py"):

        print("CHECKING " + module)
        print("--" * 50)

        current_module_failed = False

        pout = os.popen(f"pylint {cfg_dir} {module}", "r")
        output = pout.read()

        if output == "":
            print(
                f"{module} is {colorama.Fore.MAGENTA}skipped by Pylint{colorama.Style.RESET_ALL}\n")
            return

        for line in output.split("\n"):
            if "Your code has been rated at" in line:
                # Since we disabled persistent comparison, grab the current score
                score = re.findall(r"\d+.\d\d", line)[0]
                newline_at_beginning = "\n" if current_module_failed else ""
                TOTAL += float(score)
                COUNT += 1

                print(
                    f"{newline_at_beginning}Your code has been rated at {colorama.Fore.GREEN}{score}{colorama.Style.RESET_ALL}/10\n")

                if current_module_failed:
                    FAILED_MODULES.append(module)
            else:
                # NOTE (bugfix): Don't use match because that only matches the beginning of the string
                groups = re.search(
                    r"^(.*:\d+:\d+:) ([A-Z]+)(\d+): (.*)", line)

                if not groups is None:
                    fault_at_module_location = groups.group(1)
                    category = groups.group(2)
                    code_num = groups.group(3)
                    message = groups.group(4)
                    color = CATEGORY_COLORS[category]

                    print(
                        f"{fault_at_module_location} {color}{category}{code_num}: {message}{colorama.Style.RESET_ALL}")

                    current_module_failed = True


if __name__ == "__main__":
    colorama.init()

    try:
        CONFIG_DIRECTORY = "--rcfile=" + sys.argv[1]
        print("Using config file at " + sys.argv[1])
    except IndexError:
        CONFIG_DIRECTORY = ""
        print("No config file passed. Defaulting to PEP8...")

    print("Getting *.py files in whitelisted directories")
    print("Whitelisted directories are: " + ", ".join(WHITELISTED_DIRS))
    print("")

    for whitelisted_dir in WHITELISTED_DIRS:
        for file in pathlib.Path(whitelisted_dir).rglob("*.py"):
            check(CONFIG_DIRECTORY, str(file))

    print("==" * 50)
    print("%d modules checked" % COUNT)
    print(
        f"AVERAGE SCORE = {colorama.Fore.GREEN}%.02f{colorama.Style.RESET_ALL}/10" % (TOTAL / (1 if COUNT == 0 else COUNT)))

    if len(FAILED_MODULES) > 0:
        print("==" * 50)
        print("The following files needs to be fixed:\n")
        print(*FAILED_MODULES, sep="\n")
        sys.exit(1)
