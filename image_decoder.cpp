#include "image_decoder.h"

#include <algorithm>

#include "log.h"
#include "hdr_decoder.h"
#include "ldr_decoder.h"
#include "pfm_decoder.h"
#include "openexr_decoder.h"

namespace quink {

ImageWrapper ImageLoader::LoadImage(const std::string &file) {
    const DecoderCreatorListType &CreatorList = GetDecoderCreatorList();

    using CreatorScorePair = std::pair<std::shared_ptr<ImageDecoder::DecoderCreator>, int>;
    std::vector<CreatorScorePair> creatorScore;
    for (const auto &creator : CreatorList) {
        int n = creator->Probe(file);
        if (n == ImageDecoder::DecoderCreator::SCORE_DEFINITELY) {
            ALOGD("create %s", creator->GetDecoderName().c_str());
            auto decoder = creator->Create();
            return decoder->Decode(file);
        } else if (n > ImageDecoder::DecoderCreator::SCORE_UNSUPPORT) {
            creatorScore.push_back(CreatorScorePair(creator, n));
        }
    }

    if (creatorScore.empty())
        return ImageWrapper();

    std::sort(creatorScore.begin(), creatorScore.end(),
            [](const CreatorScorePair &a, const CreatorScorePair &b)
            {
                return a.second < b.second;
            });
    ALOGD("create %s", creatorScore.back().first->GetDecoderName().c_str());
    auto decoder = creatorScore.back().first->Create();
    return decoder->Decode(file);
}

const ImageLoader::DecoderCreatorListType &ImageLoader::GetDecoderCreatorList() {
    static DecoderCreatorListType CreatorList{
        std::shared_ptr<ImageDecoder::DecoderCreator>(new PfmDecoder::Creator),
        std::shared_ptr<ImageDecoder::DecoderCreator>(new OpenEXRDecoder::Creator),
        std::shared_ptr<ImageDecoder::DecoderCreator>(new HdrDecoder::Creator),
        std::shared_ptr<ImageDecoder::DecoderCreator>(new LdrDecoder::Creator),
    };

    return CreatorList;
}

}
