
#include "huesloader.hpp"

#include "util.hpp"

#include <ui/texture.hpp>
#include <misc/log.hpp>

#include <boost/bind.hpp>

namespace uome {
namespace data {

HuesLoader::HuesLoader(const boost::filesystem::path& path) {
    FullFileLoader loader(path);
    loader.read(boost::bind(&HuesLoader::readCallback, this, _1, _2));
}

HuesLoader::~HuesLoader() {
    if (hues_) {
        delete [] hues_;
        hues_ = NULL;
    }
}

void HuesLoader::readCallback(int8_t* buf, unsigned int len) {
    hueCount_ = (len / ((88 * 8) + 4)) * 8;
    hues_ = new Hue[hueCount_];

    uint16_t* ptr = reinterpret_cast<uint16_t*>(buf);

    for (unsigned int i = 0; i < hueCount_; ++i) {
        // jump header
        if ((i % 8) == 0) {
            ptr += 2;
        }

        for (unsigned int j = 0; j < 32; ++j) {
            hues_[i].colorTable_[j] = Util::getColorRGBA(*ptr);
            ++ptr;
        }

        hues_[i].tableStart = *ptr;
        ++ptr;
        hues_[i].tableEnd = *ptr;
        ++ptr;

        hues_[i].name_ = StringConverter::fromUtf8(reinterpret_cast<const char*>(ptr), 20);
        ptr += 10;
    }

    LOG_DEBUG << "Total read bytes: " << (reinterpret_cast<int8_t*>(ptr) - buf) << " len: " << len << std::endl;
}

unsigned int HuesLoader::getHueCount() const {
    return hueCount_;
}

const uint32_t* HuesLoader::getColorTable(unsigned int id) const {
    if (id > hueCount_) {
        LOG_WARN << "Trying to read hue with out of bounds id " << id << std::endl;
        id = 0;
    }

    return hues_[id].colorTable_;
}

boost::shared_ptr<ui::Texture> HuesLoader::getHuesTexture() {
    if (!huesTexture_) {
        huesTexture_.reset(new ui::Texture());
        huesTexture_->initPixelBuffer(32, hueCount_);

        uint32_t* pxBuf = huesTexture_->getPixelBufferData();

        for (unsigned int hueIdx = 0; hueIdx < hueCount_; ++hueIdx) {
            for (unsigned int pxIdx = 0; pxIdx < 32; ++pxIdx) {
                *pxBuf++ = hues_[hueIdx].colorTable_[pxIdx];
            }
        }

        huesTexture_->setReadComplete();
    }

    return huesTexture_;
}

}
}
