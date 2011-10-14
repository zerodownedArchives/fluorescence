#ifndef FLUO_UI_GUMPRENDERER_HPP
#define FLUO_UI_GUMPRENDERER_HPP

#include <ClanLib/Display/Render/program_object.h>
#include <ClanLib/Display/Render/graphic_context.h>

#include <boost/shared_ptr.hpp>

#include <ui/ingameobjectrenderer.hpp>


namespace fluo {
namespace ui {

class RenderQueue;
class GumpView;
class Texture;

class GumpRenderer : public IngameObjectRenderer {
public:
    GumpRenderer(boost::shared_ptr<RenderQueue> renderQueue, GumpView* gumpView);

    virtual boost::shared_ptr<Texture> getTexture(CL_GraphicContext& gc);

    virtual void render(CL_GraphicContext& gc);

    virtual boost::shared_ptr<RenderQueue> getRenderQueue() const;

private:
    boost::shared_ptr<CL_ProgramObject> shaderProgram_;

    GumpView* gumpView_;
    boost::shared_ptr<RenderQueue> renderQueue_;

    boost::shared_ptr<Texture> texture_;
    void checkTextureSize();
};
}
}

#endif
