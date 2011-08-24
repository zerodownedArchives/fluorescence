#ifndef UOME_WORLD_DYNAMICITEM_HPP
#define UOME_WORLD_DYNAMICITEM_HPP

#include <list>
#include <boost/shared_ptr.hpp>

#include <data/tiledataloader.hpp>

#include <ui/textureprovider.hpp>

#include "serverobject.hpp"

namespace uome {

namespace ui {
    class Texture;
}

namespace world {

class DynamicItem : public ServerObject {

public:
    DynamicItem(Serial serial);

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const;

    void setArtId(unsigned int artId);
    unsigned int getArtId() const;

    void setDirection(unsigned int direction);

    void setAmount(unsigned int amount);
    void setStackIdOffset(unsigned int offset);

    void setHue(unsigned int hue);

    const data::StaticTileInfo* getTileDataInfo() const;

    virtual void onClick();
    virtual void onDoubleClick();

    virtual void onStartDrag();

private:
    unsigned int artId_;
    unsigned int hue_;
    unsigned int amount_;
    unsigned int stackIdOffset_;
    unsigned int direction_;

    boost::shared_ptr<ui::TextureProvider> textureProvider_;

    const data::StaticTileInfo* tileDataInfo_;

    void updateVertexCoordinates();
    void updateRenderPriority();
    void updateTextureProvider();
    bool updateAnimation(unsigned int elapsedMillis);
};

}
}

#endif
