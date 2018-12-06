#pragma once

#include "image_decoder.h"

namespace quink {

class OpenEXRDecoder : public ImageDecoder {
public:
    class Creator : public DecoderCreator {
    public:
        int Probe(const std::string &file) const override;
        std::unique_ptr<ImageDecoder> Create() const override;
        std::string GetDecoderName() const override { return "OpenEXRDecoder"; }
    };
    ImageWrapper Decode(const std::string &file) override;
};

}
