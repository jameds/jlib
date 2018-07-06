# vim: ts=3 sw=3
# The following variables may be changed by the user.

CFLAGS:=-I. -std=c99 -Wall -pedantic -Werror -fno-stack-protector \
	-Wno-discarded-qualifiers -D_POSIX_C_SOURCE=200112L $(CFLAGS)
LDFLAGS:=-lm $(LDFLAGS)

version?=4
version_revision?=0
version_patch?=1

usr?=/usr/local

# I think Microsoft Windows/DOS uses PE or something.
ifdef elf386
CFLAGS:=-m32 $(CFLAGS)
LDFLAGS:=-m elf_i386 $(LDFLAGS)
lib?=lib32
else
lib?=lib
endif

static_bin=libj.a
shared_bin=libj.so.$(version).$(version_revision).$(version_patch)

# Builds a static version of the library.  The shared version is not built.
ifndef static
CFLAGS:=-fPIC $(CFLAGS)
bin=$(shared_bin)
else
bin=$(static_bin)
endif

## END USER VARIABLES

objects = error.o readwh.o \
			 getopt.o utility.o \
			 straprox.o memchr.o strrev.o strcasecmp.o

.PHONY : clean install uninstall

$(bin) :

clean :
	-rm -f $(objects)
	-rm -f $(static_bin) $(shared_bin)
	-rm -f libj.so libj.so.$(version) libj.so.$(version).$(version_revision)

install : $(bin)
	cp -Rf -t $(usr)/include j
	chmod -R a+rX $(usr)/include/j
	mkdir -p -ma+rx $(usr)/$(lib)
	cp -f -t $(usr)/$(lib) $(bin)
	chmod a+rx $(usr)/$(lib)/$(bin)
ifneq ($(static),0)
	ln -sf $(usr)/$(lib)/$(bin) $(usr)/lib/libj.so
	ln -sf $(usr)/$(lib)/$(bin) $(usr)/lib/libj.so.$(version)
	ln -sf $(usr)/$(lib)/$(bin) $(usr)/lib/libj.so.$(version).$(version_revision)
endif

uninstall :
	-rm -Rf $(usr)/include/j
	-rm -f $(usr)/$(lib)/$(bin)
ifneq ($(static),0)
	-rm -f $(usr)/$(lib)/libj.so
	-rm -f $(usr)/$(lib)/libj.so.$(version)
	-rm -f $(usr)/$(lib)/libj.so.$(version).$(version_revision)
endif

libj.so.$(version).$(version_revision).$(version_patch) : $(objects)
	$(LD) -shared $(LDFLAGS) -o $@ $(objects)
	ln -sf $@ libj.so
	ln -sf $@ libj.so.$(version)
	ln -sf $@ libj.so.$(version).$(version_revision)

libj.a : $(objects)
	$(AR) rc $@ $(objects)
	ranlib $@

.c.o :
	$(CC) -c $(CFLAGS) -o $@ $<
