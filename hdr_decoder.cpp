#include "hdr_decoder.h"

#include <algorithm>

#include "log.h"
#include "stb_image.h"

namespace quink {

int HdrDecoder::Creator::Probe(const std::string &file) const {
    auto suffix_pos = file.rfind(".");
    if (suffix_pos != std::string::npos) {
        std::string suffix = file.substr(suffix_pos);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(), tolower);
        if (suffix == ".hdr")
            return SCORE_DEFINITELY;
    }
    return SCORE_UNSUPPORT;
}

std::unique_ptr<ImageDecoder> HdrDecoder::Creator::Create() const {
    return std::unique_ptr<ImageDecoder>(new HdrDecoder());
}

ImageWrapper HdrDecoder::Decode(const std::string &file) {
    ImageWrapper imgWrapper;

    if (!stbi_is_hdr(file.c_str())) {
        ALOGD("%s is not HDR?", file.c_str());
        return imgWrapper;
    }

    int width = 0, height = 0, comp = 0;
    float *data = stbi_loadf(file.c_str(), &width, &height, &comp, 0);
    ALOGD("width %d, height %d, comp %d", width, height, comp);
    if (data == nullptr)
        return imgWrapper;
    if (comp != 3) {
        ALOGE("not support channel != 3");
        free(data);
        return imgWrapper;
    }

    auto img = std::make_shared<Image<float>>();
    img->mWidth = width;
    img->mHeight = height;
    img->mData.reset(data);
    imgWrapper.Reset(img);
    return imgWrapper;
}

}
