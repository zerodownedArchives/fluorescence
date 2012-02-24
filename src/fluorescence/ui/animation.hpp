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


#ifndef FLUO_UI_TEXTURESEQUENCE_HPP
#define FLUO_UI_TEXTURESEQUENCE_HPP

#include <boost/shared_ptr.hpp>

#include <data/ondemandreadable.hpp>

#include <stdint.h>

#include "texture.hpp"

namespace fluo {
namespace ui {

struct AnimationFrame {
    AnimationFrame() : centerX_(0), centerY_(0), texture_(new ui::Texture) { }

    int centerX_;
    int centerY_;
    boost::shared_ptr<ui::Texture> texture_;
};

class Animation : public fluo::data::OnDemandReadable<Animation> {
public:
    void addFrame(const AnimationFrame& frame);

    unsigned int getFrameCount() const;

    AnimationFrame getFrame(unsigned int idx) const;

    unsigned int getWidth(unsigned int idx) const;
    unsigned int getHeight(unsigned int idx) const;

    bool hasPixel(unsigned int idx, unsigned int pixelX, unsigned int pixelY) const;

private:
    std::vector<AnimationFrame> frames_;
};

}
}

#endif
