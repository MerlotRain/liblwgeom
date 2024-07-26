set(se_srcs 
    io.c

    io/ewkb.c
    io/ewkt.c
    io/geojson.c
    io/gml.c
    io/kml.c
    io/ora.c
    io/wkb.c
    io/wkt.c
)

add_library(mathse STATIC)


target_sources(mathse PRIVATE ${se_srcs})
target_include_directories(mathse PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
)
