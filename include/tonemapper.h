#ifndef HDR2SDR_TONEMAPPER_H
#define HDR2SDR_TONEMAPPER_H

#include "image.h"

#include <vector>

namespace quink {

class ToneMapper {
public:
    virtual ~ToneMapper() = default;
    virtual std::shared_ptr<Image<float>> Map(std::shared_ptr<Image<float>> img, bool inPlace = true) = 0;
};

class HableMapper : public ToneMapper {
public:
    std::shared_ptr<Image<float>> Map(std::shared_ptr<Image<float>> img, bool inPlace = true) override;

private:
    struct Table {
        Table();
        float Map(float f) const;

        const float mPrecise = 0.001;
        const float mUpper = 10.0;
        const int mTableSize = mUpper / mPrecise;
        std::vector<float> mData;
    };

    static const Table &GetTable();
    static std::shared_ptr<Image<float>> MapInternal(std::shared_ptr<Image<float>> img);
};

} // namespace quink

#endif
