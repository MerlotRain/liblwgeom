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

#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static char last_error[2048];

static int last_errno = 0;

void record_error(int code, const char *fmt, ...)
{
    memset(last_error, 0, 2048);
    va_list args;
    va_start(args, fmt);
    vsnprintf(last_error, sizeof(last_error), fmt, args);
    va_end(args);
}

int error_code()
{
    return last_errno;
}

char *error_messgae()
{
    return last_error;
}