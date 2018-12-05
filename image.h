#ifndef HDR2SDR_IMAGE_H
#define HDR2SDR_IMAGE_H

#include <memory>

namespace hdr2sdr {

template <typename T>
struct Image {
    const int mChannel = 3;
    int mWidth;
    int mHeight;
    std::unique_ptr<T[]> mData;

    Image() : mWidth(0), mHeight(0) { }

    Image(int width, int height) :
        mWidth(width), mHeight(height), mData(new T[width * height * mChannel]) { }

    int DataLength() const {
        return mWidth * mHeight * mChannel;
    }
};

class ImageWrapper {
public:
    enum ImageDataType {
        DataTypeEmpty,
        DataTypeFloat,
        DataTypeUInt8,
        DataTypeUInt16,
    };

    ImageWrapper(std::shared_ptr<Image<float>> img) : mImgFloat(img) { }
    ImageWrapper(std::shared_ptr<Image<uint8_t>> img) : mImgUInt8(img) { }
    ImageWrapper(std::shared_ptr<Image<uint16_t>> img) : mImgUInt16(img) { }

    ImageDataType GetDataType();

    template <typename T>
    std::shared_ptr<Image<T>> GetImg() {
        return nullptr;
    }

private:
    std::shared_ptr<Image<float>> mImgFloat;
    std::shared_ptr<Image<uint8_t>> mImgUInt8;
    std::shared_ptr<Image<uint16_t>> mImgUInt16;
};

template <>
std::shared_ptr<Image<float>> ImageWrapper::GetImg();


template <>
std::shared_ptr<Image<uint8_t>> ImageWrapper::GetImg();

template <>
std::shared_ptr<Image<uint16_t>> ImageWrapper::GetImg();

enum class ImageFormat {
    PngImage,
    JpegImage,
};

} // namespace hdr2sdr

#endif
