# slstatus version
VERSION = 1.0

# customize below to fit your system

# paths
PREFIX = /usr
MANPREFIX = $(PREFIX)/share/man

X11INC = /usr/include
X11LIB = /usr/lib64

# flags
CPPFLAGS = -I$(X11INC) -D_DEFAULT_SOURCE
CFLAGS   = -march=native -std=c99 -pedantic -O2 --pipe -ftree-vectorize -Wall -Wextra

LDFLAGS  = -L$(X11LIB) -s
# OpenBSD: add -lsndio
# FreeBSD: add -lkvm
LDLIBS   = -lX11

# compiler and linker
CC = cc
