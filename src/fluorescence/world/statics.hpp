#ifndef FLUO_WORLD_STATICS_HPP
#define FLUO_WORLD_STATICS_HPP

#include <list>
#include <boost/shared_ptr.hpp>

#include <data/ondemandreadable.hpp>
#include <data/tiledataloader.hpp>

#include <ui/textureprovider.hpp>

#include "ingameobject.hpp"

namespace fluo {

namespace data {
    class StaticsLoader;
}

namespace ui {
    class Texture;
}

namespace world {

class StaticItem : public IngameObject {
friend class data::StaticsLoader;

public:
    StaticItem();

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const;

    unsigned int getArtId() const;

    const data::StaticTileInfo* getTileDataInfo() const;

    virtual void onClick();

    // returns wheter other not this static tile requires periodic updates to updateRenderData to be rendere correctly.
    // this is only the case if is animated
    bool requireRenderUpdate() const;

private:
    unsigned int artId_;
    unsigned int hue_;
    unsigned int indexInBlock_;

    boost::shared_ptr<ui::TextureProvider> textureProvider_;

    const data::StaticTileInfo* tileDataInfo_;

    void set(int locX, int locY, int locZ, unsigned int artId, unsigned int hue);

    void updateVertexCoordinates();
    void updateRenderDepth();
    void updateTextureProvider();
    bool updateAnimation(unsigned int elapsedMillis);
};

class StaticBlock : public data::OnDemandReadable {
friend class data::StaticsLoader;

public:
    std::list<boost::shared_ptr<StaticItem> >& getItemList();

private:
    std::list<boost::shared_ptr<StaticItem> > itemList_;

    unsigned int blockIndexX_;
    unsigned int blockIndexY_;
};

}
}

#endif
