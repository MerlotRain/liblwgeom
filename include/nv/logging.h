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

#define NV_LOGLEVEL_ALL   (-99999) // print all log message
#define NV_LOGLEVEL_NONE  99999    // no log
#define NV_LOGLEVEL_TRACE 1        // eg for each field value parsed
#define NV_LOGLEVEL_DEBUG 2        // only debug level
#define NV_LOGLEVEL_INFO  3        // only general info and object codes/names
#define NV_LOGLEVEL_WARN  4        // only warning messages
#define NV_LOGLEVEL_ERROR 5        // only error message
#define NV_LOGLEVEL_FATAL 6        // fatal error, the programe will abort

#ifdef _DEBUG
#define NV_LOGLEVEL NV_LOGLEVEL_DEBUG
#else
#define NV_LOGLEVEL NV_LOGLEVEL_ERROR
#endif

#define HANDLER fprintf
#define OUTPUT  stderr

#define NV_LOG(level, ...)                           \
    {                                             \
        if (NV_LOGLEVEL_##level >= NV_LOGLEVEL) { \
            HANDLER(OUTPUT, __VA_ARGS__);         \
        }                                         \
    }

#define NV_LOG_FATAL(...)                          \
    {                                           \
        if (NV_LOGLEVEL_FATAL >= NV_LOGLEVEL) { \
            HANDLER(OUTPUT, "FATAL: ");         \
            NV_LOG(FATAL, __VA_ARGS__)             \
            HANDLER(OUTPUT, "\n");              \
            abort();                            \
        }                                       \
    }

#define NV_LOG_ERROR(...)                          \
    {                                           \
        if (NV_LOGLEVEL_ERROR >= NV_LOGLEVEL) { \
            HANDLER(OUTPUT, "ERROR: ");         \
            NV_LOG(ERROR, __VA_ARGS__)             \
            HANDLER(OUTPUT, "\n");              \
        }                                       \
    }

#define NV_LOG_WARN(...)                          \
    {                                          \
        if (NV_LOGLEVEL_WARN >= NV_LOGLEVEL) { \
            HANDLER(OUTPUT, "WARN: ");         \
            NV_LOG(WARN, __VA_ARGS__)             \
            HANDLER(OUTPUT, "\n");             \
            abort();                           \
        }                                      \
    }

#define NV_LOG_INFO(...)  NV_LOG(INFO, __VA_ARGS__)
#define NV_LOG_DEBUG(...) NV_LOG(DEBUG, __VA_ARGS__)
#define NV_LOG_TRACE(...) NV_LOG(TRACE, __VA_ARGS__)

#endif