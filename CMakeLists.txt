cmake_minimum_required(VERSION 3.10)
project(LIBLWGEOM LANGUAGES C)

set(LWGEOM_DEBUG_LEVEL 1)

set(lwgeom_SRCs 
    bitset.c
    bytebuffer.c
    geohash.c
    hashtable.c
    liblwgeom.c
    lwalgorithm.c
    lwbuilding_regularization.c
    lwdbscan.c
    lwgeom_centroid.c
    lwgeom_graph.c
    lwgeom_ordinate.c
    lwgeom_prop_geo.c
    lwgeom_prop_value.c
    lwgeom_simplifier.c
    lwin_ewkb.c
    lwin_ewkt.c
    lwin_geojson.c
    lwin_gml.c
    lwin_kml.c
    lwin_ora.c
    lwin_wkb.c
    lwin_wkt.c
    lwkmeans.c
    lwout_ewkb.c
    lwout_ewkt.c
    lwout_geojson.c
    lwout_gml.c
    lwout_kml.c
    lwout_ora.c
    lwout_wkb.c
    lwout_wkt.c
    lwutil.c
    mapsettings.c
    rtree.c
    sda.c
    stok.c 
)

add_library(lwgeom STATIC)

target_sources(lwgeom PRIVATE ${lwgeom_SRCs})
target_compile_definitions(lwgeom PRIVATE LWGEOM_DEBUG_LEVEL=${LWGEOM_DEBUG_LEVEL})