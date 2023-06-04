# farbfeld version
VERSION = 4

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

# flags
CPPFLAGS = -D_DEFAULT_SOURCE
CFLAGS   = -std=c99 -pedantic -Wall -Wextra -Os
LDFLAGS  = -s
PNG-LDLIBS = -lpng
JPG-LDLIBS = -ljpeg

# compiler and linker
CC = cc
