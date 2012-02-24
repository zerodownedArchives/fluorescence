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


#ifndef FLUO_UI_ANIMTEXTUREPROVIDER_HPP
#define FLUO_UI_ANIMTEXTUREPROVIDER_HPP

#include <boost/shared_ptr.hpp>

#include <vector>
#include <map>

#include <typedefs.hpp>
#include <data/animdataloader.hpp>

#include "textureprovider.hpp"
#include "texture.hpp"
#include "animation.hpp"

namespace fluo {
namespace ui {

class AnimTextureProvider : public TextureProvider {
public:
    AnimTextureProvider(unsigned int animId);

    virtual boost::shared_ptr<ui::Texture> getTexture() const;
    AnimationFrame getCurrentFrame() const;

    virtual bool update(unsigned int elapsedMillis);

    void setRepeatMode(unsigned int mode);
    void setDelay(unsigned int delay);

    void setDirection(unsigned int direction);
    void setAnimId(unsigned int animId);
    
    void setIdleAnim();

private:
    // stores for each anim (walk, run, ...) the frames for all directions
    std::map<unsigned int, std::vector<boost::shared_ptr<Animation> > > animations_;
    unsigned int bodyId_;
    unsigned int currentAnimId_;
    unsigned int direction_;
    unsigned int currentIdx_;
    unsigned long millis_;
    unsigned int frameMillis_;
    unsigned int repeatMode_;

    unsigned int defaultAnimId_;
    unsigned int nextAnimId_;
    unsigned int nextDirection_;
};

}
}

#endif
