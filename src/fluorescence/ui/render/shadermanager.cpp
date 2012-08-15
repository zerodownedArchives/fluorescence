/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "shadermanager.hpp"

#include <ClanLib/Display/Render/program_object.h>
#include <ClanLib/Core/System/exception.h>

#include <misc/log.hpp>

namespace fluo {
namespace ui {

ShaderManager::ShaderManager() {
}

bool ShaderManager::initShaders(CL_GraphicContext& gc) {
    LOG_DEBUG << "Loading world shader" << std::endl;
    try {
        worldShader_.reset(new CL_ProgramObject(CL_ProgramObject::load(gc, "shader/world_vertex.glsl", "shader/world_fragment.glsl")));
    } catch (const CL_Exception& ex) {
        LOG_EMERGENCY << "Error while loading world shader: " << ex.what() << std::endl;
        return false;
    }
    worldShader_->bind_attribute_location(0, "gl_Vertex");
    worldShader_->bind_attribute_location(1, "TexCoord0");
    worldShader_->bind_attribute_location(2, "gl_Normal");
    worldShader_->bind_attribute_location(3, "HueInfo0");
    worldShader_->bind_attribute_location(4, "Material0");

    if (!worldShader_->link()) {
        LOG_EMERGENCY << "Error while linking world shader:\n" << worldShader_->get_info_log().c_str() << std::endl;
        return false;
    }


    LOG_DEBUG << "Loading gump shader" << std::endl;
    try {
        gumpShader_.reset(new CL_ProgramObject(CL_ProgramObject::load(gc, "shader/gump_vertex.glsl", "shader/gump_fragment.glsl")));
    } catch (const CL_Exception& ex) {
        LOG_EMERGENCY << "Error while loading gump shader: " << ex.what() << std::endl;
        return false;
    }
    gumpShader_->bind_attribute_location(0, "gl_Vertex");
    gumpShader_->bind_attribute_location(1, "TexCoord0");
    gumpShader_->bind_attribute_location(2, "HueInfo0");

    if (!gumpShader_->link()) {
        LOG_EMERGENCY << "Error while linking gump shader:\n" << gumpShader_->get_info_log().c_str() << std::endl;
        return false;
    }


    LOG_DEBUG << "Loading particle shader" << std::endl;
    try {
        particleShader_.reset(new CL_ProgramObject(CL_ProgramObject::load(gc, "shader/particles_vertex.glsl", "shader/particles_fragment.glsl")));
    } catch (const CL_Exception& ex) {
        LOG_EMERGENCY << "Error while loading particle shader: " << ex.what() << std::endl;
        return false;
    }
    particleShader_->bind_attribute_location(0, "LocationStart");
    particleShader_->bind_attribute_location(1, "VelocityStart");
    particleShader_->bind_attribute_location(2, "VelocityEnd");
    particleShader_->bind_attribute_location(3, "Lifetime");
    particleShader_->bind_attribute_location(4, "ColorStart");
    particleShader_->bind_attribute_location(5, "ColorEnd");
    particleShader_->bind_attribute_location(6, "Sizes");
    particleShader_->bind_attribute_location(7, "FrameIndices");

    if (!particleShader_->link()) {
        LOG_EMERGENCY << "Error while linking particle shader:\n" << particleShader_->get_info_log().c_str() << std::endl;
        return false;
    }

    return true;
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
