[![Build & test](https://github.com/przemek83/commentr/actions/workflows/buld-and-test.yml/badge.svg)](https://github.com/przemek83/commentr/actions/workflows/buld-and-test.yml)
[![CodeQL](https://github.com/przemek83/commentr/actions/workflows/codeql.yml/badge.svg)](https://github.com/przemek83/commentr/actions/workflows/codeql.yml)
[![codecov](https://codecov.io/gh/przemek83/commentr/graph/badge.svg?token=HC606K7OV2)](https://codecov.io/gh/przemek83/commentr)

[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=przemek83_commentr&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=przemek83_commentr)
[![Bugs](https://sonarcloud.io/api/project_badges/measure?project=przemek83_commentr&metric=bugs)](https://sonarcloud.io/summary/new_code?id=przemek83_commentr)
[![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=przemek83_commentr&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=przemek83_commentr)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=przemek83_commentr&metric=coverage)](https://sonarcloud.io/summary/new_code?id=przemek83_commentr)
[![Duplicated Lines (%)](https://sonarcloud.io/api/project_badges/measure?project=przemek83_commentr&metric=duplicated_lines_density)](https://sonarcloud.io/summary/new_code?id=przemek83_commentr)

## Example screenshot 
![](screen.png?raw=true "")

# About
Android code editor - Work In Progress

# Building:  
Clone and use CMake with GCC/Clang/MSVC to compile the project and tests from an IDE or command line. CMake **should**:
- configure everything automatically,
- compile and create binaries.

## Used tools and libs
| Tool |  Windows | Ubuntu |
| --- | --- | --- |
| OS version | 11 26H1 | 24.04 |
|Qt | 6.8.3 | 6.8.3 |
| GCC | 13.1.0 | 13.3.0 |
| CMake | 4.3.4 | 4.3.4 |
| Git | 2.46.0 | 2.43.0 |

# Usage:  
## Launching:  
[TODO]

## Interface
[TODO]

# License
This project is licensed under the GPL v3 License. See the LICENSE file for details.

The project uses the following software:
| Name | License | Home | Description |
| --- | --- | --- | --- |
| Qt | LGPLv3 | https://www.qt.io/| cross-platform application development framework |

# Testing
For testing purposes, the QtTest framework is used. Build the project first. Make sure that the `commentr-tests` target is built. Modern IDEs supporting CMake also support running tests with monitoring of failures. But in case you would like to run it manually, go to the `build/tests` directory, where the⁣ binary `commentr-tests` should be available. Launching it should produce the following output on Linux:
Example run:
```
$ ./commentr-tests
********* Start testing of JavaHighlighterTest *********
Config: Using QtTest library 6.8.3, Qt 6.8.3 (x86_64-little_endian-lp64 shared (dynamic) release build; by GCC 10.3.1 20210422 (Red Hat 10.3.1-1)), ubuntu 24.04
PASS   : JavaHighlighterTest::initTestCase()
PASS   : JavaHighlighterTest::testKeywordHighlighting()
PASS   : JavaHighlighterTest::testFunctionHighlighting()
PASS   : JavaHighlighterTest::testQuotationAndCommentHighlighting()
PASS   : JavaHighlighterTest::testMultilineCommentHighlighting()
PASS   : JavaHighlighterTest::cleanupTestCase()
Totals: 6 passed, 0 failed, 0 skipped, 0 blacklisted, 4ms
********* Finished testing of JavaHighlighterTest *********
(...)
********* Start testing of TestCommon *********
Config: Using QtTest library 6.8.3, Qt 6.8.3 (x86_64-little_endian-lp64 shared (dynamic) release build; by GCC 10.3.1 20210422 (Red Hat 10.3.1-1)), ubuntu 24.04
PASS   : TestCommon::initTestCase()
PASS   : TestCommon::testLoadFile()
PASS   : TestCommon::testSaveFile()
PASS   : TestCommon::testNormalizeFont()
PASS   : TestCommon::testMainWindow()
PASS   : TestCommon::testSampleGetFormat()
PASS   : TestCommon::cleanupTestCase()
Totals: 7 passed, 0 failed, 0 skipped, 0 blacklisted, 1ms
********* Finished testing of TestCommon *********

```
As an alternative, CTest can be used to run tests from the `build/tests` directory:
```
$ ctest
Test project <path>/commentr/build/tests
    Start 1: commentr-tests
1/1 Test #1: commentr-tests ...................   Passed    0.17 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.17 sec
```

# Additional info
[TODO]

# Potential further improvements
[TODO]

