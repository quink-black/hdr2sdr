#include "image_decoder.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s file.pfm\n", argv[0]);
        return 1;
    }

    std::string file(argv[1]);
    auto img = hdr2sdr::ImageLoader::LoadImage(file);
    if (img == nullptr) {
        fprintf(stderr, "load %s failed\n", file.c_str());
        return 1;
    } else {
        fprintf(stdout, "load %s success\n", file.c_str());
    }

    size_t n = img->mWidth * img->mHeight * img->mChannel;
    using dataType = uint8_t;
    std::vector<dataType> data(n);
    int lowerLimit = std::numeric_limits<dataType>::min();
    int upperLimit = std::numeric_limits<dataType>::max();
    const double gamma = 1.0 / 2.2;
    for (size_t i = 0; i < n; i++) {
        int tmp = pow(img->mData[i], gamma) * upperLimit;
        data[i] = std::min(std::max(tmp, lowerLimit), upperLimit);
    }
    stbi_write_png("out.png", img->mWidth, img->mHeight, img->mChannel, data.data(), img->mWidth * img->mChannel);
    return 0;
}
