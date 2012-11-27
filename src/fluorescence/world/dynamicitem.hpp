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


#ifndef FLUO_WORLD_DYNAMICITEM_HPP
#define FLUO_WORLD_DYNAMICITEM_HPP

#include <list>
#include <boost/shared_ptr.hpp>

#include <typedefs.hpp>
#include <data/tiledataloader.hpp>

#include "serverobject.hpp"

namespace fluo {

namespace ui {
class Texture;
class TextureProvider;
class AnimTextureProvider;
class GumpMenu;

namespace components {
class ContainerView;
}
}

namespace world {
class Mobile;

class DynamicItem : public ServerObject {

public:
    DynamicItem(Serial serial);

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const;
    virtual boost::shared_ptr<ui::Texture> getGumpTexture() const;

    void setArtId(unsigned int artId);
    unsigned int getArtId() const;

    virtual bool isMirrored() const;
    void setDirection(unsigned int direction);
    unsigned int getDirection() const;

    void setAmount(unsigned int amount);
    unsigned int getAmount() const;
    void setStackIdOffset(unsigned int offset);

    const data::StaticTileInfo* getTileDataInfo() const;

    virtual void onClick();
    virtual void onDoubleClick();

    virtual void onStartDrag(const CL_Point& mousePos);
    virtual void onDraggedOnto(boost::shared_ptr<IngameObject> obj, int locX = -1, int locY = -1);

    void setLayer(unsigned int layer);
    unsigned int getLayer() const;

    void animate(unsigned int animId, unsigned int delay = 1, unsigned int repeatMode = AnimRepeatMode::DEFAULT);
    void setIdleAnim(unsigned int animId);

    virtual void onAddedToParent();
    virtual void onRemovedFromParent();
    virtual void onChildObjectAdded(const boost::shared_ptr<IngameObject>& obj);
    virtual void onBeforeChildObjectRemoved(const boost::shared_ptr<IngameObject>& obj);

    virtual void onDelete();

    void openContainerGump(unsigned int gumpId);
    void onContainerGumpClosed();

    bool isSpellbook() const;
    void setSpellbook(unsigned int scrollOffset, const uint8_t* spellBits);
    unsigned int getSpellbookScrollOffset() const;
    uint8_t getSpellbookSpellBits(unsigned int byteIndex) const;

    void setContainerView(ui::components::ContainerView* view);
    void setSpellbookGump(ui::GumpMenu* menu);

private:
    unsigned int artId_;
    unsigned int amount_;
    unsigned int stackIdOffset_;
    unsigned int direction_;

    boost::shared_ptr<ui::TextureProvider> textureProvider_;
    boost::shared_ptr<ui::TextureProvider> gumpTextureProvider_;

    const data::StaticTileInfo* tileDataInfo_;

    void updateVertexCoordinates();
    void updateRenderDepth();
    void updateTextureProvider();
    bool updateAnimation(unsigned int elapsedMillis);

    bool equipped_;
    unsigned int layer_;
    boost::shared_ptr<ui::AnimTextureProvider> animTextureProvider_;

    ui::GumpMenu* spellbookGump_;
    ui::components::ContainerView* containerView_;

    unsigned int animType_; // only relevant when mounted

    bool isSpellbook_;
    unsigned int spellbookScrollOffset_;
    uint8_t spellbookSpellBits_[8];
    bool spellbookClosedCallback();
};

}
}

#endif
