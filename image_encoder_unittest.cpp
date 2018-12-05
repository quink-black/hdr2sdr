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
    std::shared_ptr<quink::Image<float>> img = quink::ImageLoader::LoadImage(file);
    if (img == nullptr) {
        fprintf(stderr, "load %s failed\n", file.c_str());
        return 1;
    } else {
        fprintf(stdout, "load %s success\n", file.c_str());
    }

    size_t n = img->mWidth * img->mHeight * img->mChannel;
    const double gamma = 1.0 / 2.2;
    for (size_t i = 0; i < n; i++) {
        img->mData[i] = pow(img->mData[i], gamma);
    }

    if (1) {
        auto encs = quink::ImageStore::GetEncoders();
        for (size_t i = 0; i < encs.size(); i++) {
            std::string name = std::to_string(i) + encs[i]->GetDefaultSuffix();
            printf("Encode %s...\n", name.c_str());
            encs[i]->EncodeFloat(name, img);
            printf("Encode %s finished\n", name.c_str());
        }
    }

    if (0) {
        quink::JpegEncoder jpg;
        for (int i = 20; i <= 100; i += 20) {
            std::string name = std::string("jpeg-") + std::to_string(i) + jpg.GetDefaultSuffix();
            jpg.SetInt("quality", i);

            printf("Encode %s...\n", name.c_str());
            jpg.EncodeFloat(name, img);
            printf("Encode %s finished\n", name.c_str());
        }
    }

    if (1) {
        std::shared_ptr<quink::ImageEncoder> jpgEnc(new quink::JpegEncoder(90));
        quink::PairEncoder enc(jpgEnc);
        printf("Encode image pair...\n");
        enc.EncodeFloat("pair", img);
        printf("Encode image pair finished\n");
    }

    return 0;
}

