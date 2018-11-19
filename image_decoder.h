#ifndef HDR2SDR_IMAGE_DECODER_H
#define HDR2SDR_IMAGE_DECODER_H

#include <memory>
#include <string>
#include <vector>

#include "image.h"

namespace hdr2sdr {

class ImageDecoder {
public:
    class DecoderBuilder {
    public:
        virtual ~DecoderBuilder() = default;

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

    using DecoderBuilderListType = std::vector<std::shared_ptr<ImageDecoder::DecoderBuilder>>;
    static std::unique_ptr<Image<float>> LoadImage(const std::string &file);

private:
    static const DecoderBuilderListType &GetDecoderBuilderList();
};

} // namespace hdr2sdr

#endif
