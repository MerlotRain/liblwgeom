set(se_hdrs mathSE.h)
set(se_srcs coordinate.cpp)

add_library(mathSE SHARED)

GENERATE_EXPORT_HEADER(mathSE
EXPORT_MACRO_NAME SE_API
)

target_sources(mathSE PRIVATE ${se_srcs})
target_include_directories(mathSE PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_LIST_DIR})

