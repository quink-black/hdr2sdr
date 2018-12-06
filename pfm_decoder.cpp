#include "pfm_decoder.h"

#include <algorithm>
#include <functional>

namespace quink {
int PfmDecoder::Creator::Probe(const std::string &file) const {
    auto suffix_pos = file.rfind(".");
    if (suffix_pos != std::string::npos) {
        std::string suffix = file.substr(suffix_pos);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(), tolower);
        if (suffix == ".pfm")
            return SCORE_DEFINITELY;
    }
    return SCORE_UNSUPPORT;
}

std::unique_ptr<ImageDecoder> PfmDecoder::Creator::Create() const {
    return std::unique_ptr<ImageDecoder>(new PfmDecoder());
}

ImageWrapper PfmDecoder::Decode(const std::string &file) {
    std::unique_ptr<FILE, std::function<void(FILE *)>> in(fopen(file.c_str(), "rb"),
            [](FILE *f) { fclose(f); });
    ImageWrapper imgWrapper;

    char buf[2];
    if (fscanf(in.get(), "%2c ", buf) != 1 || buf[0] != 'P' || buf[1] != 'F') {
        return imgWrapper;
    }

    int width, height;
    if (fscanf(in.get(), "%d %d ", &width, &height) != 2 || width < 0 || height < 0) {
        return imgWrapper;
    }

    float f;
    if (fscanf(in.get(), "%f ", &f) != 1) {
        return imgWrapper;
    }

    auto img = std::make_shared<Image<float>>(width, height);
    size_t n = width * height * 3;
    if (fread(img->mData.get(), sizeof(float), n, in.get()) != n) {
        fprintf(stderr, "file %s is broken", file.c_str());
        return imgWrapper;
    }

    if (f > 0) {
        // data is in big endian
        union {
            uint8_t buf[4];
            float f;
        } tmp;
        for (size_t i = 0; i < n; i++) {
            tmp.f = img->mData[i];
            std::swap(tmp.buf[0], tmp.buf[3]);
            std::swap(tmp.buf[1], tmp.buf[2]);
            img->mData[i] = tmp.f;
        }
    }

    imgWrapper.Reset(img);

    return imgWrapper;
}

}
