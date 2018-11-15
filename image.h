#ifndef HDR2SDR_IMAGE_H
#define HDR2SDR_IMAGE_H

#include <memory>
#include <string>
#include <vector>

namespace hdr2sdr {

struct Image {
    const int mChannel = 3;
    int mWidth;
    int mHeight;
    std::unique_ptr<float[]> mData;

    Image(int width, int height) :
        mWidth(width), mHeight(height), mData(new float[width * height * mChannel]) { }
};

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
    virtual std::unique_ptr<Image> Decode(const std::string &file) = 0;
};

class ImageLoader {
public:
    using DecoderBuilderListType = std::vector<std::shared_ptr<ImageDecoder::DecoderBuilder>>;
    static std::unique_ptr<Image> LoadImage(const std::string &file);

private:
    static const DecoderBuilderListType &GetDecoderBuilderList();
};

} // namespace hdr2sdr

#endif
