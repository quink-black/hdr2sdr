#include "image_merge.h"
#include "image_encoder.h"

namespace quink {

template <>
std::shared_ptr<Image<float>> ImageMerge::Merge(std::shared_ptr<Image<uint8_t>> imgLow,
        std::shared_ptr<Image<uint8_t>> imgHigh) {
    if (imgLow->mWidth != imgHigh->mWidth ||
            imgLow->mHeight != imgHigh->mHeight ||
            imgLow->mGamma != imgHigh->mGamma)
        return nullptr;

    auto img = std::make_shared<Image<float>>(imgLow->mWidth, imgLow->mHeight, imgLow->mGamma);
    int n = img->DataLength();
    for (int i = 0; i < n; i++)
        img->mData[i] = imgLow->mData[i] / 255.0 + (float)imgHigh->mData[i] / PairEncoder::magic_numer;
    return img;
}

template <>
std::shared_ptr<Image<uint16_t>> ImageMerge::Merge(std::shared_ptr<Image<uint8_t>> imgLow,
        std::shared_ptr<Image<uint8_t>> imgHigh) {
    if (imgLow->mWidth != imgHigh->mWidth ||
            imgLow->mHeight != imgHigh->mHeight ||
            imgLow->mGamma != imgHigh->mGamma)
        return nullptr;

    auto img = std::make_shared<Image<uint16_t>>(imgLow->mWidth, imgLow->mHeight, imgLow->mGamma);
    int n = img->DataLength();
    for (int i = 0; i < n; i++)
        img->mData[i] = imgLow->mData[i] + (imgHigh->mData[i] << 8) / PairEncoder::magic_numer;
    return img;
}

}
