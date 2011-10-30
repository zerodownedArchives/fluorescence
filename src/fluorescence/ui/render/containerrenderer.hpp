#ifndef FLUO_UI_CONTAINERRENDERER_HPP
#define FLUO_UI_CONTAINERRENDERER_HPP

#include <ClanLib/Display/Render/program_object.h>
#include <ClanLib/Display/Render/graphic_context.h>

#include <boost/shared_ptr.hpp>

#include <ui/render/ingameobjectrenderer.hpp>


namespace fluo {
namespace ui {

namespace components {
class ContainerView;
}

class RenderQueue;
class Texture;

class ContainerRenderer : public IngameObjectRenderer {
public:
    ContainerRenderer(boost::shared_ptr<RenderQueue> renderQueue, components::ContainerView* gumpView);
    ~ContainerRenderer();

    virtual boost::shared_ptr<Texture> getTexture(CL_GraphicContext& gc);

    virtual void render(CL_GraphicContext& gc);

    virtual boost::shared_ptr<RenderQueue> getRenderQueue() const;

private:
    boost::shared_ptr<CL_ProgramObject> shaderProgram_;

    components::ContainerView* gumpView_;
    boost::shared_ptr<RenderQueue> renderQueue_;

    boost::shared_ptr<Texture> texture_;
    void checkTextureSize();
};
}
}

#endif
