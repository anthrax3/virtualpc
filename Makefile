CC=gcc
CFLAGS=-rdynamic -g3 -ggdb -Wall -Werror --std=c89 -D_POSIX_C_SOURCE=200112L
LDFLAGS=-rdynamic -g3 -ggdb
LDLIBS=
SRC_DIR=src
OUT_NAME=pc.out
TARGET_DIR=bin
TARGET=$(TARGET_DIR)/$(OUT_NAME)
SOURCES=$(shell find $(SRC_DIR) -name "*.c" -print)
OBJECTS=$(SOURCES:.c=.o)
DEPENDS=$(SOURCES:.c=.d)

.PHONY: clean directories

all:
	mkdir -p $(TARGET_DIR)
	$(MAKE) $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

%.d: %.c
	$(CC) -M $(CFLAGS) $< > $@

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.d' -delete
	rm -rf ./bin

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPENDS)
endif