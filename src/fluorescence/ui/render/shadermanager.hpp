#ifndef FLUO_UI_SHADERMANAGER_HPP
#define FLUO_UI_SHADERMANAGER_HPP

#include <ClanLib/Display/Render/graphic_context.h>
#include <boost/shared_ptr.hpp>

class CL_ProgramObject;

namespace fluo {
namespace ui {

class ShaderManager {
public:
    ShaderManager(CL_GraphicContext& gc);


    boost::shared_ptr<CL_ProgramObject> getWorldShader();
    boost::shared_ptr<CL_ProgramObject> getGumpShader();

private:
    boost::shared_ptr<CL_ProgramObject> worldShader_;
    boost::shared_ptr<CL_ProgramObject> gumpShader_;
};

}
}

#endif
