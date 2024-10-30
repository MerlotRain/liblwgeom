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

#ifndef LW_LOGGER_H
#define LW_LOGGER_H 1

#include <zlog.h>

extern zlog_category_t *log_category;

extern int log_init();
extern void log_fini();

#define ESC_START    "\033["
#define ESC_END      "\033[0m"
#define COLOR_FATAL  "31;40;5m"
#define COLOR_ALERT  "31;40;1m"
#define COLOR_CRIT   "31;40;1m"
#define COLOR_ERROR  "35;40;1m"
#define COLOR_WARN   "33;40;1m"
#define COLOR_NOTICE "34;40;1m"
#define COLOR_INFO   "32;40;1m"
#define COLOR_DEBUG  "36;40;1m"
#define COLOR_TRACE  "37;40;1m"

#define LOG_FATAL(fmt, args...) \
	zlog(log_category, \
	     __FILE__, \
	     sizeof(__FILE__) - 1, \
	     __func__, \
	     sizeof(__func__) - 1, \
	     __LINE__, \
	     ZLOG_LEVEL_FATAL, \
	     ESC_START COLOR_FATAL fmt ESC_END, \
	     ##args)

#define LOG_ERROR(fmt, args...) \
	zlog(log_category, \
	     __FILE__, \
	     sizeof(__FILE__) - 1, \
	     __func__, \
	     sizeof(__func__) - 1, \
	     __LINE__, \
	     ZLOG_LEVEL_ERROR, \
	     ESC_START COLOR_ERROR fmt ESC_END, \
	     ##args)

#define LOG_WARN(fmt, args...) \
	zlog(log_category, \
	     __FILE__, \
	     sizeof(__FILE__) - 1, \
	     __func__, \
	     sizeof(__func__) - 1, \
	     __LINE__, \
	     ZLOG_LEVEL_WARN, \
	     ESC_START COLOR_WARN fmt ESC_END, \
	     ##args)

#define LOG_NOTICE(fmt, args...) \
	zlog(log_category, \
	     __FILE__, \
	     sizeof(__FILE__) - 1, \
	     __func__, \
	     sizeof(__func__) - 1, \
	     __LINE__, \
	     ZLOG_LEVEL_NOTICE, \
	     ESC_START COLOR_NOTICE fmt ESC_END, \
	     ##args)

#define LOG_INFO(fmt, args...) \
	zlog(log_category, \
	     __FILE__, \
	     sizeof(__FILE__) - 1, \
	     __func__, \
	     sizeof(__func__) - 1, \
	     __LINE__, \
	     ZLOG_LEVEL_INFO, \
	     ESC_START COLOR_INFO fmt ESC_END, \
	     ##args)

#define LOG_DEBUG(fmt, args...) \
	zlog(log_category, \
	     __FILE__, \
	     sizeof(__FILE__) - 1, \
	     __func__, \
	     sizeof(__func__) - 1, \
	     __LINE__, \
	     ZLOG_LEVEL_DEBUG, \
	     ESC_START COLOR_DEBUG fmt ESC_END, \
	     ##args)

#endif /* LW_LOGGER_H */