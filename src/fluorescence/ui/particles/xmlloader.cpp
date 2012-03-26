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



#include "xmlloader.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <data/manager.hpp>
#include <world/particleeffect.hpp>

#include "particleemitter.hpp"
#include "startlocationprovider.hpp"
#include "motionmodel.hpp"

namespace fluo {
namespace ui {
namespace particles {
    
XmlLoader* XmlLoader::singleton_ = NULL;
XmlLoader* XmlLoader::getSingleton() {
    if (!singleton_) {
        singleton_ = new XmlLoader();
    }
    
    return singleton_;
}

XmlLoader::XmlLoader() {
}

bool XmlLoader::fromFile(const UnicodeString& name, world::ParticleEffect* effect) {
    boost::filesystem::path path = "effects";
    std::string utf8FileName = StringConverter::toUtf8String(name) + ".xml";
    path = path / utf8FileName;
    
    path = data::Manager::getShardFilePath(path);
    
    if (!boost::filesystem::exists(path)) {
        LOG_ERROR << "Unable to particle effect xml, file not found: " << utf8FileName << std::endl;
        return false;
    }

    LOG_DEBUG << "Parsing xml particle effect file: " << path << std::endl;
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.string().c_str());

    if (result) {
        return getSingleton()->parse(doc, effect);
    } else {
        LOG_ERROR << "Error parsing particle effect xml file at offset " << result.offset << ": " << result.description() << std::endl;
        return false;
    }
}

bool XmlLoader::fromString(const UnicodeString& str, world::ParticleEffect* effect) {
    pugi::xml_document doc;
    std::string utf8String = StringConverter::toUtf8String(str);
    pugi::xml_parse_result result = doc.load_buffer(utf8String.c_str(), utf8String.length());

    if (result) {
        return getSingleton()->parse(doc, effect);
    } else {
        LOG_ERROR << "Error parsing particle effect xml string at offset " << result.offset << ": " << result.description() << std::endl;
        return false;
    }
}

bool XmlLoader::parse(pugi::xml_document& doc, world::ParticleEffect* effect) const {
    pugi::xml_node iter = doc.document_element().first_child();
    while (iter) {
        effect->addEmitter(parseEmitter(iter));
        iter = iter.next_sibling();
    }
    
    return true;
}

boost::shared_ptr<ParticleEmitter> XmlLoader::parseEmitter(pugi::xml_node& node) const {
    int sizeMax = node.child("size").attribute("max").as_int();
    boost::shared_ptr<ParticleEmitter> ret(new ParticleEmitter(sizeMax));
    
    int sizeInit = node.child("size").attribute("init").as_int();    
    ret->emittedStartCount_ = sizeInit;
    
    ret->lifetimes_[1u] = node.child("lifetime").attribute("duration").as_float();
    
    pugi::xml_node particlesCommon = node.child("particles");
    bool moveWithEmitter = particlesCommon.child("movewithemitter").attribute("value").as_bool();
    ret->emittedMoveWithEmitter_ = moveWithEmitter;
    
    UnicodeString textureResource(particlesCommon.child("texture").attribute("source").value());
    UnicodeString textureId(particlesCommon.child("texture").attribute("id").value());
    ret->emittedTexture_ = data::Manager::getTexture(textureResource, textureId);
    
    std::string motionModel = particlesCommon.child("motion").attribute("model").value();
    bool motionModelVelocities = false;
    bool motionModelExplosion = false;
    if (motionModel == "static") {
        ret->emittedMotionModel_.reset((MotionModel*)new MotionModelStatic());
    } else if (motionModel == "explosion") {
        ret->emittedMotionModel_.reset((MotionModel*)new MotionModelAwayFromEmitter());
        motionModelExplosion = true;
    } else if (motionModel == "velocities") {
        ret->emittedMotionModel_.reset((MotionModel*)new MotionModelStartEndVelocity());
        motionModelVelocities = true;
    } else {
        LOG_WARN << "Unknown motion model for particles \"" << motionModel << "\", assuming static" << std::endl;
        ret->emittedMotionModel_.reset((MotionModel*)new MotionModelStatic());
    }
    
    
    std::string shapeType = node.child("shape").attribute("type").value();
    bool shapeHasSize = true;
    if (shapeType == "oval") {
        ret->emittedStartLocationProvider_.reset((StartLocationProvider*)new StartLocationProviderOval());
    } else if (shapeType == "oval-outline") {
        ret->emittedStartLocationProvider_.reset((StartLocationProvider*)new StartLocationProviderOvalOutline());
    } else if (shapeType == "box") {
        ret->emittedStartLocationProvider_.reset((StartLocationProvider*)new StartLocationProviderBox());
    } else if (shapeType == "box-outline") {
        ret->emittedStartLocationProvider_.reset((StartLocationProvider*)new StartLocationProviderBoxOutline());
    } else if (shapeType == "point") {
        ret->emittedStartLocationProvider_.reset((StartLocationProvider*)new StartLocationProviderEmitter());
        shapeHasSize = false;
    } else {
        LOG_WARN << "Unknown particle emitter shape \"" << shapeType << "\", assuming point" << std::endl;
        ret->emittedStartLocationProvider_.reset((StartLocationProvider*)new StartLocationProviderEmitter());
        shapeHasSize = false;
    }
    
    
    // find t0 values, copy also to t1
    pugi::xml_node emitterT0 = node.child("t0");
    if (emitterT0) {
        float lifetimeMin, lifetimeMax, frequency;
        CL_Vec4f colorStartMin, colorStartMax, colorEndMin, colorEndMax;
        int shapeWidth, shapeHeight;
        CL_Vec3f velocityStartMin, velocityStartMax, velocityEndMin, velocityEndMax;
        float accelerationStartMin, accelerationStartMax, accelerationEndMin, accelerationEndMax;
        
        parseEmitterTxNode(emitterT0, lifetimeMin, lifetimeMax, colorStartMin, colorStartMax, colorEndMin, colorEndMax, frequency, 
                shapeHasSize, shapeWidth, shapeHeight,
                velocityStartMin, velocityStartMax, velocityEndMin, velocityEndMax, 
                accelerationStartMin, accelerationStartMax, accelerationEndMin, accelerationEndMax);
        
        ret->emittedLifetimeMin_.set(lifetimeMin, lifetimeMin);
        ret->emittedLifetimeMax_.set(lifetimeMax, lifetimeMax);
        
        ret->emittedColorStartMin_.set(colorStartMin, colorStartMin);
        ret->emittedColorStartMax_.set(colorStartMax, colorStartMax);
        ret->emittedColorEndMin_.set(colorEndMin, colorEndMin);
        ret->emittedColorEndMax_.set(colorEndMax, colorEndMax);
        
        ret->emitPerSecond_.set(frequency, frequency);
        
        if (motionModelExplosion) {
            boost::shared_ptr<MotionModelAwayFromEmitter> motionMod = boost::static_pointer_cast<MotionModelAwayFromEmitter>(ret->emittedMotionModel_);
            motionMod->setAccelerationT0(accelerationStartMin, accelerationStartMax, accelerationEndMin, accelerationEndMax);
        } else if (motionModelVelocities) {
            boost::shared_ptr<MotionModelStartEndVelocity> motionMod = boost::static_pointer_cast<MotionModelStartEndVelocity>(ret->emittedMotionModel_);
            motionMod->setVelocitiesT0(velocityStartMin, velocityStartMax, velocityEndMin, velocityEndMax);
        }
        
        if (shapeHasSize) {
            boost::shared_ptr<StartLocationProviderWithSize> startPosProv = 
                    boost::dynamic_pointer_cast<StartLocationProviderWithSize>(ret->emittedStartLocationProvider_);
            if (startPosProv) {
                startPosProv->setSize(shapeWidth, shapeWidth, shapeHeight, shapeHeight);
            }
        }
    }
    
    // find T1 values, overwrite
    pugi::xml_node emitterT1 = node.child("t1");
    if (emitterT1) {
        float lifetimeMin, lifetimeMax, frequency;
        CL_Vec4f colorStartMin, colorStartMax, colorEndMin, colorEndMax;
        int shapeWidth, shapeHeight;
        CL_Vec3f velocityStartMin, velocityStartMax, velocityEndMin, velocityEndMax;
        float accelerationStartMin, accelerationStartMax, accelerationEndMin, accelerationEndMax;
        
        parseEmitterTxNode(emitterT1, lifetimeMin, lifetimeMax, colorStartMin, colorStartMax, colorEndMin, colorEndMax, frequency, 
                shapeHasSize, shapeWidth, shapeHeight,
                velocityStartMin, velocityStartMax, velocityEndMin, velocityEndMax, 
                accelerationStartMin, accelerationStartMax, accelerationEndMin, accelerationEndMax);
        
        ret->emittedLifetimeMin_.setT1(lifetimeMin);
        ret->emittedLifetimeMax_.setT1(lifetimeMax);
        
        ret->emittedColorStartMin_.setT1(colorStartMin);
        ret->emittedColorStartMax_.setT1(colorStartMax);
        ret->emittedColorEndMin_.setT1(colorEndMin);
        ret->emittedColorEndMax_.setT1(colorEndMax);
        
        ret->emitPerSecond_.setT1(frequency);
        
        if (motionModelExplosion) {
            boost::shared_ptr<MotionModelAwayFromEmitter> motionMod = boost::static_pointer_cast<MotionModelAwayFromEmitter>(ret->emittedMotionModel_);
            motionMod->setAccelerationT1(accelerationStartMin, accelerationStartMax, accelerationEndMin, accelerationEndMax);
        } else if (motionModelVelocities) {
            boost::shared_ptr<MotionModelStartEndVelocity> motionMod = boost::static_pointer_cast<MotionModelStartEndVelocity>(ret->emittedMotionModel_);
            motionMod->setVelocitiesT1(velocityStartMin, velocityStartMax, velocityEndMin, velocityEndMax);
        }
        
        if (shapeHasSize) {
            boost::shared_ptr<StartLocationProviderWithSize> startPosProv = 
                    boost::dynamic_pointer_cast<StartLocationProviderWithSize>(ret->emittedStartLocationProvider_);
            if (startPosProv) {
                startPosProv->setSizeT1(shapeWidth, shapeHeight);
            }
        }
    }
    
    return ret;
}

CL_Vec4f XmlLoader::parseColor(pugi::xml_node node) const {
    if (!node) {
        return CL_Vec4f(0, 0, 0, 1);
    }
    
    CL_Vec4f ret;
    
    ret.r = node.attribute("r").as_float();
    ret.g = node.attribute("g").as_float();
    ret.b = node.attribute("b").as_float();
    ret.a = node.attribute("a").as_float();
    
    return ret;
}

CL_Vec3f XmlLoader::parseVelocity(pugi::xml_node node) const {
    if (!node) {
        return CL_Vec3f(0, 0, 0);
    }
    
    CL_Vec3f ret;
    
    ret.x = node.attribute("x").as_float();
    ret.y = node.attribute("y").as_float();
    ret.z = node.attribute("z").as_float();
    
    return ret;
}

void XmlLoader::parseEmitterTxNode(pugi::xml_node& node, float& lifetimeMin, float& lifetimeMax, 
        CL_Vec4f& colorStartMin, CL_Vec4f& colorStartMax, CL_Vec4f& colorEndMin, CL_Vec4f& colorEndMax, float& frequency, 
        bool shapeHasSize, int& shapeWidth, int& shapeHeight,
        CL_Vec3f& velocityStartMin, CL_Vec3f& velocityStartMax, CL_Vec3f& velocityEndMin, CL_Vec3f& velocityEndMax,
        float& accelerationStartMin, float& accelerationStartMax, float& accelerationEndMin, float& accelerationEndMax) const {
            
    pugi::xml_node particlesNode = node.child("particles");
    lifetimeMin = particlesNode.child("lifetime").attribute("min").as_float();
    lifetimeMax = particlesNode.child("lifetime").attribute("max").as_float();
    
    pugi::xml_node particlesT0 = particlesNode.child("t0");
    colorStartMin = parseColor(particlesT0.child("colormin"));
    colorStartMax = parseColor(particlesT0.child("colormax"));
    velocityStartMin = parseVelocity(particlesT0.child("velocitymin"));
    velocityStartMax = parseVelocity(particlesT0.child("velocitymax"));
    accelerationStartMin = particlesT0.child("acceleration").attribute("min").as_float();
    accelerationStartMax = particlesT0.child("acceleration").attribute("max").as_float();
    
    pugi::xml_node particlesT1 = particlesNode.child("t1");
    colorEndMin = parseColor(particlesT1.child("colormin"));
    colorEndMax = parseColor(particlesT1.child("colormax"));
    velocityEndMin = parseVelocity(particlesT1.child("velocitymin"));
    velocityEndMax = parseVelocity(particlesT1.child("velocitymax"));
    accelerationEndMin = particlesT1.child("acceleration").attribute("min").as_float();
    accelerationEndMax = particlesT1.child("acceleration").attribute("max").as_float();
    
    frequency = node.child("frequency").attribute("value").as_int();
    
    if (shapeHasSize) {
        shapeWidth = node.child("shape").attribute("width").as_int();
        shapeHeight = node.child("shape").attribute("height").as_int();
    }
}

}
}
}
