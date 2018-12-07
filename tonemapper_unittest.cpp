#include "tonemapper.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include "image_decoder.h"
#include "image_encoder.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s file.pfm\n", argv[0]);
        return 1;
    }

    std::string file(argv[1]);
    auto imgWrapper = quink::ImageLoader::LoadImage(file);
    auto imgIn = imgWrapper.GetImg<float>();
    if (imgIn == nullptr) {
        fprintf(stderr, "load %s failed\n", file.c_str());
        return 1;
    } else {
        fprintf(stdout, "load %s success\n", file.c_str());
    }

    quink::HableMapper tonemapper;
    auto t1 = std::chrono::high_resolution_clock::now();
    tonemapper.Map(imgIn, false);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto img = tonemapper.Map(imgIn, false);
    auto t3 = std::chrono::high_resolution_clock::now();
    printf("%lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
    printf("%lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count());

    quink::ImageStore::StoreImage("tonemap.png", quink::ImageFormat::PngImage, std::move(img));

    return 0;
}
