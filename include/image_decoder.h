#pragma once

#include <memory>
#include <string>
#include <vector>

#include "image.h"

namespace quink {

class ImageDecoder {
public:
    class DecoderCreator {
    public:
        virtual ~DecoderCreator() = default;

        static const int SCORE_UNSUPPORT = 0;
        static const int SCORE_DEFINITELY = 100;
        // return score, higher is better
        virtual int Probe(const std::string &file) const = 0;
        virtual std::unique_ptr<ImageDecoder> Create() const = 0;
        virtual std::string GetDecoderName() const = 0;
    };

    virtual ~ImageDecoder() = default;
    virtual ImageWrapper Decode(const std::string &file) = 0;
};

class ImageLoader {
public:
    ImageLoader() = delete;
    ~ImageLoader() = delete;

    static ImageWrapper LoadImage(const std::string &file);

private:
    using DecoderCreatorListType =
        std::vector<std::shared_ptr<ImageDecoder::DecoderCreator>>;
    static const DecoderCreatorListType &GetDecoderCreatorList();
};

} // namespace quink
