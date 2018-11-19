#ifndef HDR2SDR_IMAGE_ENCODER_H
#define HDR2SDR_IMAGE_ENCODER_H

#include "image.h"

namespace hdr2sdr {

class ImageEncoder {
public:
    virtual ~ImageEncoder() = default;
    virtual int Encode(const std::string &file, std::shared_ptr<Image<float>> img) { return -1; }
    virtual int Encode(const std::string &file, std::shared_ptr<Image<uint8_t>> img) { return -1; }
};

class PngEncoder : public ImageEncoder {
public:
    int Encode(const std::string &file, std::shared_ptr<Image<float>> img) override;
    int Encode(const std::string &file, std::shared_ptr<Image<uint8_t>> img) override;
};

class ImageStore {
public:
    ImageStore() = delete;
    ~ImageStore() = delete;

    template <typename T>
    static int StoreImage(const std::string &file, ImageFormat format, std::shared_ptr<Image<T>> img) {
        switch (format)
        {
        case ImageFormat::PngImage:
            {
                PngEncoder enc;
                return enc.Encode(file, img);
            }
        case ImageFormat::JpegImage:
            break;
        }

        return -1;
    }
};

} // namespace hdr2sdr

#endif
