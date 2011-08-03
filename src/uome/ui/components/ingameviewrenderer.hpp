#ifndef UOME_UI_INGAMEVIEWRENDERER_HPP
#define UOME_UI_INGAMEVIEWRENDERER_HPP

#include <ClanLib/Display/Render/program_object.h>
#include <ClanLib/Display/Render/graphic_context.h>

#include <boost/shared_ptr.hpp>

namespace uome {
namespace ui {

class RenderQueue;
class IngameView;

class IngameViewRenderer {
public:
    IngameViewRenderer(IngameView* ingameView);
    ~IngameViewRenderer();

    void renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect);

private:
    boost::shared_ptr<CL_ProgramObject> shaderProgram_;

    IngameView* ingameView_;
};

}
}

#endif
