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

    if (1) {
        auto img = imgWrapper.GetImg<uint8_t>();
        img->GammaCorrect(2.2f);
        auto encs = quink::ImageStore::GetEncoders();
        for (size_t i = 0; i < encs.size(); i++) {
            std::string name = std::to_string(i) + encs[i]->GetDefaultSuffix();
            printf("Encode %s...\n", name.c_str());
            encs[i]->EncodeUInt8(name, img);
            printf("Encode %s finished\n", name.c_str());
        }
    }

    if (0) {
        auto img = imgWrapper.GetImg<uint8_t>();
        img->GammaCorrect(2.2f);
        quink::JpegEncoder jpg;
        for (int i = 20; i <= 100; i += 20) {
            std::string name = std::string("jpeg-") + std::to_string(i) + jpg.GetDefaultSuffix();
            jpg.SetInt("quality", i);

            printf("Encode %s...\n", name.c_str());
            jpg.EncodeUInt8(name, img);
            printf("Encode %s finished\n", name.c_str());
        }
    }

    if (1) {
        auto img = imgWrapper.GetImg<float>();
        std::shared_ptr<quink::ImageEncoder> jpgEnc(new quink::JpegEncoder(90));
        quink::PairEncoder enc(jpgEnc);
        printf("Encode image pair...\n");
        enc.EncodeFloat("pair", img);
        printf("Encode image pair finished\n");
    }

    return 0;
}

