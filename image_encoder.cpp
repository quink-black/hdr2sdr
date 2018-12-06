#include "image_encoder.h"

#include <limits>
#include <vector>

#include "log.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#undef STB_IMAGE_WRITE_IMPLEMENTATION

namespace quink {

int ImageEncoder::EncodeFloat(const std::string &file, std::shared_ptr<Image<float>> imgIn) {
    std::shared_ptr<Image<uint8_t>> img = std::make_shared<Image<uint8_t>>(imgIn->mWidth, imgIn->mHeight);;

    const int lowerLimit = 0;
    const int upperLimit = 255;
    const size_t n = imgIn->mWidth * imgIn->mHeight * imgIn->mChannel;
    for (size_t i = 0; i < n; i++) {
        int tmp = imgIn->mData[i] * upperLimit;
        img->mData[i] = std::min(std::max(tmp, lowerLimit), upperLimit);
    }
    return EncodeUInt8(file, img);
}

bool ImageEncoder::SetInt(const std::string &key, int value) {
    return mConfig.SetInt(key, value);
}

int PngEncoder::EncodeUInt8(const std::string &file, std::shared_ptr<Image<uint8_t>> img) {
    int ret = stbi_write_png(file.c_str(), img->mWidth, img->mHeight, img->mChannel, img->mData.get(), img->mWidth * img->mChannel);
    return ret;
}

JpegEncoder::JpegEncoder(int quality) : mQuality(quality) {
    mConfig.RegisterInt("quality", [this](int n)
            {
                if (n <= 0 || n > 100) {
                    ALOGD("set quality to %d is invalid", n);
                    return false;
                }
                mQuality = n;
                ALOGD("set quality to %d", n);
                return true;
            });
}

void JpegEncoder::JpegErrorExit(j_common_ptr jpeg) {
    JpegEncoder *This = static_cast<JpegEncoder *>(jpeg->client_data);
    assert(This);
    This->mErrorMgr.output_message(jpeg);
    longjmp(This->mJmpBuffer, 1);
}

void JpegEncoder::JpegErrorMessage(j_common_ptr jpeg) {
    char errMsg[JMSG_LENGTH_MAX];
    jpeg->err->format_message(jpeg, errMsg);
    ALOGE("%s: %s", __func__, errMsg);
}

int JpegEncoder::EncodeUInt8(const std::string &file, std::shared_ptr<Image<uint8_t>> img) {
    FILE *out = fopen(file.c_str(), "wb");
    if (!out)
        return -1;

    std::unique_ptr<FILE, std::function<void(FILE *)>> outfile(out,
            [](FILE *f) { fclose(f); });

    jpeg_compress_struct cinfo = {};
    cinfo.client_data = static_cast<void *>(this);
    cinfo.err = jpeg_std_error(&mErrorMgr);
    mErrorMgr.error_exit = &JpegEncoder::JpegErrorExit;
    mErrorMgr.output_message = &JpegEncoder::JpegErrorMessage;

    JSAMPROW rowPointer[1] = {};
    if (setjmp(mJmpBuffer))
        goto error;

    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, out);

    cinfo.image_width = img->mWidth;
    cinfo.image_height = img->mHeight;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_colorspace(&cinfo, JCS_YCbCr);
    jpeg_set_quality(&cinfo, mQuality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    while (cinfo.next_scanline < cinfo.image_height) {
        rowPointer[0] = &img->mData[cinfo.next_scanline * img->mWidth * 3];
        jpeg_write_scanlines(&cinfo, rowPointer, 1);
    }
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    return 0;

error:
    jpeg_destroy_compress(&cinfo);
    return -1;
}

PairEncoder::PairEncoder(std::shared_ptr<ImageEncoder> enc) : mEncoder(enc) { }

int PairEncoder::EncodeFloat(const std::string &file, std::shared_ptr<Image<float>> img) {
    auto imgHigh = std::make_shared<Image<uint8_t>>(img->mWidth, img->mHeight);
    auto imgLow = std::make_shared<Image<uint8_t>>(img->mWidth, img->mHeight);

    img->GammaCorrect(2.2f);
    for (int i = 0; i < img->DataLength(); i += 3) {
        float r = img->mData[i + 0];
        float g = img->mData[i + 1];
        float b = img->mData[i + 2];
        // BT.709
        float luminance = 0.212671 * r + 0.71516 * g + 0.072169 * b;
        float rLow = r / (1.0 + luminance);
        float gLow = g / (1.0 + luminance);
        float bLow = b / (1.0 + luminance);

        imgLow->mData[i + 0] = std::min<float>(rLow * 255, 255);
        imgLow->mData[i + 1] = std::min<float>(gLow * 255, 255);
        imgLow->mData[i + 2] = std::min<float>(bLow * 255, 255);
        imgHigh->mData[i + 0] = std::min<float>(r - rLow, 255);
        imgHigh->mData[i + 1] = std::min<float>(g - gLow, 255);
        imgHigh->mData[i + 2] = std::min<float>(b - bLow, 255);
    }

    int ret = mEncoder->EncodeUInt8(file + "-1" + GetDefaultSuffix(), imgLow);
    if (ret)
        return ret;
    ret = mEncoder->EncodeUInt8(file + "-2" + GetDefaultSuffix(), imgHigh);
    return ret;
}

std::string PairEncoder::GetDefaultSuffix() {
    return mEncoder->GetDefaultSuffix();
}

} // namespace quink
