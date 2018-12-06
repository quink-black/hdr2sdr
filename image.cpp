#include "image.h"

namespace quink {

ImageWrapper::ImageDataType ImageWrapper::GetDataType() {
    if (mImgFloat)
        return DataTypeFloat;
    else if (mImgUInt8)
        return DataTypeUInt8;
    else if (mImgUInt16)
        return DataTypeUInt16;
    else
        return DataTypeFloat;
}

template <>
std::shared_ptr<Image<float>> ImageWrapper::GetImg() {
    std::shared_ptr<Image<float>> img;
    if (mImgFloat) {
        img = mImgFloat;
    } else if (mImgUInt8) {
        img = std::make_shared<Image<float>>(mImgUInt8->mWidth, mImgUInt8->mHeight, mImgUInt8->mGamma);
        int n = img->DataLength();
        for (int i = 0; i < n; i++)
            img->mData[i] = mImgUInt8->mData[i] / 255.0f;
    } else if (mImgUInt16) {
        img = std::make_shared<Image<float>>(mImgUInt16->mWidth, mImgUInt16->mHeight, mImgUInt16->mHeight);
        int n = img->DataLength();
        for (int i = 0; i < n; i++)
            img->mData[i] = mImgUInt16->mData[i] / 255.0f;
    }

    return img;
}

template <>
std::shared_ptr<Image<uint8_t>> ImageWrapper::GetImg() {
    std::shared_ptr<Image<uint8_t>> img;
    if (mImgFloat) {
        img = std::make_shared<Image<uint8_t>>(mImgFloat->mWidth, mImgFloat->mHeight, mImgFloat->mGamma);
        int n = img->DataLength();
        for (int i = 0; i < n; i++)
            img->mData[i] = std::min<float>(mImgFloat->mData[i] * 255, 255);
    } else if (mImgUInt8) {
        img = mImgUInt8;
    } else if (mImgUInt16) {
        img = std::make_shared<Image<uint8_t>>(mImgUInt16->mWidth, mImgUInt16->mHeight, mImgUInt16->mGamma);
        int n = img->DataLength();
        for (int i = 0; i < n; i++)
            img->mData[i] = std::min<uint16_t>(mImgUInt16->mData[i], 255);
    }

    return img;
}

template <>
std::shared_ptr<Image<uint16_t>> ImageWrapper::GetImg() {
    std::shared_ptr<Image<uint16_t>> img;
    if (mImgFloat) {
        img = std::make_shared<Image<uint16_t>>(mImgFloat->mWidth, mImgFloat->mHeight, mImgFloat->mGamma);
        int n = img->DataLength();
        for (int i = 0; i < n; i++)
            img->mData[i] = std::min<float>(mImgFloat->mData[i] * 255, 65535);
    } else if (mImgUInt8) {
        img = std::make_shared<Image<uint16_t>>(mImgUInt8->mWidth, mImgUInt8->mHeight, mImgUInt8->mGamma);
        int n = img->DataLength();
        for (int i = 0; i < n; i++)
            img->mData[i] = mImgUInt8->mData[i];
    } else if (mImgUInt16) {
        img = mImgUInt16;
    }

    return img;
}

}
