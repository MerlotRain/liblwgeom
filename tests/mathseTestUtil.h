/**
 * Copyright (c) 2023-present Merlot.Rain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/// Naming conventions and registration methods for test suites
/// ---
/// The test cases for a module need to be placed in a file consisting of the
/// module name and the Test suffix. For example, the test file for the stok
/// module is named stokTest.c

/// Each file should be placed in its corresponding folder, for example,
/// stokTest.c is located in the util folder.

/// The name of the test suite needs to start with the name of the folder
/// structure where it is located, for example, the file 'til/tokTest.c' should
/// be named 'util-stok'. This naming can facilitate cmake to generate
/// corresponding test units based on file names.

extern int algorithm_pointSuite();
extern int algorithm_ellipseSuite();

extern int io_wktSuite();

/// register all suites
int register_suite()
{
    int sr = io_wktSuite() + algorithm_pointSuite() + algorithm_ellipseSuite();
    return sr;
}