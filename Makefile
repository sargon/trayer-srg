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
-include $(DEP)
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
