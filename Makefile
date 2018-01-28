CFLAGS  := -Iinclude $(CFLAGS)
LDFLAGS := -shared $(LDFLAGS)

OBJECTS  = wread math getopt printopts strrev strwcpy
INCLUDES = ftr math getopt string

OBJECTS  := $(patsubst %,obj/%.o,$(OBJECTS))
INCLUDES := $(patsubst %,include/j/%.h,$(INCLUDES))

BIN    ?= a.out
ABIN   ?= b.out

SO     ?= libj.so
SO_PAT := $(SO).2.0.0
SO_REV := $(SO).2.0
SO_INT := $(SO).2

A      ?= libj.a
PREFIX ?= /usr/local

ARCH := $(shell getconf LONG_BIT)
ifdef LIB32
ifeq ($(ARCH), 64)
CFLAGS := -m32 $(CFLAGS)
SO_INSTALL ?= $(PREFIX)/lib32
endif
else
SO_INSTALL ?= $(PREFIX)/lib
endif

INCLUDE_INSTALL ?= $(PREFIX)/include/j

.PHONY: shared static install uninstall test clean remake

shared : $(SO)

static : $(A)

install: $(SO)
	install -m644 -d $(INCLUDE_INSTALL)
	install -m644 -t $(INCLUDE_INSTALL) $(INCLUDES)
	install -m644 -t $(SO_INSTALL) $(SO) $(SO_PAT) $(SO_REV) $(SO_INT) $(A)

uninstall:
	rm -rf $(INCLUDE_INSTALL)
	rm -f $(SO_INSTALL)/$(SO) $(SO_INSTALL)/$(SO_PAT) \
		$(SO_INSTALL)/$(SO_REV) $(SO_INSTALL)/$(SO_INT) $(SO_INSTALL)/$(A)

$(SO) : $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(SO_PAT) $^
	ln -sf $(SO_PAT) $(SO_REV)
	ln -sf $(SO_PAT) $(SO_INT)
	ln -sf $(SO_PAT) $(SO)

$(A) : $(OBJECTS)
	ar rc $@ $^
	ranlib $@

obj/%.o : src/%.c $(INCLUDES)
	$(CC) $(CFLAGS) -fpic -c -o $@ $<

test : $(SO) $(A)
	$(CC) $(CFLAGS) src/main.c -L. -Wl,-rpath=. -l:$(SO_PAT) -o $(BIN)
	$(CC) $(CFLAGS) src/main.c -L. -l:$(A) -o $(ABIN)

clean :
	rm -f $(OBJECTS)
	rm -f $(SO) $(SO_PAT) $(SO_REV) $(SO_INT) $(A) $(BIN) $(ABIN)

remake :
	$(MAKE) clean
	$(MAKE) shared
