# mathSE Makefile

PREFIX?=/usr/local
INSTALL_BIN=${PREFIX}/bin
INSTALL=install
PKG_CONFIG?=pkg-config

FINAL_LIBS=-lm
DEBUG=-g -ggdb

CCCOLOR="\033[34m"
LINKCOLOR="\033[34;1m"
SRCCOLOR="\033[33m"
BINCOLOR="\033[37;1m"
MAKECOLOR="\033[32;1m"
ENDCOLOR="\033[0m"

SE_NAME=mathSE
SE_ALGORITHM_OBJ = pg_clone.o pv_area.o pv_width.p
SE_DS_OBJ = btree.o hashtable.o sort.o
SE_INDEX_OBJ = kdtree.o quadtree.o strtree.o 
SE_IO_OBJ = ewkb.o ewkt.o geojson.o gml.o kml.o wkb.o wkt.o
SE_TEST_OBJ
SE_BASE_OBJ = coordinate.o crs.o endian.o geohash.o i4.o table.o

CC = gcc

.PHONY: clean

clean:
	- rm -rf *.o
