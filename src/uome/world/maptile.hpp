#ifndef UOME_WORLD_MAPTILE_HPP
#define UOME_WORLD_MAPTILE_HPP


namespace uome {
namespace world {

class MapTile : public IngameObject {
public:
    MapTile() { }
    MapTile(int locX, int locY, int locZ, unsigned int artId) :
            artId_(artId), texture_(data::Data::getArtLoader()->getMapTexture(artId)) {
        setLocation(locX, locY, locZ);
    }

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() {
        return texture_;
    }

private:
    unsigned int artId_;

    boost::shared_ptr<ui::Texture> texture_;
};

}
}

#endif
