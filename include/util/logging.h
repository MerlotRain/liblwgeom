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

#ifndef LOGGING_H
#define LOGGING_H

#ifdef __cpluscplus
extern "C" {
#endif

#include <mathse.h>

#define SE_LOGLEVEL_ALL   -99999 // print all log message
#define SE_LOGLEVEL_NONE  99999  // no log
#define SE_LOGLEVEL_TRACE 1      // eg for each field value parsed
#define SE_LOGLEVEL_DEBUG 2      // only debug level
#define SE_LOGLEVEL_INFO  3      // only general info and object codes/names
#define SE_LOGLEVEL_WARN  4      // only warning messages
#define SE_LOGLEVEL_ERROR 5      // only error message
#define SE_LOGLEVEL_FATAL 6      // fatal error, the programe will abort

#ifdef _DEBUG
#define SE_LOGLEVEL SE_LOGLEVEL_DEBUG
#else
#define SE_LOGLEVEL SE_LOGLEVEL_ERROR
#endif

#define HANDLER fprintf
#define OUTPUT  stderr

#define LOG(level, ...)                           \
    {                                             \
        if (SE_LOGLEVEL_##level >= SE_LOGLEVEL) { \
            HANDLER(OUTPUT, __VA_ARGS__);         \
        }                                         \
    }

#define LOG_FATAL(...)                          \
    {                                           \
        if (SE_LOGLEVEL_FATAL >= SE_LOGLEVEL) { \
            HANDLER(OUTPUT, "FATAL: ");         \
            LOG(FATAL, __VA_ARGS__)             \
            HANDLER(OUTPUT, "\n");              \
            abort();                            \
        }                                       \
    }

#define LOG_ERROR(...)                          \
    {                                           \
        if (SE_LOGLEVEL_ERROR >= SE_LOGLEVEL) { \
            HANDLER(OUTPUT, "ERROR: ");         \
            LOG(ERROR, __VA_ARGS__)             \
            HANDLER(OUTPUT, "\n");              \
            abort();                            \
        }                                       \
    }

#define LOG_WARN(...)                          \
    {                                          \
        if (SE_LOGLEVEL_WARN >= SE_LOGLEVEL) { \
            HANDLER(OUTPUT, "WARN: ");         \
            LOG(WARN, __VA_ARGS__)             \
            HANDLER(OUTPUT, "\n");             \
            abort();                           \
        }                                      \
    }

#define LOG_INFO(...)  LOG(INFO, __VA_ARGS__)
#define LOG_DEBUG(...) LOG(DEBUG, __VA_ARGS__)
#define LOG_TRACE(...) LOG(TRACE, __VA_ARGS__)

#ifdef __cpluscplus
}
#endif

#endif