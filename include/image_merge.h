#pragma once

#include "image.h"

namespace quink {

class ImageMerge {
public:
    template<typename T>
    static std::shared_ptr<Image<T>> Merge(std::shared_ptr<Image<uint8_t>> imgLow,
            std::shared_ptr<Image<uint8_t>> imgHigh) {
        (void)imgLow;
        (void)imgHigh;
        return nullptr;
    }
};

template <>
std::shared_ptr<Image<float>> ImageMerge::Merge(std::shared_ptr<Image<uint8_t>> imgLow,
        std::shared_ptr<Image<uint8_t>> imgHigh);

template <>
std::shared_ptr<Image<uint16_t>> ImageMerge::Merge(std::shared_ptr<Image<uint8_t>> imgLow,
        std::shared_ptr<Image<uint8_t>> imgHigh);

}
