
OS ?= $(shell uname)
CXX ?= g++
BIN ?= stordb
V8ARCH ?= x64

#PREFIX ?= /usr/local
PREFIX ?= $(shell pwd)

MAIN = src/main.cc
SRC = $(filter-out $(MAIN), $(wildcard src/*.cc))
SRC += $(wildcard src/modules/*.cc)
OBJS = $(SRC:.cc=.o)

DEPS = $(wildcard deps/*/*.c)
DOBJS = $(DEPS:.c=.o)

LIBV8 ?= $(wildcard v8/out/$(V8ARCH).release/lib*.a)
LDB ?= leveldb/libleveldb.a

STORDB_JS_PATH ?= $(PREFIX)/lib/stordb

TARGET_NAME = libstordb
TARGET_STATIC = $(TARGET_NAME).a

CXXFLAGS += -std=gnu++11
CXXFLAGS += -Ideps -Iinclude -Iv8/include -Ileveldb/include
CXXFLAGS += -DSTORDB_JS_PATH='"$(STORDB_JS_PATH)"'

ifeq ($(OS), Darwin)
	CXXFLAGS += -stdlib=libstdc++
endif

.PHONY: $(BIN)
$(BIN): $(TARGET_STATIC)
	$(CXX) $(TARGET_STATIC) $(LIBV8) $(LDB) $(MAIN) $(CXXFLAGS) -o $(BIN)

.PHONY: $(TARGET_STATIC)
$(TARGET_STATIC): $(OBJS) $(DOBJS)
	ar crus $(TARGET_STATIC) $(OBJS) $(DOBJS)

.PHONY: $(OBJS)
$(OBJS):
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cc) -o $(@)

$(DOBJS):
	$(CC) -std=c99 -Ideps -c $(@:.o=.c) -o $(@)

install:
	install $(BIN) $(PREFIX)/bin

.PHONY: leveldb v8
dependencies: leveldb v8

leveldb:
	@echo "  MAKE $(@)"
	@CXXFLAGS='$(CXXFLAGS)' make -C $(@) 1>/dev/null

v8:
	@echo "  MAKE $(@)"
	@make $(V8ARCH).release -C $(@)

clean:
	@echo "  MAKE clean v8"
	@make clean -C v8
	@echo "  MAKE clean leveldb"
	@make clean -C leveldb
	@echo "  MAKE clean v8"
	@make clean -C v8
	@echo "  RM $(BIN)"
	@rm -f $(BIN)

