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

#include <mathse.h>

#ifdef __cpluscplus
extern "C" {
#endif

#define MG_LOGLEVEL_ALL   (-99999) // print all log message
#define MG_LOGLEVEL_NONE  99999    // no log
#define MG_LOGLEVEL_TRACE 1        // eg for each field value parsed
#define MG_LOGLEVEL_DEBUG 2        // only debug level
#define MG_LOGLEVEL_INFO  3        // only general info and object codes/names
#define MG_LOGLEVEL_WARN  4        // only warning messages
#define MG_LOGLEVEL_ERROR 5        // only error message
#define MG_LOGLEVEL_FATAL 6        // fatal error, the programe will abort

#ifdef _DEBUG
#define MG_LOGLEVEL MG_LOGLEVEL_DEBUG
#else
#define MG_LOGLEVEL MG_LOGLEVEL_ERROR
#endif

#define HANDLER fprintf
#define OUTPUT  stderr

#define LOG(level, ...)                           \
    {                                             \
        if (MG_LOGLEVEL_##level >= MG_LOGLEVEL) { \
            HANDLER(OUTPUT, __VA_ARGS__);         \
        }                                         \
    }

#define LOG_FATAL(...)                          \
    {                                           \
        if (MG_LOGLEVEL_FATAL >= MG_LOGLEVEL) { \
            HANDLER(OUTPUT, "FATAL: ");         \
            LOG(FATAL, __VA_ARGS__)             \
            HANDLER(OUTPUT, "\n");              \
            abort();                            \
        }                                       \
    }

#define LOG_ERROR(...)                          \
    {                                           \
        if (MG_LOGLEVEL_ERROR >= MG_LOGLEVEL) { \
            HANDLER(OUTPUT, "ERROR: ");         \
            LOG(ERROR, __VA_ARGS__)             \
            HANDLER(OUTPUT, "\n");              \
            abort();                            \
        }                                       \
    }

#define LOG_WARN(...)                          \
    {                                          \
        if (MG_LOGLEVEL_WARN >= MG_LOGLEVEL) { \
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