
#include "particleeffect.hpp"

namespace fluo {
namespace world {

ParticleEffect::ParticleEffect() : IngameObject(IngameObject::TYPE_MAP) {
}

bool ParticleEffect::hasPixel(int pixelX, int pixelY) const {
    return false;
}

bool ParticleEffect::isInDrawArea(int leftPixelCoord, int rightPixelCoord, int topPixelCoord, int bottomPixelCoord) const {
    return true;
}

boost::shared_ptr<ui::Texture> ParticleEffect::getIngameTexture() const {
    static boost::shared_ptr<ui::Texture> ret;
    return ret;
}

}
}

