
#include "shadermanager.hpp"

#include <ClanLib/Display/Render/program_object.h>
#include <ClanLib/Core/System/exception.h>

#include <misc/log.hpp>

namespace fluo {
namespace ui {

ShaderManager::ShaderManager(CL_GraphicContext& gc) {
    worldShader_.reset(new CL_ProgramObject(CL_ProgramObject::load(gc, "shader/world_vertex.glsl", "shader/world_fragment.glsl")));
    worldShader_->bind_attribute_location(0, "gl_Vertex");
    worldShader_->bind_attribute_location(1, "TexCoord0");
    worldShader_->bind_attribute_location(2, "gl_Normal");
    worldShader_->bind_attribute_location(3, "HueInfo0");

    if (!worldShader_->link()) {
        LOG_EMERGENCY << "Error while linking world shader:\n" << worldShader_->get_info_log().c_str() << std::endl;
        throw CL_Exception("Unable to link shader");
    }


    gumpShader_.reset(new CL_ProgramObject(CL_ProgramObject::load(gc, "shader/gump_vertex.glsl", "shader/gump_fragment.glsl")));
    gumpShader_->bind_attribute_location(0, "gl_Vertex");
    gumpShader_->bind_attribute_location(1, "TexCoord0");
    gumpShader_->bind_attribute_location(2, "HueInfo0");

    if (!gumpShader_->link()) {
        LOG_EMERGENCY << "Error while linking gump shader:\n" << gumpShader_->get_info_log().c_str() << std::endl;
        throw CL_Exception("Unable to link shader");
    }


    particleShader_.reset(new CL_ProgramObject(CL_ProgramObject::load(gc, "shader/particles_vertex.glsl", "shader/particles_fragment.glsl")));
    particleShader_->bind_attribute_location(0, "PositionStart");
    particleShader_->bind_attribute_location(1, "VelocityStart");
    particleShader_->bind_attribute_location(2, "VelocityEnd");
    particleShader_->bind_attribute_location(3, "Lifetime");
    particleShader_->bind_attribute_location(4, "ColorStart");
    particleShader_->bind_attribute_location(5, "ColorEnd");

    if (!particleShader_->link()) {
        LOG_EMERGENCY << "Error while linking particle shader:\n" << particleShader_->get_info_log().c_str() << std::endl;
        throw CL_Exception("Unable to link shader");
    }
}

boost::shared_ptr<CL_ProgramObject> ShaderManager::getWorldShader() {
    return worldShader_;
}

boost::shared_ptr<CL_ProgramObject> ShaderManager::getGumpShader() {
    return gumpShader_;
}

boost::shared_ptr<CL_ProgramObject> ShaderManager::getParticleShader() {
    return particleShader_;
}

}
}
