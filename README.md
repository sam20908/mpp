# mpp

## Not actively maintained anymore, but I will try to work on it when I have time.

---

A modern C++ matrix library

[![Build Status](https://dev.azure.com/samestimable2016/mpp/_apis/build/status/sam20908.mpp?branchName=main)](https://dev.azure.com/samestimable2016/mpp/_build/latest?definitionId=3&branchName=main)

#### Tested Compilers:

* GCC 10
* VS 16.10 (works up to VS 16.11.2, see https://github.com/microsoft/STL/issues/1814#issuecomment-845572895)

## Table of contents

#### Just getting started? Go [here](docs/demo.md)!

When you know the basics of how the library works, you can move on to these:

* Learn about extra iterator functionalities [here](docs/more_iter_funcs.md)
* Learn about algorithms [here](docs/algos.md)
* Learn about utilities [here](docs/utils.md)
* Learn about customizations [here](docs/customize.md)

---

## FAQ

#### Why assertions instead of exceptions?

The operations have mathematical preconditions, which means it's safe to assume that the user knows what they're doing, but assertions can still be triggered in debug mode.
