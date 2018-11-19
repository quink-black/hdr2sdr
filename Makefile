CXXFLAGS := -g -O0 -std=c++11 -I.
CXXFLAGS += -fsanitize=address
LDFLAGS :=

BUILD_DIR := build
OBJ := $(BUILD_DIR)/image_decoder.o \
	$(BUILD_DIR)/tonemapper.o \

HEADER := image.h \
	stb_image.h \
	stb_image_write.h \
	tonemapper.h

BIN := image_decoder_unittest tonemapper_unittest

all: build build/.sum $(BIN)

build:
	mkdir $@

build/.sum: $(HEADER) Makefile
	touch *.cpp
	touch $@

image_decoder_unittest: $(OBJ) $(BUILD_DIR)/image_decoder_unittest.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

tonemapper_unittest: $(OBJ) $(BUILD_DIR)/tonemapper_unittest.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: ../%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f main
	rm -rf build
	rm -f $(BIN)

.PHONY: all clean
