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

    std::string name("decode-float.png");
    printf("store as %s\n", name.c_str());
    quink::ImageStore::StoreImage(name, quink::ImageFormat::PngImage, img);

    quink::ImageWrapper imgWrapper(img);
    printf("data type %d\n", imgWrapper.GetDataType());

    name = "decode-uint8_t.png";
    printf("store as %s\n", name.c_str());
    auto img2 = imgWrapper.GetImg<uint8_t>();
    quink::ImageStore::StoreImage(name, quink::ImageFormat::PngImage, img2);

    return 0;
}
