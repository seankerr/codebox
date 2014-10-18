AR=ar
CC=gcc

BUILD=build
LIB=lib
SRC=src
TEST=test
CFLAGS=-Wall -Werror -std=c99 -pedantic -fpic -c -I include
LFLAGS=

SOURCE=$(shell find $(SRC) -type f -iname '*.c')

DEBUG_OBJ=$(patsubst %.c, %.debug.o, $(SOURCE))
RELEASE_OBJ=$(patsubst %.c, %.release.o, $(SOURCE))

BUILD_DEBUG_OBJ=$(patsubst $(SRC)/%, $(BUILD)/%, $(DEBUG_OBJ))
BUILD_RELEASE_OBJ=$(patsubst $(SRC)/%, $(BUILD)/%, $(RELEASE_OBJ))

test: debug
	$(CC) $(CFLAGS) -o build/test.o $(TEST)/test.c
	$(CC) $(BUILD)/*.debug.o $(BUILD)/**/*.debug.o $(BUILD)/test.o -o $(BUILD)/test $(LFLAGS)

debug: $(BUILD_DEBUG_OBJ)
	@if [ ! -d $(LIB)/debug ]; then mkdir -p $(LIB)/debug; fi

	@if [ "$(shell uname)" != "Darwin" ]; then \
		$(CC) -shared \
		      $(BUILD)/*.debug.o \
		      $(BUILD)/**/*.debug.o \
		      -o $(LIB)/debug/libcodebox.so \
		      $(LFLAGS); \
	else \
		$(CC) -dynamiclib \
		      $(BUILD)/*.debug.o \
		      $(BUILD)/**/*.debug.o \
		      -o $(LIB)/debug/libcodebox.dylib \
		      $(LFLAGS); \
	fi

	$(AR) rcs $(LIB)/debug/libcodebox.a \
		$(BUILD)/*.debug.o \
		$(BUILD)/**/*.debug.o

release: $(BUILD_RELEASE_OBJ)
	@if [ ! -d $(LIB)/release ]; then mkdir -p $(LIB)/release; fi

	@if [ "$(shell uname)" != "Darwin" ]; then \
		$(CC) -shared \
		      $(BUILD)/*.release.o \
		      $(BUILD)/**/*.release.o \
		      -o $(LIB)/release/libcodebox.so \
		      $(LFLAGS); \
	else \
		$(CC) -dynamiclib \
		      $(BUILD)/*.release.o \
		      $(BUILD)/**/*.release.o \
		      -o $(LIB)/release/libcodebox.dylib \
		      $(LFLAGS); \
	fi

	$(AR) rcs $(LIB)/release/libcodebox.a \
		$(BUILD)/*.release.o \
		$(BUILD)/**/*.release.o

$(BUILD_DEBUG_OBJ):
	$(shell mkdir -p `dirname $@` >/dev/null)

	$(CC) $(CFLAGS) -o $@ $(shell echo $@ | sed -e 's/\.debug\.o/.c/g' | sed -e 's/$(BUILD)\//$(SRC)\//g')

$(BUILD_RELEASE_OBJ):
	$(shell mkdir -p `dirname $@` >/dev/null)

	$(CC) $(CFLAGS) -DNDEBUG -o $@ $(shell echo $@ | sed -e 's/\.release\.o/.c/g' | sed -e 's/$(BUILD)\//$(SRC)\//g')

clean:
	@rm -rf $(BUILD) $(LIB)