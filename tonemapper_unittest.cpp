#include "tonemapper.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "image_decoder.h"
#include "image_encoder.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s file.pfm\n", argv[0]);
        return 1;
    }

    std::string file(argv[1]);
    auto imgIn = hdr2sdr::ImageLoader::LoadImage(file);
    if (imgIn == nullptr) {
        fprintf(stderr, "load %s failed\n", file.c_str());
        return 1;
    } else {
        fprintf(stdout, "load %s success\n", file.c_str());
    }

    hdr2sdr::HableMapper tonemapper;
    auto img = tonemapper.Map(std::move(imgIn));

    size_t n = img->mWidth * img->mHeight * img->mChannel;
    const double gamma = 1.0 / 2.2;
    for (size_t i = 0; i < n; i++) {
        img->mData[i] = pow(img->mData[i], gamma);
    }
    hdr2sdr::ImageStore::StoreImage("tonemap.png", hdr2sdr::ImageFormat::PngImage, std::move(img));

    return 0;
}
