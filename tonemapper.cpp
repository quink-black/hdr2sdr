#include "tonemapper.h"

#include <assert.h>

namespace quink {

HableMapper::Table::Table() {
    int n = mTableSize;
    auto img = std::make_shared<Image<float>>(n, 1);
    for (int i = 0; i < n; i++) {
        img->mData[i] = mPrecise * i;
    }
    auto imgOut = MapInternal(img);
    mData.reserve(n);
    for (int i = 0; i < n; i++) {
        mData.push_back(imgOut->mData[i]);
    }
}

float HableMapper::Table::Map(float f) const {
    assert(f >= 0.0);

    int index = f / mPrecise;
    if (index >= mTableSize)
        return mData.back();
    else
        return mData[index];
}

const HableMapper::Table &HableMapper::GetTable() {
    static HableMapper::Table table;
    return table;
}

std::shared_ptr<Image<float>> HableMapper::MapInternal(std::shared_ptr<Image<float>> img) {
    auto hable = [](float in) {
        float a = 0.15f, b = 0.50f, c = 0.10f, d = 0.20f, e = 0.02f, f = 0.30f;
        return (in * (in * a + b * c) + d * e) / (in * (in * a + b) + d * f) - e / f;
    };

    img->GammaCorrect(1.0f);
    std::shared_ptr<Image<float>> out = std::make_shared<Image<float>>(img->mWidth, img->mHeight);
    int n = img->mWidth * img->mHeight * img->mChannel;
    const float exposureBias = 2.0f;
    const float whiteScale = 1.0 / hable(11.2);
    for (int i = 0; i < n; i++) {
        float curr = hable(exposureBias * img->mData[i]);
        float color = curr * whiteScale;
        out->mData[i] = color;
    }
    out->GammaCorrect(2.2f);
    return out;
}

std::shared_ptr<Image<float>> HableMapper::Map(std::shared_ptr<Image<float>> img, bool inPlace) {
    if (img == nullptr)
        return nullptr;

    img->GammaCorrect(1.0f);
    std::shared_ptr<Image<float>> out;
    if (inPlace)
        out = img;
    else
        out = std::make_shared<Image<float>>(img->mWidth, img->mHeight);

    const Table &table = GetTable();
    int n = img->DataLength();
    for (int i = 0; i < n; i++) {
        out->mData[i] = table.Map(img->mData[i]);
    }

    return out;
}

} // namespace quink
