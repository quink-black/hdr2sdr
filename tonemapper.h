#ifndef HDR2SDR_TONEMAPPER_H
#define HDR2SDR_TONEMAPPER_H

#include "image.h"

namespace quink {

class ToneMapper {
public:
    virtual ~ToneMapper() = default;
    virtual std::shared_ptr<Image<float>> Map(std::shared_ptr<Image<float>> img, bool inPlace = true) = 0;
};

class HableMapper : public ToneMapper {
public:
    std::shared_ptr<Image<float>> Map(std::shared_ptr<Image<float>> img, bool inPlace = true) override;
};

} // namespace quink

#endif
