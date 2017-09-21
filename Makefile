CC=gcc
CFLAGS=-rdynamic -g3 -ggdb -Wall -Werror --std=c89 -D_POSIX_C_SOURCE=200112L
LDFLAGS=-rdynamic -g3 -ggdb
LDLIBS=
SRC_DIR=src
OUT_NAME=pc.out
TARGET_DIR=bin
TARGET=$(TARGET_DIR)/$(OUT_NAME)
SOURCES=$(shell find $(SRC_DIR) -name "*.c" -print)
DEPENDS=$(SOURCES:.c=.d)

TARGETS=pc basm
SOURCES := $(filter-out $(patsubst %,src/%.c,$(TARGETS)),$(SOURCES))
OBJECTS=$(SOURCES:.c=.o)

.PHONY: clean

all:
	mkdir -p $(TARGET_DIR)
	$(MAKE) $(addprefix $(TARGET_DIR)/,$(TARGETS))

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_DIR)/%: $(SRC_DIR)/%.o $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.d' -delete
	rm -rf ./bin
