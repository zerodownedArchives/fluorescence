#ifndef FLUO_UI_WORLDVIEWRENDERER_HPP
#define FLUO_UI_WORLDVIEWRENDERER_HPP

#include <ClanLib/Display/Render/program_object.h>
#include <ClanLib/Display/Render/graphic_context.h>

#include <boost/shared_ptr.hpp>

#include <ui/render/ingameobjectrenderer.hpp>

namespace fluo {
namespace ui {

class RenderQueue;
class Texture;

namespace components {
class WorldView;
}

class WorldViewRenderer : public IngameObjectRenderer {
public:
    WorldViewRenderer(boost::shared_ptr<RenderQueue> renderQueue, components::WorldView* ingameView);
    ~WorldViewRenderer();

    virtual boost::shared_ptr<Texture> getTexture(CL_GraphicContext& gc);
    virtual void render(CL_GraphicContext& gc);

    virtual boost::shared_ptr<RenderQueue> getRenderQueue() const;

private:
    boost::shared_ptr<CL_ProgramObject> shaderProgram_;

    components::WorldView* worldView_;
    boost::shared_ptr<RenderQueue> renderQueue_;

    boost::shared_ptr<Texture> texture_;
    void checkTextureSize();
};

}
}

#endif
