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
