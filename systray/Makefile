# Part 0
# load common stuff
TOPDIR = ..
include $(TOPDIR)/Makefile.common
#$(warning INCS=$(INCS))

INCS += -I../
SRC := egg-marshal.c   eggtraymanager.c  fixedtip.c main.c  
OBJ := $(SRC:%.c=%.o)

TARGET = systray.o

all:$(TARGET)
$(TARGET): $(OBJ) 
	$(LD) -r $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $<


clean:
	$(RM)  $(OBJ) $(TARGET) *~

