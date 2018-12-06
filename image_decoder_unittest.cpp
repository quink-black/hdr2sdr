#include "image_decoder.h"
#include "image_encoder.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s file.pfm\n", argv[0]);
        return 1;
    }

    std::string file(argv[1]);
    auto imgWrapper = quink::ImageLoader::LoadImage(file);
    if (imgWrapper.Empty()) {
        fprintf(stderr, "load %s failed\n", file.c_str());
        return 1;
    } else {
        fprintf(stdout, "load %s success\n", file.c_str());
    }

    {
        std::string name("decode-float.png");
        printf("store as %s\n", name.c_str());

        auto img = imgWrapper.GetImg<float>();
        img->GammaCorrect(2.2f);
        quink::ImageStore::StoreImage(name, quink::ImageFormat::PngImage, img);
    }

    {
        std::string name = "decode-uint8_t.png";
        printf("store as %s\n", name.c_str());

        auto img = imgWrapper.GetImg<uint8_t>();
        img->GammaCorrect(2.2f);
        quink::ImageStore::StoreImage(name, quink::ImageFormat::PngImage, img);
    }

    return 0;
}
