/*****************************************************************************/
/*  Math Spatial Engine - Open source 2D geometry algorithm library          */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/



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


/// module util
extern int util_stokSuite();
/// collection of util suite
static int util_suite()
{
    int r = util_stokSuite();
    return r;
}


/// module mg
extern int mg_pointSuite();
/// collection of mg suite
static int mg_suite()
{
    int r = mg_pointSuite();
    return r;
}


/// register all suites
int register_suite()
{
    int sr = util_suite() + mg_suite();
    return sr;
}