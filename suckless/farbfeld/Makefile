# See LICENSE file for copyright and license details
# farbfeld - suckless image format with conversion tools
.POSIX:

include config.mk

REQ = util
HDR = arg.h
BIN = png2ff ff2png jpg2ff ff2jpg ff2pam ff2ppm
SCR = 2ff
MAN1 = 2ff.1 $(BIN:=.1)
MAN5 = farbfeld.5

all: $(BIN)

png2ff-LDLIBS = $(PNG-LDLIBS)
ff2png-LDLIBS = $(PNG-LDLIBS)
jpg2ff-LDLIBS = $(JPG-LDLIBS)
ff2jpg-LDLIBS = $(JPG-LDLIBS)

png2ff: png2ff.o $(REQ:=.o)
ff2png: ff2png.o $(REQ:=.o)
jpg2ff: jpg2ff.o $(REQ:=.o)
ff2jpg: ff2jpg.o $(REQ:=.o)
ff2pam: ff2pam.o $(REQ:=.o)
ff2ppm: ff2ppm.o $(REQ:=.o)

png2ff.o: png2ff.c config.mk $(HDR) $(REQ:=.h)
ff2png.o: ff2png.c config.mk $(HDR) $(REQ:=.h)
jpg2ff.o: jpg2ff.c config.mk $(HDR) $(REQ:=.h)
ff2jpg.o: ff2jpg.c config.mk $(HDR) $(REQ:=.h)
ff2pam.o: ff2pam.c config.mk $(HDR) $(REQ:=.h)
ff2ppm.o: ff2ppm.c config.mk $(HDR) $(REQ:=.h)

.o:
	$(CC) -o $@ $(LDFLAGS) $< $(REQ:=.o) $($*-LDLIBS)

.c.o:
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

clean:
	rm -f $(BIN) $(BIN:=.o) $(REQ:=.o)

dist:
	rm -rf "farbfeld-$(VERSION)"
	mkdir -p "farbfeld-$(VERSION)"
	cp -R FORMAT LICENSE Makefile README config.mk $(SCR) \
	      $(HDR) $(BIN:=.c) $(REQ:=.c) $(REQ:=.h) \
	      $(MAN1) $(MAN5) "farbfeld-$(VERSION)"
	tar -cf - "farbfeld-$(VERSION)" | gzip -c > "farbfeld-$(VERSION).tar.gz"
	rm -rf "farbfeld-$(VERSION)"

install: all
	mkdir -p "$(DESTDIR)$(PREFIX)/bin"
	cp -f $(SCR) $(BIN) "$(DESTDIR)$(PREFIX)/bin"
	for f in $(BIN) $(SCR); do chmod 755 "$(DESTDIR)$(PREFIX)/bin/$$f"; done
	mkdir -p "$(DESTDIR)$(MANPREFIX)/man1"
	cp -f $(MAN1) "$(DESTDIR)$(MANPREFIX)/man1"
	for m in $(MAN1); do chmod 644 "$(DESTDIR)$(MANPREFIX)/man1/$$m"; done
	mkdir -p "$(DESTDIR)$(MANPREFIX)/man5"
	cp -f $(MAN5) "$(DESTDIR)$(MANPREFIX)/man5"
	for m in $(MAN5); do chmod 644 "$(DESTDIR)$(MANPREFIX)/man5/$$m"; done

uninstall:
	for f in $(BIN) $(SCR); do rm -f "$(DESTDIR)$(PREFIX)/bin/$$f"; done
	for m in $(MAN1); do rm -f "$(DESTDIR)$(MANPREFIX)/man1/$$m"; done
	for m in $(MAN5); do rm -f "$(DESTDIR)$(MANPREFIX)/man5/$$m"; done
