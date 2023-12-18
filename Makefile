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
SE_AL
