#include "image_decoder.h"

#include "pfm_decoder.h"
#include "openexr_decoder.h"

namespace quink {

ImageWrapper ImageLoader::LoadImage(const std::string &file) {
    const DecoderCreatorListType &CreatorList = GetDecoderCreatorList();

    // TODO: sort by scores
    for (const auto &Creator : CreatorList) {
        int n = Creator->Probe(file);
        if (n == ImageDecoder::DecoderCreator::SCORE_DEFINITELY) {
            auto decoder = Creator->Create();
            return decoder->Decode(file);
        }
    }
    return ImageWrapper();
}

const ImageLoader::DecoderCreatorListType &ImageLoader::GetDecoderCreatorList() {
    static DecoderCreatorListType CreatorList{
        std::shared_ptr<ImageDecoder::DecoderCreator>(new PfmDecoder::Creator),
        std::shared_ptr<ImageDecoder::DecoderCreator>(new OpenEXRDecoder::Creator),
    };

    return CreatorList;
}

}
