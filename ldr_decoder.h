#pragma once

#include "image_decoder.h"

namespace quink {

class LdrDecoder : public ImageDecoder {
public:
    class Creator : public DecoderCreator {
    public:
        int Probe(const std::string &file) const override;
        std::unique_ptr<ImageDecoder> Create() const override;
        std::string GetDecoderName() const override { return "LdrDecoder"; }
    };

    ImageWrapper Decode(const std::string &file) override;
};

}
