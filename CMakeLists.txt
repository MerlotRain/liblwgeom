cmake_minimum_required(VERSION 3.0...3.20)
project(libmathse C CXX)

if(MSVC)
    set(mathse libmathse)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -wd4244 -wd4800 -wd4805 -wd4101 -D_CRT_NONSTDC_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS")
else()
    set(mathse mathse)
endif()

find_package(PNG REQUIRED)
find_package(Git)
set(PACKAGE_VERSION "")
execute_process(COMMAND ${GIT_EXECUTABLE} describe --long --tags --always
                  OUTPUT_VARIABLE NL_PACKAGE_VERSION)
string(STRIP "${NL_PACKAGE_VERSION}" PACKAGE_VERSION)

option(BUILD_SHARD_LIBRARY "build shared libmathse library" ON)
option(BUILD_TESTS "build libmathse tests" OFF)
option(BUILD_PROGRAMS "build program tools" OFF)

option(MATHSE_DISABLE_LIBPNG_CANVAS "on libmathse libpng support" OFF)
option(MATHSE_DISABLE_QT_CANVAS "on libmathse qt canvas support" OFF)


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
CHECK_INCLUDE_FILE("iconv.h" HAVE_ICONV_H)
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
check_symbol_exists("setenv" "stdlib.h" HAVE_SETENV)
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



set(mathse_HEADERS
    include/mathse.h
    include/mg.h
    include/btree.h
    include/geohash.h
    include/rtree.h

    include/util/buf.h
    include/util/encode.h
    include/util/hash.h
    include/util/logging.h
    include/util/sort.h
    include/util/stok.h
)

set(mathse_SOURCES
    src/mg.c
    
    src/algorithm/ellipse.c
    src/algorithm/point.c

    src/index/rtree.c
    src/index/geohash.c
    src/index/btree.c

    src/io/ewkb.c
    src/io/ewkt.c
    src/io/geojson.c
    src/io/gml.c
    src/io/kml.c
    src/io/wkb.c
    src/io/wkt.c

    src/util/buf.c
    src/util/stok.c
    src/util/hash.c
    src/util/base64.c
    src/util/md5.c
    src/util/sort.c
)

set(MATHSE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(BUILD_SHARD_LIBRARY)
  add_library(${mathse} SHARED
      ${mathse_HEADERS}
      ${mathse_SOURCES}
  )
  target_compile_definitions(${mathse} PRIVATE DLL_EXPORT)
else()
  add_library(${mathse} STATIC
    ${mathse_HEADERS}
    ${mathse_SOURCES}
  )
endif()

if(WIN32)
  target_compile_definitions(${mathse} PUBLIC _USE_MATH_DEFINES)
endif()

target_include_directories(${mathse} SYSTEM PUBLIC
  ${PNG_INCLUDE_DIRS}
)

target_include_directories(${mathse} PUBLIC 
  ${MATHSE_INCLUDE_DIR}
  ${MATHSE_INCLUDE_DIR}/util
)

target_include_directories(${mathse} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${mathse} PNG::PNG)


if(BUILD_TESTS)
  include(CTest)
  add_subdirectory(cunit)
  add_subdirectory(tests)
endif()