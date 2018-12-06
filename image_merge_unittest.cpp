#include "tonemapper.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "image_decoder.h"
#include "image_encoder.h"
#include "image_merge.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s low.jpg high.jpg\n", argv[0]);
        return 1;
    }

    auto imgLowWrapper = quink::ImageLoader::LoadImage(argv[1]);
    auto imgHighWrapper = quink::ImageLoader::LoadImage(argv[2]);
    if (imgLowWrapper.Empty() || imgHighWrapper.Empty()) {
        fprintf(stderr, "load files failed\n");
        return 1;
    } else {
        fprintf(stdout, "load files success\n");
    }

    auto imgLow = imgLowWrapper.GetImg<uint8_t>();
    auto imgHigh = imgHighWrapper.GetImg<uint8_t>();
    auto img = quink::ImageMerge::Merge<float>(imgLow, imgHigh);

    quink::HableMapper tonemapper;
    img = tonemapper.Map(img);
    quink::ImageStore::StoreImage("tonemap.jpg", quink::ImageFormat::JpegImage, img);

    return 0;
}
