#ifndef FLUO_UI_INGAMEVIEWRENDERER_HPP
#define FLUO_UI_INGAMEVIEWRENDERER_HPP

#include <ClanLib/Display/Render/program_object.h>
#include <ClanLib/Display/Render/graphic_context.h>

#include <boost/shared_ptr.hpp>

#include "ui/ingameobjectrenderer.hpp"

namespace fluo {
namespace ui {

class RenderQueue;
class IngameView;
class Texture;

class IngameViewRenderer : public IngameObjectRenderer {
public:
    IngameViewRenderer(boost::shared_ptr<RenderQueue> renderQueue, IngameView* ingameView);
    ~IngameViewRenderer();

    virtual boost::shared_ptr<Texture> getTexture(CL_GraphicContext& gc);
    virtual void render(CL_GraphicContext& gc);

private:
    boost::shared_ptr<CL_ProgramObject> shaderProgram_;

    IngameView* ingameView_;
    boost::shared_ptr<RenderQueue> renderQueue_;

    boost::shared_ptr<Texture> texture_;
    void checkTextureSize();
};

}
}

#endif
