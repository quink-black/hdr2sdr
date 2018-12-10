#pragma once

#include <memory>
#include <string>
#include <vector>

#include "image.h"

namespace quink {

class ImageDecoder {
public:
    virtual ~ImageDecoder() = default;
    virtual ImageWrapper Decode(const std::string &file) = 0;
};

class ImageDecoderFactory {
public:
    virtual ~ImageDecoderFactory() = default;

    static const int SCORE_UNSUPPORT = 0;
    static const int SCORE_DEFINITELY = 100;
    // return score, higher is better
    virtual int Probe(const std::string &file) const = 0;
    virtual std::unique_ptr<ImageDecoder> Create() const = 0;
    virtual std::string GetDecoderName() const = 0;
};


class ImageLoader {
public:
    ImageLoader() = delete;
    ~ImageLoader() = delete;

    static ImageWrapper LoadImage(const std::string &file);

private:
    using ImageDecoderFactoryListType =
        std::vector<std::shared_ptr<ImageDecoderFactory>>;
    static const ImageDecoderFactoryListType &GetImageDecoderFactoryList();
};

} // namespace quink
