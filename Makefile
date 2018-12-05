all: build
	ninja -C $<

build:
	PKG_CONFIG_PATH=third-party/install/lib/pkgconfig meson $@
