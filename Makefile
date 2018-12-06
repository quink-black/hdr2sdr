all: build
	ninja -C $< -j6

build:
	PKG_CONFIG_PATH=third-party/install/lib/pkgconfig meson $@
