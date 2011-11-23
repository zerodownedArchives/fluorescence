#ifndef FLUO_WORLD_PARTICLEFFECT_HPP
#define FLUO_WORLD_PARTICLEFFECT_HPP

#include "ingameobject.hpp"

namespace fluo {
namespace world {

class ParticleEffect : public IngameObject {
public:
    ParticleEffect();

    virtual bool hasPixel(int pixelX, int pixelY) const;
    virtual bool isInDrawArea(int leftPixelCoord, int rightPixelCoord, int topPixelCoord, int bottomPixelCoord) const;

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const;
};

}
}

#endif
