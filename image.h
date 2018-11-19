#ifndef HDR2SDR_IMAGE_H
#define HDR2SDR_IMAGE_H

#include <memory>

namespace hdr2sdr {

struct Image {
    const int mChannel = 3;
    int mWidth;
    int mHeight;
    std::unique_ptr<float[]> mData;

    Image() : mWidth(0), mHeight(0) { }

    Image(int width, int height) :
        mWidth(width), mHeight(height), mData(new float[width * height * mChannel]) { }

};


} // namespace hdr2sdr

#endif
