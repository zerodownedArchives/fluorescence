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

#include <data/tiledataloader.hpp>

#include "serverobject.hpp"

namespace fluo {

namespace ui {
    class Texture;
    class TextureProvider;
    class AnimTextureProvider;
    class GumpMenu;
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

    void setAmount(unsigned int amount);
    unsigned int getAmount() const;
    void setStackIdOffset(unsigned int offset);

    const data::StaticTileInfo* getTileDataInfo() const;

    virtual void onClick();
    virtual void onDoubleClick();

    virtual void onStartDrag(const CL_Point& mousePos);
    virtual void onDraggedOnto(boost::shared_ptr<IngameObject> obj);

    void setLayer(unsigned int layer);
    unsigned int getLayer() const;

    void playAnim(unsigned int animId);

    virtual void onAddedToParent();
    virtual void onRemovedFromParent();
    virtual void onChildObjectAdded(boost::shared_ptr<IngameObject> obj);
    virtual void onChildObjectRemoved(boost::shared_ptr<IngameObject> obj);

    void openContainerGump(unsigned int gumpId);
    void onContainerGumpClosed();

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

    ui::GumpMenu* containerGump_;
};

}
}

#endif
