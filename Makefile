
OS ?= $(shell uname)
CXX ?= g++
CWD = $(shell pwd)
BIN ?= stordb
OUT ?= out
V8ARCH ?= x64

PREFIX ?= /usr/local

MAIN = src/main.cc
SRC = $(filter-out $(MAIN), $(wildcard src/*.cc))
SRC += $(wildcard src/modules/*.cc)
OBJS = $(SRC:.cc=.o)

DEPS = $(wildcard deps/*/*.c)
DOBJS = $(DEPS:.c=.o)

#LIBV8 ?= $(wildcard $(CWD)/v8/out/native/lib*.a)
LIBV8 = v8/out/native/libv8_base.a \
				v8/out/native/libv8_libbase.a \
				v8/out/native/libv8_snapshot.a \

STORDB_JS_PATH ?= $(PREFIX)/lib/stordb

TARGET_NAME = libstordb
TARGET_STATIC = $(TARGET_NAME).a

CORO_FLAGS += -DCORO_PTHREAD

CFLAGS += $(CORO_FLAGS) -std=c99 -Ideps

CXXFLAGS += -std=gnu++11
CXXFLAGS += -Ideps -Iinclude -Iv8/include
CXXFLAGS += -DSTORDB_JS_PATH='"$(STORDB_JS_PATH)"'
CXXFLAGS += $(CORO_FLAGS)

ifeq ($(OS), Darwin)
	CXXFLAGS += -stdlib=libstdc++
else
	CXXFLAGS += -lrt
endif

.PHONY: $(BIN)
$(BIN): out $(TARGET_STATIC)
	@echo "  CXX($(BIN))"
	@$(CXX) $(OUT)/$(TARGET_STATIC) $(LIBV8) $(LDB) $(MAIN) $(CXXFLAGS) -o $(OUT)/$(BIN)
	@echo "  OUT($(OUT)/$(BIN))"

.PHONY: $(TARGET_STATIC)
$(TARGET_STATIC): out $(OBJS) $(DOBJS)
	@echo "  AR($(TARGET_STATIC))"
	@ar crus $(OUT)/$(TARGET_STATIC) $(OBJS) $(DOBJS)
	@echo "  OUT($(OUT)/$(TARGET_STATIC))"

out:
	@mkdir -p out

.PHONY: $(OBJS)
$(OBJS):
	@echo "  CXX($(@))"
	@$(CXX) $(CXXFLAGS) -c $(@:.o=.cc) -o $(@)

$(DOBJS):
	@echo "  CC($(@))"
	$(CC) $(CFLAGS) -c $(@:.o=.c) -o $(@)

install:
	@# stordb
	rm -rf $(PREFIX)/include/stordb
	rm -rf $(PREFIX)/lib/stordb
	mkdir $(PREFIX)/include/stordb
	mkdir $(PREFIX)/lib/stordb
	install $(OUT)/$(TARGET_STATIC) $(PREFIX)/lib
	install $(OUT)/$(BIN) $(PREFIX)/bin
	install include/stordb.h $(PREFIX)/include
	cp include/stordb/*.h $(PREFIX)/include/stordb
	cp -rf lib/stordb/* $(PREFIX)/lib/stordb
	@# v8
	cp -f v8/include/*.h $(PREFIX)/include
	cp -f v8/out/native/*.a $(PREFIX)/lib

link:
	@# v8
	$(foreach lib, $(LIBV8), $(shell ln -sf $(CWD)/$(lib) $(PREFIX)/lib/$(shell basename $(lib:.$(V8ARCH).a=.a))))
	@# stordb
	ln -fs $(CWD)/include/stordb $(PREFIX)/include
	ln -fs $(CWD)/lib/stordb $(PREFIX)/lib
	ln -fs $(CWD)/include/stordb.h $(PREFIX)/include/stordb.h
	ln -fs $(CWD)/$(TARGET_STATIC) $(PREFIX)/lib
	ln -fs $(CWD)/$(BIN) $(PREFIX)/bin

dependencies: v8

.PHONY: v8

v8:
	@echo "  MAKE $(@)"
	@#make dependencies -C $(@)
	@#make builddeps -C $(@)
	@make i18nsupport=off native -C $(@)

clean:
	rm -f $(BIN)
	rm -f $(TARGET_STATIC)

