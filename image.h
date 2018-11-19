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

};

enum class ImageFormat {
    PngImage,
    JpegImage,
};

} // namespace hdr2sdr

#endif
