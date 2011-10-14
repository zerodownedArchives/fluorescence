#ifndef FLUO_UI_INGAMEOBJECTRENDERER_HPP
#define FLUO_UI_INGAMEOBJECTRENDERER_HPP

#include <boost/shared_ptr.hpp>
#include <ClanLib/Display/Render/graphic_context.h>

namespace fluo {
namespace ui {

class Texture;
class RenderQueue;

class IngameObjectRenderer {
public:
    enum {
        TYPE_WORLD, // render the world "as-is", with all objects, including speech etc
        TYPE_GUMP, // render gump images, e.g. paperdoll
    };

    IngameObjectRenderer(unsigned int rendererType, bool polling);

    // whether this renderer is constantly polling the objects for change or has to be notified
    bool isPolling() const;

    // get the rendered objects as a texture. can be used i.e. for postprocessing or for a cached texture that does not change very often
    virtual boost::shared_ptr<Texture> getTexture(CL_GraphicContext& gc) = 0;

    // render the objects to the current state of the gc
    virtual void render(CL_GraphicContext& gc) = 0;


    bool isWorldRenderer() const;
    bool isGumpRenderer() const;

    virtual boost::shared_ptr<RenderQueue> getRenderQueue() const = 0;

private:
    unsigned int rendererType_;
    bool polling_;
};

}
}


#endif
