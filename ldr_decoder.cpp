#include "ldr_decoder.h"

#include "log.h"
#include "stb_image.h"

namespace quink {

int LdrDecoder::Creator::Probe(const std::string &file) const {
    int x, y, comp;
    if (stbi_info(file.c_str(), &x, &y, &comp))
        return SCORE_UNSUPPORT + 1;
    return SCORE_UNSUPPORT;
}

std::unique_ptr<ImageDecoder> LdrDecoder::Creator::Create() const {
    return std::unique_ptr<ImageDecoder>(new LdrDecoder());
}

ImageWrapper LdrDecoder::Decode(const std::string &file) {
    ImageWrapper imgWrapper;

    int width = 0, height = 0, comp = 0;
    uint8_t *data = stbi_load(file.c_str(), &width, &height, &comp, 3);
    ALOGD("width %d, height %d, comp %d", width, height, comp);
    if (data == nullptr)
        return imgWrapper;
    if (comp < 3) {
        ALOGE("not support channel < 3");
        free(data);
        return imgWrapper;
    }

    auto img = std::make_shared<Image<uint8_t>>();
    img->mWidth = width;
    img->mHeight = height;
    img->mGamma = 2.2;
    img->mData.reset(data);
    imgWrapper.Reset(img);
    return imgWrapper;
}

}
