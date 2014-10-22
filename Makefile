AR = ar
CC = gcc

BUILD  = build
LIB    = lib
SRC    = src
TEST   = test
CFLAGS = -Wall -Werror -std=c99 -pedantic -fpic -c -I include
LFLAGS = -lm

SOURCES=$(shell find $(SRC)/ -type f -iname '*.c')
OBJECTS=$(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(SOURCES))

.PHONY: debug
debug: CFLAGS     += -g
debug: TARGET_DIR  = debug

.PHONY: release
release: CFLAGS     += -DNDEBUG
release: TARGET_DIR  = release

test: debug
	$(CC) $(CFLAGS) -o build/test.o $(TEST)/test.c
	$(CC) $(BUILD)/*.o \
	      $(BUILD)/**/*.o \
	      -o $(BUILD)/test $(LFLAGS)

debug:   lib
release: lib

$(BUILD)/%.o: $(SRC)/%.c
	@if [ ! -d $(shell dirname $@) ]; then mkdir -p $(shell dirname $@); fi

	$(CC) $(CFLAGS) $< -o $@

lib: $(OBJECTS)
	@if [ ! -d $(LIB)/$(TARGET_DIR) ]; then mkdir -p $(LIB)/$(TARGET_DIR); fi

	@if [ "$(shell uname)" != "Darwin" ]; then \
		$(CC) -shared \
		      $(BUILD)/*.o \
		      $(BUILD)/**/*.o \
		      -o $(LIB)/$(TARGET_DIR)/libcodebox.so \
		      $(LFLAGS); \
	else \
		$(CC) -dynamiclib \
		      $(BUILD)/*.o \
		      $(BUILD)/**/*.o \
		      -o $(LIB)/$(TARGET_DIR)/libcodebox.dylib \
		      $(LFLAGS); \
	fi

	$(AR) rcs $(LIB)/$(TARGET_DIR)/libcodebox.a \
		      $(BUILD)/*.o \
		      $(BUILD)/**/*.o

clean:
	@rm -rf $(BUILD)