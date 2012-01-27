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


#ifndef FLUO_WORLD_OVERHEADMESSAGE_HPP
#define FLUO_WORLD_OVERHEADMESSAGE_HPP

#include <boost/shared_ptr.hpp>

#include "ingameobject.hpp"
#include <misc/string.hpp>

namespace fluo {

namespace ui {
class Texture;
}

namespace world {
class OverheadMessage : public IngameObject {
public:
    // If parameter useRgbColor is true, the color value is interpreted as a 32bit rgba value. If false, like a uo hue id
    OverheadMessage(const UnicodeString& text, unsigned int font, unsigned int color, bool useRgbColor = true);

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const;

    void setParentPixelOffset(int y);

    virtual void onClick();
    virtual void onAddedToParent();
    virtual void onRemovedFromParent();

    bool isExpired() const;
    void expire();

private:
    UnicodeString text_;
    boost::shared_ptr<ui::Texture> texture_;

    void updateVertexCoordinates();
    void updateRenderDepth();
    void updateTextureProvider();
    bool updateAnimation(unsigned int elapsedMillis);

    int parentPixelOffsetY_;

    int milliSecondsToLive_;
};
}
}


#endif
