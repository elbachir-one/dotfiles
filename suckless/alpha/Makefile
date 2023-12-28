# alpha - a fork from simple terminal
# See LICENSE file for copyright and license details.

.POSIX:

include config.mk

SRC = alpha.c x.c boxdraw.c hb.c
OBJ = $(SRC:.c=.o)

all: options alpha

options:
	@echo alpha build options:
	@echo "CFLAGS  = $(STCFLAGS)"
	@echo "LDFLAGS = $(STLDFLAGS)"
	@echo "CC      = $(CC)"

config.h:
	cp config.def.h config.h

.c.o:
	$(CC) $(STCFLAGS) -c $<

alpha.o: config.h alpha.h win.h
x.o: arg.h config.h alpha.h win.h hb.h
hb.o: alpha.h
boxdraw.o: config.h alpha.h boxdraw_data.h

$(OBJ): config.h config.mk

alpha: $(OBJ)
	$(CC) -o $@ $(OBJ) $(STLDFLAGS)

clean:
	rm -f alpha $(OBJ) alpha-$(VERSION).tar.gz *.o *.orig *.rej config.h

dist: clean
	mkdir -p alpha-$(VERSION)
	cp -R FAQ LEGACY TODO LICENSE Makefile README config.mk\
		config.def.h alpha.info alpha.1 arg.h alpha.h win.h $(SRC)\
		alpha-$(VERSION)
	tar -cf - alpha-$(VERSION) | gzip > alpha-$(VERSION).tar.gz
	rm -rf alpha-$(VERSION)

install: alpha
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f alpha $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/alpha
	tic -sx alpha.info
	mkdir -p $(DESTDIR)$(PREFIX)/share/applications
	cp -f alpha.desktop $(DESTDIR)$(PREFIX)/share/applications
	@echo Please see the README file regarding the terminfo entry of alpha.

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/alpha
	rm -f $(DESTDIR)$(PREFIX)/share/applications/alpha.desktop

.PHONY: all options clean dist install uninstall
