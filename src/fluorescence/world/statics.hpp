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
    bool periodicRenderUpdateRequired() const;
    
    static bool isIdIgnored(unsigned int artId);
    static bool isIdWater(unsigned int artId);

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

class StaticBlock : public data::OnDemandReadable<StaticBlock> {
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
