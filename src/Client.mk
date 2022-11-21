ifeq ("$(PLATFORM)", "86")
	ARCH=-m32
endif

CC = gcc
CFLAGS += $(ARCH) -ffunction-sections -fPIC -Werror -Wno-format-truncation -Wall -Wextra -O2 -I.
LDFLAGS = -shared
RM = rm -f

TARGET_LIB = libftsclient.so
SRCS_LIB = util.c libftsclient.c
OBJS_LIB = $(SRCS_LIB:.c=.o)

TARGET_BIN = appftsClient

.PHONY: all
all: $(TARGET_LIB) $(TARGET_BIN) 

$(TARGET_LIB): $(OBJS_LIB)
	$(CC) $(ARCH) $(LDFLAGS) -o $@ $^ 

$(TARGET_BIN):
	$(CC) $(CFLAGS) -c -o appftsclient.o appftsclient.c
	$(CC) $(ARCH) -o $@ appftsclient.o -L. -lftsclient

.o:%.c
	$(CC) $(CFLAGS) $< >$@

.PHONY: clean
clean:
	@rm -f *.so $(TARGET_BIN) *.o
