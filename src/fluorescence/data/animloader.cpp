/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "animloader.hpp"
#include "util.hpp"

#include <typedefs.hpp>
#include <ui/texture.hpp>
#include <ui/animation.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace data {

AnimLoader::AnimLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath, unsigned int highDetailCount, unsigned int lowDetailCount) :
    highDetailCount_(highDetailCount), lowDetailCount_(lowDetailCount) {

    boost::shared_ptr<IndexedOnDemandFileLoader<unsigned int, ui::Animation> > loader(new IndexedOnDemandFileLoader<unsigned int, ui::Animation>(idxPath, mulPath,
                boost::bind(&AnimLoader::readCallback, this, _1, _2, _3, _4, _5, _6)));
    cache_.init(loader);
}

boost::shared_ptr<ui::Animation> AnimLoader::getAnimation(unsigned int bodyId, unsigned int animId, unsigned int direction) {
    unsigned int realId = 0;
    if (bodyId < highDetailCount_) {
        realId = bodyId*110;
    } else if (bodyId < highDetailCount_ + lowDetailCount_) {
        realId = (bodyId - highDetailCount_)*65 + highDetailCount_*110;
    } else {
        realId = (bodyId - highDetailCount_ - lowDetailCount_) * 175 + lowDetailCount_*65 + highDetailCount_*110;
    }

    realId += 5*animId + direction;

    boost::shared_ptr<ui::Animation> ret = cache_.get(realId);
    return ret;
}

unsigned int AnimLoader::getAnimType(unsigned int bodyId) const {
    if (bodyId < highDetailCount_) {
        return AnimType::HIGH_DETAIL;
    } else if (bodyId < highDetailCount_ + lowDetailCount_) {
        return AnimType::LOW_DETAIL;
    } else {
        return AnimType::PEOPLE;
    }
}

void AnimLoader::readCallback(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<ui::Animation> anim, unsigned int extra, unsigned int userData) {
    //LOGARG_DEBUG(LOGTYPE_DATA, "AnimLoader::readCallback index=%u len=%u", index, len);

    uint16_t* palette = reinterpret_cast<uint16_t*>(buf);

    uint32_t frameCount = *reinterpret_cast<uint32_t*>(buf + 0x200);
    uint32_t* frameOffsets = reinterpret_cast<uint32_t*>(buf + 0x200 + 4);

    //LOGARG_DEBUG(LOGTYPE_DATA, "framecount=%u", frameCount);


    for (unsigned int frameIdx = 0; frameIdx < frameCount; ++frameIdx) {
        if (frameOffsets[frameIdx] == 0) { // some animations are buggy, e.g. 0x13e3 (smithhammer) action 5
            // try to use the last texture
            if (frameIdx > 0) {
                anim->addFrame(anim->getFrame(frameIdx - 1));
            } else {
                ui::AnimationFrame curFrame;
                curFrame.centerX_ = 0;
                curFrame.centerY_ = 0;
                curFrame.texture_->initPixelBuffer(1, 1);
            }

            continue;
        }
        uint8_t* ptr = reinterpret_cast<uint8_t*>(buf) + 0x200 + frameOffsets[frameIdx];
        //int8_t* frameStart = ptr;

        // frame header
        ui::AnimationFrame curFrame;
        curFrame.centerX_ = *reinterpret_cast<int16_t*>(ptr);
        ptr += 2;
        curFrame.centerY_ = *reinterpret_cast<int16_t*>(ptr);
        ptr += 2;

        unsigned int width = *reinterpret_cast<uint16_t*>(ptr);
        ptr += 2;
        unsigned int height = *reinterpret_cast<uint16_t*>(ptr);
        ptr += 2;


        //LOGARG_DEBUG(LOGTYPE_DATA, "loop frame=%u offset=%u centerX=%u centerY=%u width=%u height=%u", frameIdx, frameOffsets[frameIdx], curFrame.centerX_, curFrame.centerY_, width, height);

        curFrame.texture_->initPixelBuffer(width, height);
        uint32_t* pixBufPtr = curFrame.texture_->getPixelBufferData();

        uint32_t header = *reinterpret_cast<uint32_t*>(ptr);
        ptr += 4;
        while (header != 0x7FFF7FFFu) {
            // read next run
            int xOffset = (header >> 22) & 0x3FF;
            int yOffset = (header >> 12) & 0x3FF;

            // keep track of sign
            if ((xOffset & 0x200) == 0x200) {
                //xOffset |= 0xFFFFFC00;
                xOffset |= 0xFFFFFE00;
            }

            if ((yOffset & 0x200) == 0x200) {
                //yOffset |= 0xFFFFFC00;
                yOffset |= 0xFFFFFE00;
            }

            unsigned int yPixel = curFrame.centerY_ + yOffset + height;
            unsigned int xRun = header & 0xFFF;

            //LOGARG_DEBUG(LOGTYPE_DATA, "xOffset=%i yOffset=%i run=%u ypixel=%u", xOffset, yOffset, xRun, yPixel);

            for (unsigned int px = 0; px < xRun; ++px) {
                uint16_t readPixel = palette[*ptr];
                ++ptr;

                unsigned int xPixel = curFrame.centerX_ + xOffset + px;

                pixBufPtr[yPixel * width + xPixel] = Util::getColorRGBA(readPixel);
            }

            header = *reinterpret_cast<uint32_t*>(ptr);
            ptr += 4;
        }

        //LOGARG_DEBUG(LOGTYPE_DATA, "read %u bytes", ptr - frameStart);

        curFrame.texture_->setReadComplete();

        anim->addFrame(curFrame);
    }
}

}
}
