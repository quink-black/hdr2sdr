all: build
	ninja -C $<

build:
	meson $@
