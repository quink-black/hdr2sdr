CXXFLAGS := -g -O0 -std=c++11 -I.
#CXXFLAGS += -fsanitize=address
LDFLAGS :=

BUILD_DIR := build
OBJ := $(BUILD_DIR)/image.o \
	$(BUILD_DIR)/image_unittest.o \

HEADER := image.h stb_image.h stb_image_write.h

all: build build/.sum image_unittest

build:
	mkdir $@

build/.sum: $(HEADER) Makefile
	touch *.cpp
	touch $@

image_unittest: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDFLAGS)

%.o: ../%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f main
	rm -rf build

.PHONY: all clean
