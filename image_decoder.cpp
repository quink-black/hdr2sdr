#include "image_decoder.h"

#include <algorithm>

#include "log.h"
#include "hdr_decoder.h"
#include "ldr_decoder.h"
#include "pfm_decoder.h"
#include "openexr_decoder.h"

namespace quink {

ImageWrapper ImageLoader::LoadImage(const std::string &file) {
    const ImageDecoderFactoryListType &CreatorList = GetImageDecoderFactoryList();

    using CreatorScorePair = std::pair<std::shared_ptr<ImageDecoderFactory>, int>;
    std::vector<CreatorScorePair> creatorScore;
    for (const auto &creator : CreatorList) {
        int n = creator->Probe(file);
        if (n == ImageDecoderFactory::SCORE_DEFINITELY) {
            ALOGD("create %s", creator->GetDecoderName().c_str());
            auto decoder = creator->Create();
            return decoder->Decode(file);
        } else if (n > ImageDecoderFactory::SCORE_UNSUPPORT) {
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

const ImageLoader::ImageDecoderFactoryListType &ImageLoader::GetImageDecoderFactoryList() {
    static ImageDecoderFactoryListType CreatorList{
        std::shared_ptr<ImageDecoderFactory>(new PfmDecoderFactory),
        std::shared_ptr<ImageDecoderFactory>(new OpenEXRDecoderFactory),
        std::shared_ptr<ImageDecoderFactory>(new HdrDecoderFactory),
        std::shared_ptr<ImageDecoderFactory>(new LdrDecoderFactory),
    };

    return CreatorList;
}

}
