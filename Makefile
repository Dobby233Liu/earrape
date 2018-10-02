# Based on Leurak's work: https://github.com/Leurak/MEMZ/blob/887e311/WindowsTrojan/Makefile

CC = i686-w64-mingw32-gcc

CCFLAGS = -mwindows
LIBS    = -lwinmm

BUILDDIR = dist

CFILES = $(shell find src -name '*.c' -type f)

all: dir $(BUILDDIR)/earrape.exe

dir: $(BUILDDIR)

$(BUILDDIR):
	mkdir $(BUILDDIR)

$(BUILDDIR)/earrape.exe: $(CFILES)
	$(CC) $(CFILES) $(CCFLAGS) $(LIBS) -o $@

clean: $(BUILDDIR)
	rm -r $(BUILDDIR)

.PHONY: clean all dir exe