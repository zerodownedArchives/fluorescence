#ifndef FLUO_WORLD_PARTICLEFFECT_HPP
#define FLUO_WORLD_PARTICLEFFECT_HPP

#include <boost/shared_ptr.hpp>
#include <list>
#include <ClanLib/Display/Render/graphic_context.h>

#include "ingameobject.hpp"

namespace fluo {

namespace ui {
namespace particles {
class Emitter;
}
}
    
namespace world {

class ParticleEffect : public IngameObject {
public:
    ParticleEffect();

    virtual bool hasPixel(int pixelX, int pixelY) const;
    virtual bool isInDrawArea(int leftPixelCoord, int rightPixelCoord, int topPixelCoord, int bottomPixelCoord) const;

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const;
    virtual void update(unsigned int elapsedMillis);
    
    virtual void updateTextureProvider();
    virtual bool updateAnimation(unsigned int elapsedMillis);
    virtual void updateVertexCoordinates();
    virtual void updateRenderDepth();
    
    
    void renderAll(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader);
    void addEmitter(boost::shared_ptr<ui::particles::Emitter> emitter);
    
private:
    std::list<boost::shared_ptr<ui::particles::Emitter> > emitters_;
};

}
}

#endif
