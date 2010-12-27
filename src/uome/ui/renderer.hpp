#ifndef UOME_UI_RENDERER_HPP
#define UOME_UI_RENDERER_HPP

#include <ClanLib/Display/Render/program_object.h>
#include <ClanLib/Display/Render/graphic_context.h>

#include <boost/shared_ptr.hpp>

namespace uome {
namespace ui {

class RenderQueue;

class Renderer {
public:
    Renderer(CL_GraphicContext& gc);
    ~Renderer();

    void renderOneFrame();
    void setIngameClipping(int leftPixelCoord, int rightPixelCoord, int topPixelCoord, int bottomPixelCoord);

    boost::shared_ptr<RenderQueue> getRenderQueue();

private:
    bool initialized_;
    void initialize();

    boost::shared_ptr<CL_ProgramObject> shaderProgram_;

    boost::shared_ptr<RenderQueue> renderQueue_;

    int ingameClippingLeftPixelCoord_;
    int ingameClippingRightPixelCoord_;
    int ingameClippingTopPixelCoord_;
    int ingameClippingBottomPixelCoord_;
};

}
}

#endif
