#ifndef UOME_WORLD_STATICS_HPP
#define UOME_WORLD_STATICS_HPP

#include <list>
#include <boost/shared_ptr.hpp>

#include <data/ondemandreadable.hpp>
#include <data/tiledataloader.hpp>

#include "ingameobject.hpp"

namespace uome {

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
    virtual ui::Texture* getIngameTexture() const;

    unsigned int getArtId();

    void updateVertexCoordinates();
    void updateRenderPriority();
    void updateTextureProvider();

private:
    unsigned int artId_;
    unsigned int hue_;
    unsigned int indexInBlock_;

    boost::shared_ptr<ui::Texture> texture_;

    const data::StaticTileInfo* tileDataInfo_;

    void set(int locX, int locY, int locZ, unsigned int artId, unsigned int hue);
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
