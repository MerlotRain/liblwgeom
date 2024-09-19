cmake_minimum_required(VERSION 3.0...3.20)
project(liblwgeom LANGUAGES C CXX)

set(lwgeom lwgeom)
if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -wd4244 -wd4800 -wd4805 -wd4101 -D_CRT_NONSTDC_NO_WARNINGS")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Git)
set(PACKAGE_VERSION "")
execute_process(COMMAND ${GIT_EXECUTABLE} describe --long --tags --always
                  OUTPUT_VARIABLE NL_PACKAGE_VERSION)
string(STRIP "${NL_PACKAGE_VERSION}" PACKAGE_VERSION)

option(BUILD_TESTS "build liblwgeom tests" OFF)
option(BUILD_WITH_ICONV "build with icolwgeom" OFF)
option(BUILD_WITH_PROJ "build with proj" OFF)
option(BUILD_WITH_CAIRO "build with cairo" OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CUnit cunit)
if(BUILD_WITH_CAIRO)
  pkg_check_modules(CAIRO cairo)
  message("found cario: " ${CAIRO_FOUND})
  message("CAIRO_LIBRARIES=${CAIRO_LIBRARIES}")
  message("CAIRO_INCLUDE_DIRS=${CAIRO_INCLUDE_DIRS}")
endif()

if(BUILD_WITH_ICONV)
  find_package(Icolwgeom REQUIRED)
endif()

if(BUILD_WITH_PROJ)
  find_package(PROJ REQUIRED)
endif()

include(CheckIncludeFile)
CHECK_INCLUDE_FILE("stddef.h" HAVE_STDDEF_H)
CHECK_INCLUDE_FILE("stdlib.h" HAVE_STDLIB_H)
CHECK_INCLUDE_FILE("string.h" HAVE_STRING_H)
CHECK_INCLUDE_FILE("strings.h" HAVE_STRINGS_H)
CHECK_INCLUDE_FILE("libgen.h" HAVE_LIBGEN_H)
CHECK_INCLUDE_FILE("unistd.h" HAVE_UNISTD_H)
CHECK_INCLUDE_FILE("float.h" HAVE_FLOAT_H)
CHECK_INCLUDE_FILE("ctype.h" HAVE_CTYPE_H)
CHECK_INCLUDE_FILE("wchar.h" HAVE_WCHAR_H)
CHECK_INCLUDE_FILE("wctype.h" HAVE_WCTYPE_H)
CHECK_INCLUDE_FILE("alloca.h" HAVE_ALLOCA_H)
CHECK_INCLUDE_FILE("getopt.h" HAVE_GETOPT_H)
CHECK_INCLUDE_FILE("stdint.h" HAVE_STDINT_H)
CHECK_INCLUDE_FILE("inttypes.h" HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE("limits.h" HAVE_LIMITS_H)
CHECK_INCLUDE_FILE("malloc.h" HAVE_MALLOC_H)
CHECK_INCLUDE_FILE("memory.h" HAVE_MEMORY_H)
CHECK_INCLUDE_FILE("dirent.h" HAVE_DIRENT_H)
CHECK_INCLUDE_FILE("endian.h" HAVE_ENDIAN_H)
CHECK_INCLUDE_FILE("sys/endian.h" HAVE_SYS_ENDIAN_H)
CHECK_INCLUDE_FILE("byteorder.h" HAVE_BYTEORDER_H)
CHECK_INCLUDE_FILE("sys/byteorder.h" HAVE_SYS_BYTEORDER_H)
CHECK_INCLUDE_FILE("byteswap.h" HAVE_BYTESWAP_H)
if (WIN32)
  CHECK_INCLUDE_FILE("winsock2.h" HAVE_WINSOCK2_H)
elseif (APPLE)
  CHECK_INCLUDE_FILE("machine/endian.h" HAVE_MACHINE_ENDIAN_H)
endif()
CHECK_INCLUDE_FILE("icolwgeom.h" HAVE_ICONV_H)
CHECK_INCLUDE_FILE("pcre2.h" HAVE_PCRE2_H)
CHECK_INCLUDE_FILE("sys/stat.h" HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE("direct.h" HAVE_DIRECT_H)
CHECK_INCLUDE_FILE("sys/param.h" HAVE_SYS_PARAM_H)
CHECK_INCLUDE_FILE("sys/time.h" HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILE("sys/types.h" HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE("libps/pslib.h" HAVE_LIBPS_PSLIB_H)


include(CheckSymbolExists)
check_symbol_exists("floor" "math.h" HAVE_FLOOR)
check_symbol_exists("gettimeofday" "sys/time.h" HAVE_GETTIMEOFDAY)
check_symbol_exists("memchr" "string.h" HAVE_MEMCHR)
check_symbol_exists("memmem" "string.h" HAVE_MEMMEM)
check_symbol_exists("memmove" "string.h" HAVE_MEMMOVE)
check_symbol_exists("realloc" "stdlib.h" HAVE_REALLOC)
check_symbol_exists("scandir" "dirent.h" HAVE_SCANDIR)
check_symbol_exists("setelwgeom" "stdlib.h" HAVE_SETENV)
check_symbol_exists("strcasecmp" "strings.h" HAVE_STRCASECMP)
check_symbol_exists("strcasestr" "string.h" HAVE_STRCASESTR)
check_symbol_exists("strchr" "string.h" HAVE_STRCHR)
check_symbol_exists("strnlen" "string.h" HAVE_STRNLEN)
check_symbol_exists("strrchr" "string.h" HAVE_STRRCHR)
check_symbol_exists("strtol" "stdlib.h" HAVE_STRTOL)
check_symbol_exists("strtoul" "stdlib.h" HAVE_STRTOUL)
check_symbol_exists("strtoull" "stdlib.h" HAVE_STRTOULL)
check_symbol_exists("wcscmp" "wchar.h" HAVE_WCSCMP)
check_symbol_exists("wcscpy" "wchar.h" HAVE_WCSCPY)
check_symbol_exists("wcslen" "wchar.h" HAVE_WCSLEN)
check_symbol_exists("wcsnlen" "wchar.h" HAVE_WCSNLEN)


#-----------------------------------------------------------------------------
# Setup build directories
#-----------------------------------------------------------------------------
# Place executables and shared libraries in the same location for
# colwgeomenience of direct execution from common spot and for
# colwgeomenience in elwgeomironments without RPATH support.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

#-----------------------------------------------------------------------------
# version about
#-----------------------------------------------------------------------------
set(LWGEOM_VERSION_MAJOR 1)
set(LWGEOM_VERSION_MINOR 0)
set(LWGEOM_VERSION_PATCH 1)

#-----------------------------------------------------------------------------
# source files
#-----------------------------------------------------------------------------
file(GLOB_RECURSE lwgeom_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.c CONFIGURE_DEPEND)
file(GLOB_RECURSE lwgeom_HEADER_PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/*.h CONFIGURE_DEPEND)

add_library(${lwgeom} STATIC
  ${lwgeom_SOURCES}
  ${lwgeom_HEADER_PRIVATE}
)

if(WIN32)
  target_compile_definitions(${lwgeom} PUBLIC _USE_MATH_DEFINES)
endif()

target_include_directories(${lwgeom} PUBLIC 
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_include_directories(${lwgeom} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(BUILD_WITH_CAIRO)
  target_include_directories(${lwgeom} PRIVATE ${CAIRO_INCLUDE_DIRS})
  target_link_libraries(${lwgeom} ${CAIRO_LIBRARIES})
endif()

if(BUILD_WITH_OPENCV)
  target_link_libraries(${lwgeom} ${OpenCV_LIBS})
endif()

if(BUILD_WITH_PROJ)
  target_link_libraries(${lwgeom} PROJ::proj)
endif()

if(BUILD_TESTS)
  include(CTest)
endif()