#ifndef HDR2SDR_IMAGE_DECODER_H
#define HDR2SDR_IMAGE_DECODER_H

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
    };

    virtual ~ImageDecoder() = default;
    virtual std::unique_ptr<Image<float>> Decode(const std::string &file) = 0;
};

class ImageLoader {
public:
    ImageLoader() = delete;
    ~ImageLoader() = delete;

    static std::unique_ptr<Image<float>> LoadImage(const std::string &file);

private:
    using DecoderCreatorListType =
        std::vector<std::shared_ptr<ImageDecoder::DecoderCreator>>;
    static const DecoderCreatorListType &GetDecoderCreatorList();
};

} // namespace quink

#endif
