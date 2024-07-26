/*****************************************************************************/
/*  MathSE - Open source 2D geometry algorithm library                       */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/



#ifndef ALLOC_H
#define ALLOC_H

#if defined(USE_TCMALLOC)

#    include <google/tcmalloc.h>

#    define malloc(size)        tc_malloc(size)
#    define calloc(count, size) tc_calloc(count, size)
#    define realloc(ptr, size)  tc_realloc(ptr, size)
#    define free(ptr)           tc_free(ptr)

#elif defined(USE_JEMALLOC)

#    include <jemalloc/jemalloc.h>

#    define malloc(size)        je_malloc(size)
#    define calloc(count, size) je_calloc(count, size)
#    define realloc(ptr, size)  je_realloc(ptr, size)
#    define free(ptr)           je_free(ptr)

#endif



#endif