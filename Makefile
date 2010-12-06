# Part 0
# load common stuff
TOPDIR = .
include $(TOPDIR)/Makefile.common

# Part 1
# recursive make

SRC = panel.c misc.c bg.c eggtraymanager.c eggmarshalers.c fixedtip.c main.c gdk-helper.c
OBJ = $(SRC:%.c=%.o)
DEP = $(SRC:%.c=%.dep)

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
ifneq ($(MAKECMDGOALS),tar)
-include $(DEP)
endif
endif
endif


TARGET = trayer
$(TARGET): $(OBJ) 
	$(CC) $(LDFLAGS) $(LIBS) $(OBJ) -o $@
ifeq (,$(DEVEL))
	strip $@
endif

all: $(TARGET)


clean:
	$(RM) $(TARGET) $(OBJ) $(DEP) *~

distclean: 
	rm -f Makefile.config config.h

install: 
	install -d $(PREFIX)/bin
	install -m 755 $(TARGET) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)

.PHONY: tar


CWD=$(shell pwd)
VER=$(shell grep -e "\#define[[:space:]]\+VERSION[[:space:]]\+" panel.c | \
		sed -e 's/^[^\"]\+\"//' -e 's/\".*$$//' )


tar: 
	$(MAKE) distclean
	cd ..; \
	if [ -e trayer-$(VER) ]; then \
		echo trayer-$(VER) already exist; \
		echo "won't override";\
		exit 1;\
	else\
		ln -s $(CWD) trayer-$(VER);\
		tar --exclude CVS -hzcvf trayer-$(VER).tgz trayer-$(VER);\
		rm -f trayer-$(VER);\
	fi;

