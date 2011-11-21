#ifndef FLUO_UI_GUMPRENDERER_HPP
#define FLUO_UI_GUMPRENDERER_HPP

#include <ClanLib/Display/Render/graphic_context.h>

#include <boost/shared_ptr.hpp>

#include <ui/render/ingameobjectrenderer.hpp>


namespace fluo {
namespace ui {

namespace components {
class GumpView;
}

class RenderQueue;
class Texture;

class GumpRenderer : public IngameObjectRenderer {
public:
    GumpRenderer(boost::shared_ptr<RenderQueue> renderQueue, components::GumpView* gumpView);
    ~GumpRenderer();

    virtual boost::shared_ptr<Texture> getTexture(CL_GraphicContext& gc);

    virtual void render(CL_GraphicContext& gc);

    virtual boost::shared_ptr<RenderQueue> getRenderQueue() const;

private:
    components::GumpView* gumpView_;
    boost::shared_ptr<RenderQueue> renderQueue_;

    boost::shared_ptr<Texture> texture_;
    void checkTextureSize();
};
}
}

#endif
