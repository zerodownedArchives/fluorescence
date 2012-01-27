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


#ifndef FLUO_UI_PARTICLES_XMLLOADER_HPP
#define FLUO_UI_PARTICLES_XMLLOADER_HPP

#include <boost/shared_ptr.hpp>
#include <misc/pugixml/pugixml.hpp>
#include <ClanLib/Core/Math/vec4.h>

#include <misc/string.hpp>

namespace fluo {

namespace world {
class ParticleEffect;
}

namespace ui {
namespace particles {
    
class ParticleEmitter;

class XmlLoader {
public:
    static boost::shared_ptr<world::ParticleEffect> fromFile(const UnicodeString& name);
    static boost::shared_ptr<world::ParticleEffect> fromString(const UnicodeString& str);
    
private:
    static XmlLoader* singleton_;
    static XmlLoader* getSingleton();
    XmlLoader();
    XmlLoader(const XmlLoader& l) { }
    XmlLoader& operator=(const XmlLoader& l) { return *this; }
    
    boost::shared_ptr<world::ParticleEffect> parse(pugi::xml_document& doc) const;
    
    boost::shared_ptr<ParticleEmitter> parseEmitter(pugi::xml_node& node) const;
    
    CL_Vec4f parseColor(pugi::xml_node node) const;
    CL_Vec3f parseVelocity(pugi::xml_node node) const;
    void parseEmitterTxNode(pugi::xml_node& node, float& lifetimeMin, float& lifetimeMax, 
        CL_Vec4f& colorStartMin, CL_Vec4f& colorStartMax, CL_Vec4f& colorEndMin, CL_Vec4f& colorEndMax, float& frequency,
        bool shapeHasSize, int& shapeWidth, int& shapeHeight,
        CL_Vec3f& velocityStartMin, CL_Vec3f& velocityStartMax, CL_Vec3f& velocityEndMin, CL_Vec3f& velocityEndMax,
        float& accelerationStartMin, float& accelerationStartMax, float& accelerationEndMin, float& accelerationEndMax) const;
};

}
}
}

#endif
