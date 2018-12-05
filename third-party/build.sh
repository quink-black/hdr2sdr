#!/bin/sh

currect_dir=$PWD
mkdir -p build/libjpeg-turbo \
    && cd build/libjpeg-turbo \
    && cmake ../../project/libjpeg-turbo \
        -DCMAKE_INSTALL_PREFIX=${currect_dir}/install \
    && make -j6 install
