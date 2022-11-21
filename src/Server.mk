ifeq ("$(PLATFORM)", "86")
	ARCH=-m32
endif

CC = gcc
CFLAGS += $(ARCH) -ffunction-sections -fPIC -Werror -Wno-format-truncation -Wall -Wextra -O2 -I.
LDFLAGS = -shared
RM = rm -f

TARGET_LIB = libftsserver.so
SRCS_LIB = util.c libftsserver.c
OBJS_LIB = $(SRCS_LIB:.c=.o)

TARGET_BIN = appftsServer

.PHONY: all
all: $(TARGET_LIB) $(TARGET_BIN) 

$(TARGET_LIB): $(OBJS_LIB)
	$(CC) $(ARCH) $(LDFLAGS) -o $@ $^ 

$(TARGET_BIN):
	$(CC) $(CFLAGS) -c -o appftsserver.o appftsserver.c
	$(CC) $(ARCH) -o $@ appftsserver.o -L. -lftsserver

.o:%.c
	$(CC) $(CFLAGS) $< >$@

.PHONY: clean
clean:
	@rm -f *.so $(TARGET_BIN) *.o
