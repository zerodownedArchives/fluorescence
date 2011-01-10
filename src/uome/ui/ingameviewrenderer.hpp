#ifndef UOME_UI_INGAMEWINDOWRENDERER_HPP
#define UOME_UI_INGAMEWINDOWRENDERER_HPP

#include <ClanLib/Display/Render/program_object.h>
#include <ClanLib/Display/Render/graphic_context.h>

#include <boost/shared_ptr.hpp>

namespace uome {
namespace ui {

class RenderQueue;
class IngameWindow;

class IngameWindowRenderer {
public:
    IngameWindowRenderer(IngameWindow* ingameWindow);
    ~IngameWindowRenderer();

    void renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect);

private:
    boost::shared_ptr<CL_ProgramObject> shaderProgram_;

    IngameWindow* ingameWindow_;
};

}
}

#endif
