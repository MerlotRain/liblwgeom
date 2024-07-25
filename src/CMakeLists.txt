set(se_srcs 
    mathse.c

    io/ewkb.cpp
    io/ewkt.cpp
    io/geojson.cpp
    io/gml.cpp
    io/kml.cpp
    io/ora.cpp
    io/wkb.cpp
    io/wkt.cpp
)

add_library(mathse STATIC)


target_sources(mathse PRIVATE ${se_srcs})
target_include_directories(mathse PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
)
