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

#ifndef LOGGING_H
#define LOGGING_H

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