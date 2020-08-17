BUILD_BIN = BIN
BUILD_LIB = LIB
# BUILD_OPT = $(BUILD_LIB)
CFLAGS ?= -g -Wall
CFLAGS += -std=gnu99 -I.
CPPFLAGS += -D_GNU_SOURCE -D__CHECK_ENDIAN__
RM = rm -f

DNVME_BIN = dnvme
DNVME_LIB = dnvme.so

ifeq ($(BUILD_OPT),$(BUILD_LIB))
CFLAGS += -fPIC
SHARED = -shared
DNVME = $(DNVME_LIB)
else
DNVME = $(DNVME_BIN)
endif

AUTHOR=xingzou <sweet.zou@hotmail.com>

all: $(DNVME)

OBJS := dnvme_ioctrl.o dnvme_commands.o dnvme_show.o

ifeq ($(BUILD_OPT),$(BUILD_BIN))
    OBJS += main.o
endif

$(DNVME): $(OBJS)
 ifeq ($(BUILD_OPT),$(BUILD_LIB))
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SHARED) -o $@ $^
else
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^
endif

%.o: %.c %.h inc/dnvme_interface.h inc/dnvme_ioctl.h dnvme.h dnvme_ioctrl.h dnvme_commands.h dnvme_show.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) -rf $(DNVME) $(OBJS)

.PHONY: all clean


