#include "tonemapper.h"

namespace hdr2sdr {

std::shared_ptr<Image> HableMapper::Map(std::shared_ptr<Image> img, bool inPlace) {
    auto hable = [](float in) {
        float a = 0.15f, b = 0.50f, c = 0.10f, d = 0.20f, e = 0.02f, f = 0.30f;
        return (in * (in * a + b * c) + d * e) / (in * (in * a + b) + d * f) - e / f;
    };

    if (img == nullptr)
        return nullptr;

    std::shared_ptr<Image> out;
    if (inPlace)
        out = img;
    else
        out = std::make_shared<Image>(img->mWidth, img->mHeight);

    int n = img->mWidth * img->mHeight * img->mChannel;
    const float exposureBias = 2.0f;
    const float whiteScale = 1.0 / hable(11.2);
    for (int i = 0; i < n; i++) {
        float curr = hable(exposureBias * img->mData[i]);
        float color = curr * whiteScale;
        out->mData[i] = color;
    }
    return out;
}

} // namespace hdr2sdr