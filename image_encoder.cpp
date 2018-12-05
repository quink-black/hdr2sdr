#include "image_encoder.h"

#include <limits>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#undef STB_IMAGE_WRITE_IMPLEMENTATION

namespace hdr2sdr {

int ImageEncoder::EncodeFloat(const std::string &file, std::shared_ptr<Image<float>> imgIn) {
    std::shared_ptr<Image<uint8_t>> img = std::make_shared<Image<uint8_t>>(imgIn->mWidth, imgIn->mHeight);;

    const int lowerLimit = 0;
    const int upperLimit = 255;
    const size_t n = imgIn->mWidth * imgIn->mHeight * imgIn->mChannel;
    for (size_t i = 0; i < n; i++) {
        int tmp = imgIn->mData[i] * upperLimit;
        img->mData[i] = std::min(std::max(tmp, lowerLimit), upperLimit);
    }
    return EncodeInt8(file, img);
}

int ImageEncoder::SetInt(const std::string &key, int value) {
    auto search = ParamsInt.find(key);
    if (search == ParamsInt.end())
        return -1;
    *search->second = value;
    return 0;
}

void ImageEncoder::RegisterInt(const std::string &key, int *p) {
    assert(ParamsInt.find(key) == ParamsInt.end());
    ParamsInt[key] = p;
}

int PngEncoder::EncodeInt8(const std::string &file, std::shared_ptr<Image<uint8_t>> img) {
    int ret = stbi_write_png(file.c_str(), img->mWidth, img->mHeight, img->mChannel, img->mData.get(), img->mWidth * img->mChannel);
    return ret;
}

JpegEncoder::JpegEncoder(int quality) : mQuality(quality) {
    RegisterInt("quality", &mQuality);
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
    fprintf(stderr, "%s: %s\n", __func__, errMsg);
}

int JpegEncoder::EncodeInt8(const std::string &file, std::shared_ptr<Image<uint8_t>> img) {
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

} // namespace hdr2sdr
