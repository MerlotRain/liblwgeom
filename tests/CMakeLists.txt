file(GLOB_RECURSE _sources ${CMAKE_CURRENT_LIST_DIR}/*.c CONFIGURE_DEPEND)
add_executable(mathseUnitTest ${_sources})
target_link_libraries(mathseUnitTest cunit mathse)

file(GLOB_RECURSE _testfiles ${CMAKE_CURRENT_LIST_DIR}/**/*Test.c CONFIGURE_DEPEND)

message("_testfiles=${_testfiles}")
enable_testing() 

foreach(_testfile ${_testfiles})
    string(REPLACE ${CMAKE_CURRENT_LIST_DIR}/ "" _testname ${_testfile})
    string(REPLACE "Test.c" "" _testname ${_testname})
    string(REPLACE "/" "-" _cmake_testname  ${_testname})
    add_test(NAME unit-${_cmake_testname} COMMAND mathseUnitTest ${_testname})
endforeach()