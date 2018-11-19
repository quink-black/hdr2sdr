#include "image_encoder.h"

#include <limits>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace hdr2sdr {

int PngEncoder::Encode(const std::string &file, std::shared_ptr<Image<float>> imgIn) {
    std::shared_ptr<Image<uint8_t>> img = std::make_shared<Image<uint8_t>>(imgIn->mWidth, imgIn->mHeight);;

    const int lowerLimit = 0;
    const int upperLimit = 255;
    const size_t n = imgIn->mWidth * imgIn->mHeight * imgIn->mChannel;
    for (size_t i = 0; i < n; i++) {
        int tmp = imgIn->mData[i] * upperLimit;
        img->mData[i] = std::min(std::max(tmp, lowerLimit), upperLimit);
    }
    return Encode(file, img);
}

int PngEncoder::Encode(const std::string &file, std::shared_ptr<Image<uint8_t>> img) {
    int ret = stbi_write_png(file.c_str(), img->mWidth, img->mHeight, img->mChannel, img->mData.get(), img->mWidth * img->mChannel);
    return ret;
}

} // namespace hdr2sdr
