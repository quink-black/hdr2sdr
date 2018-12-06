#include "tonemapper.h"

namespace quink {

std::shared_ptr<Image<float>> HableMapper::Map(std::shared_ptr<Image<float>> img, bool inPlace) {
    auto hable = [](float in) {
        float a = 0.15f, b = 0.50f, c = 0.10f, d = 0.20f, e = 0.02f, f = 0.30f;
        return (in * (in * a + b * c) + d * e) / (in * (in * a + b) + d * f) - e / f;
    };

    if (img == nullptr)
        return nullptr;

    img->GammaCorrect(1.0f);
    std::shared_ptr<Image<float>> out;
    if (inPlace)
        out = img;
    else
        out = std::make_shared<Image<float>>(img->mWidth, img->mHeight);

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

} // namespace quink
