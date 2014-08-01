
OS ?= $(shell uname)
CXX ?= g++
BIN ?= stordb

#PREFIX ?= /usr/local
PREFIX = $(shell pwd)

MAIN = src/main.cc
SRC = $(filter-out $(MAIN), $(wildcard src/*.cc))
SRC += $(wildcard src/modules/*.cc)
OBJS = $(SRC:.cc=.o)

DEPS = $(wildcard deps/*/*.c)
DOBJS = $(DEPS:.c=.o)

LIBV8_BASE ?= $(shell find v8/out/native/ -name 'libv8_base.*.a' | head -1)
LIBV8_SNAPSHOT ?= $(shell find v8/out/native/ -name 'libv8_snapshot.a' | head -1)
LIBV8 = $(LIBV8_BASE) $(LIBV8_SNAPSHOT)
LDB ?= leveldb/libleveldb.a

STORDB_JS_PATH ?= $(PREFIX)/lib/stordb

CXXFLAGS += -std=gnu++11
CXXFLAGS += -Ideps -Iinclude -Iv8/include -Ileveldb/include -lsnappy -lpthread
CXXFLAGS += -DSTORDB_JS_PATH='"$(STORDB_JS_PATH)"'

ifeq ($(OS), Darwin)
	CXXFLAGS += -stdlib=libstdc++
else
	CXXFLAGS += -lrt
endif

.PHONY: $(BIN)
$(BIN): $(DOBJS) $(OBJS)
	$(CXX) $(OBJS) $(DOBJS) $(LIBV8) $(LDB) $(MAIN) $(CXXFLAGS) -o $(BIN)

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
	@make dependencies -C $(@)
	@make i18nsupport=off native -C $(@)

clean:
	@echo "  MAKE clean v8"
	@make clean -C v8
	@echo "  MAKE clean leveldb"
	@make clean -C leveldb
	@echo "  MAKE clean v8"
	@make clean -C v8
	@echo "  RM $(BIN)"
	@rm -f $(BIN)

