#include "openexr_decoder.h"

#include <algorithm>

#include "log.h"
#define TINYEXR_IMPLEMENTATION
#include "tinyexr.h"

namespace quink {

static void convert(void *p, int offset, int type, float *dst) {
    switch(type) {
    case TINYEXR_PIXELTYPE_UINT: {
        int32_t *pix = (int32_t *)p;
        *dst = pix[offset];
        break;
    }
    case TINYEXR_PIXELTYPE_HALF:
    case TINYEXR_PIXELTYPE_FLOAT: {
        float *pix = (float *)p;
        *dst = pix[offset];
        break;
    }
    default:
        *dst = 0.0;
        break;
    }
}

int OpenEXRDecoder::Creator::Probe(const std::string &file) const {
    auto suffix_pos = file.rfind(".");
    if (suffix_pos != std::string::npos) {
        std::string suffix = file.substr(suffix_pos);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(), tolower);
        if (suffix == ".exr")
            return SCORE_DEFINITELY;
    }
    return SCORE_UNSUPPORT;
}

std::unique_ptr<ImageDecoder> OpenEXRDecoder::Creator::Create() const {
    return std::unique_ptr<ImageDecoder>(new OpenEXRDecoder());
}

ImageWrapper OpenEXRDecoder::Decode(const std::string &file) {
    ImageWrapper imgWrapper;
    std::shared_ptr<Image<float>> imgOut;

    EXRImage img;
    EXRVersion version;
    EXRHeader header;
    const char *errMsg = nullptr;
    int num_channels;
    int idxR = -1, idxG = -1, idxB = -1;

    InitEXRImage(&img);
    InitEXRHeader(&header);
    ParseEXRVersionFromFile(&version, file.c_str());
    if (ParseEXRHeaderFromFile(&header, &version, file.c_str(), &errMsg)) {
        ALOGE("could not parser exr header: %s", errMsg);
        goto out;
    }

    for (int i = 0; i < header.num_channels; ++i) {
        if (header.requested_pixel_types[i] == TINYEXR_PIXELTYPE_HALF)
            header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;
    }

    if (LoadEXRImageFromFile(&img, &header, file.c_str(), &errMsg)) {
        ALOGE("could not open exr file: %s", errMsg);
        goto out;
    }

    ALOGD("image width %d, height %d", img.width, img.height);

    num_channels = header.num_channels;
    if (num_channels > 3)
        num_channels = 3;
    else if (num_channels < 3) {
        ALOGE("not support %d channels", num_channels);
        goto out;
    }

    imgOut = std::make_shared<Image<float>>(img.width, img.height);
    for (int c = 0; c < header.num_channels; ++c) {
        if (strcmp(header.channels[c].name, "R") == 0)
            idxR = c;
        else if (strcmp(header.channels[c].name, "G") == 0)
            idxG = c;
        else if (strcmp(header.channels[c].name, "B") == 0)
            idxB = c;
        else if (strcmp(header.channels[c].name, "A") == 0)
            ;
        else {
            ALOGE("channel name %s", header.channels[c].name);
            goto out;
        }
    }
    if (idxR == -1 || idxG == -1 || idxB == -1) {
        ALOGE("channel is incomplete");
        goto out;
    }

    for (int i = 0; i < imgOut->mHeight; ++i) {
        for (int j = 0; j < imgOut->mWidth; ++j) {
            int index = i * imgOut->mWidth + j;
            convert(img.images[idxR], index, header.pixel_types[idxR], &imgOut->mData[index * 3 + 0]);
            convert(img.images[idxG], index, header.pixel_types[idxG], &imgOut->mData[index * 3 + 1]);
            convert(img.images[idxB], index, header.pixel_types[idxB], &imgOut->mData[index * 3 + 2]);
        }
    }

    imgWrapper.Reset(imgOut);

out:
    FreeEXRHeader(&header);
    FreeEXRImage(&img);
    return imgWrapper;
}

}
