##
# dwl-bar
#
# @file
# @version 0.0
VERSION    = 0.0
PKG_CONFIG = pkg-config

# paths
PREFIX = /usr/local
MANDIR = $(PREFIX)/share/man
SRCDIR = src

PKGS   = wayland-client wayland-cursor pangocairo
FILES  = $(SRCDIR)/main.c $(SRCDIR)/main.h $(SRCDIR)/log.c $(SRCDIR)/log.h \
		 $(SRCDIR)/render.c $(SRCDIR)/render.h $(SRCDIR)/event.c $(SRCDIR)/event.h \
		 $(SRCDIR)/util.c $(SRCDIR)/util.h $(SRCDIR)/shm.c $(SRCDIR)/shm.h \
		 $(SRCDIR)/input.c $(SRCDIR)/input.h $(SRCDIR)/user.c $(SRCDIR)/user.h \
		 $(SRCDIR)/bar.c $(SRCDIR)/bar.h $(SRCDIR)/config.h
OBJS   = $(SRCDIR)/xdg-output-unstable-v1-protocol.o $(SRCDIR)/xdg-shell-protocol.o \
		 $(SRCDIR)/wlr-layer-shell-unstable-v1-protocol.o
OBJS  := $(filter-out $(SRCDIR)/xdg-output-unstable-v1-protocol.o,$(OBJS))
OBJS  += $(SRCDIR)/dwl-ipc-unstable-v2-protocol.o

## Compile Flags
CC        = gcc
BARCFLAGS = `$(PKG_CONFIG) --cflags $(PKGS)` $(CFLAGS)
BARLIBS   = `$(PKG_CONFIG) --libs $(PKGS)` $(LIBS)

WAYLAND_SCANNER   = `$(PKG_CONFIG) --variable=wayland_scanner wayland-scanner`
WAYLAND_PROTOCOLS = `$(PKG_CONFIG) --variable=pkgdatadir wayland-protocols`


all: dwl-bar
dwl-bar: $(FILES) $(OBJS)
	$(CC) $^ $(BARLIBS) $(BARCFLAGS) -o $@
$(SRCDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	$(CC) -c $< $(BARLIBS) $(BARCFLAGS) -o $@

$(SRCDIR)/xdg-shell-protocol.h:
	$(WAYLAND_SCANNER) client-header \
		$(WAYLAND_PROTOCOLS)/stable/xdg-shell/xdg-shell.xml $@
$(SRCDIR)/xdg-shell-protocol.c:
	$(WAYLAND_SCANNER) private-code \
		$(WAYLAND_PROTOCOLS)/stable/xdg-shell/xdg-shell.xml $@

$(SRCDIR)/wlr-layer-shell-unstable-v1-protocol.h:
	$(WAYLAND_SCANNER) client-header \
		protocols/wlr-layer-shell-unstable-v1.xml $@
$(SRCDIR)/wlr-layer-shell-unstable-v1-protocol.c:
	$(WAYLAND_SCANNER) private-code \
		protocols/wlr-layer-shell-unstable-v1.xml $@

$(SRCDIR)/dwl-ipc-unstable-v2-protocol.h:
	$(WAYLAND_SCANNER) client-header \
		protocols/dwl-ipc-unstable-v2.xml $@
$(SRCDIR)/dwl-ipc-unstable-v2-protocol.c:
	$(WAYLAND_SCANNER) private-code \
		protocols/dwl-ipc-unstable-v2.xml $@

$(SRCDIR)/config.h:
	cp src/config.def.h $@

dev: clean $(SRCDIR)/config.h $(OBJS)

clean:
	rm -f dwl-bar src/config.h src/*.o src/*-protocol.* *.orig *.rej src/*.orig src/*.rej config.h

dist: clean
	mkdir -p dwl-bar-$(VERSION)
	cp -R LICENSE Makefile README.md dwl-bar.1 src protocols \
		dwl-bar-$(VERSION)
	tar -caf dwl-bar-$(VERSION).tar.gz dwl-bar-$(VERSION)
	rm -rf dwl-bar-$(VERSION)

install: dwl-bar
	mkdir -p $(PREFIX)/bin
	cp -f dwl-bar $(PREFIX)/bin
	chmod 755 $(PREFIX)/bin/dwl-bar
	mkdir -p $(MANDIR)/man1
	cp -f dwl-bar.1 $(MANDIR)/man1
	chmod 644 $(MANDIR)/man1/dwl-bar.1

uninstall:
	rm -f $(PREFIX)/bin/dwl-bar $(MANDIR)/man1/dwl-bar.1

# end
