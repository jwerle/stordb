
OS ?= $(shell uname)
CXX ?= g++
CWD = $(shell pwd)
BIN ?= stordb
V8ARCH ?= x64

PREFIX ?= /usr/local
#PREFIX ?= $(CWD)

MAIN = src/main.cc
SRC = $(filter-out $(MAIN), $(wildcard src/*.cc))
SRC += $(wildcard src/modules/*.cc)
OBJS = $(SRC:.cc=.o)

DEPS = $(wildcard deps/*/*.c)
DOBJS = $(DEPS:.c=.o)

LIBV8 ?= $(wildcard $(CWD)/v8/out/native/lib*.a)

STORDB_JS_PATH ?= $(PREFIX)/lib/stordb

TARGET_NAME = libstordb
TARGET_STATIC = $(TARGET_NAME).a

CXXFLAGS += -std=gnu++11
CXXFLAGS += -Ideps -Iinclude -Iv8/include
CXXFLAGS += -DSTORDB_JS_PATH='"$(STORDB_JS_PATH)"'

ifeq ($(OS), Darwin)
	CXXFLAGS += -stdlib=libstdc++
else
	CXXFLAGS += -lrt
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
	@# stordb
	mkdir $(PREFIX)/include/stordb
	mkdir $(PREFIX)/lib/stordb
	install $(TARGET_STATIC) $(PREFIX)/lib
	install $(BIN) $(PREFIX)/bin
	install include/stordb.h $(PREFIX)/include
	cp include/stordb/*.h $(PREFIX)/include/stordb
	@# v8
	cp v8/out/native/*.a $(PREFIX)/lib
	cp v8/include/*.h $(PREFIX)/include

link:
	@# stordb
	ln -fs $(CWD)/include/stordb $(PREFIX)/include
	ln -fs $(CWD)/lib/stordb $(PREFIX)/lib
	ln -fs $(CWD)/include/stordb.h $(PREFIX)/include/stordb.h
	ln -fs $(CWD)/$(TARGET_STATIC) $(PREFIX)/lib
	ln -fs $(CWD)/$(BIN) $(PREFIX)/bin
	@# v8
	$(foreach lib, $(LIBV8), $(shell ln -sf $(lib) $(PREFIX)/lib/$(shell basename $(lib:.$(V8ARCH).a=.a))))

dependencies: v8

.PHONY: v8

v8:
	@echo "  MAKE $(@)"
	@make dependencies -C $(@)
	@#make i18nsupport=off $(V8ARCH).release -C $(@)
	@make i18nsupport=off native -C $(@)

clean:
	@echo "  MAKE clean v8"
	@make clean -C v8
	@echo "  MAKE clean v8"
	@make clean -C v8
	@echo "  RM $(BIN)"
	@rm -f $(BIN)

