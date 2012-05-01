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
#include "xmlloadexception.hpp"
#include "timelinestatic.hpp"
#include "timelinepause.hpp"
#include "timelineimmediate.hpp"
#include "timelineblend.hpp"
#include "timelineinterpolate.hpp"
#include "timelineforever.hpp"
#include "timelineevent.hpp"

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

boost::shared_ptr<world::ParticleEffect> XmlLoader::fromFile(const UnicodeString& name) {
    boost::shared_ptr<world::ParticleEffect> effect;
    
    boost::filesystem::path path = "effects";
    std::string utf8FileName = StringConverter::toUtf8String(name) + ".xml";
    path = path / utf8FileName;
    
    path = data::Manager::getShardFilePath(path);
    
    if (!boost::filesystem::exists(path)) {
        LOG_ERROR << "Unable to open particle effect xml, file not found: " << utf8FileName << std::endl;
        return effect;
    }

    LOG_DEBUG << "Parsing xml particle effect file: " << path << std::endl;
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.string().c_str());

    if (result) {
        try {
            effect = getSingleton()->parse(doc);
        } catch (const XmlLoadException& ex) {
            LOG_ERROR << "Unable to load xml particle effect: " << ex.what() << std::endl;
        }
    } else {
        LOG_ERROR << "Error parsing particle effect xml file at offset " << result.offset << ": " << result.description() << std::endl;
    }
    return effect;
}

boost::shared_ptr<world::ParticleEffect> XmlLoader::fromString(const UnicodeString& str) {
    pugi::xml_document doc;
    std::string utf8String = StringConverter::toUtf8String(str);
    pugi::xml_parse_result result = doc.load_buffer(utf8String.c_str(), utf8String.length());

    boost::shared_ptr<world::ParticleEffect> effect;
    if (result) {
        try {
            effect = getSingleton()->parse(doc);
        } catch (const XmlLoadException& ex) {
            LOG_ERROR << "Unable to load xml particle effect: " << ex.what() << std::endl;
        }
    } else {
        LOG_ERROR << "Error parsing particle effect xml string at offset " << result.offset << ": " << result.description() << std::endl;
    }
    return effect;
}

boost::shared_ptr<world::ParticleEffect> XmlLoader::parse(pugi::xml_document& doc) const {
    pugi::xml_node statesNode = doc.document_element().child("states");
    if (!statesNode) {
        throw XmlLoadException("ParticleEffect definition requires states node");
    }
    
    std::map<UnicodeString, ParticleEmitterState> stateMap;
    
    // find default state
    pugi::xml_node defaultStateNode = statesNode.find_child_by_attribute("state", "default", "true");
    if (!defaultStateNode) {
        throw XmlLoadException("ParticleEffect definition requires a default state");
    }
    
    ParticleEmitterState pseudoDefault;
    ParticleEmitterState defaultState = parseState(defaultStateNode, pseudoDefault);
    
    // check state validity
    if (!defaultState.emittedMotionModel_) {
        throw XmlLoadException("Default state has no motion model");
    }
    if (!defaultState.emittedStartLocationProvider_) {
        throw XmlLoadException("Default state has no shape");
    }
    stateMap[defaultState.name_] = defaultState;
    
    // parse other states
    pugi::xml_node stateIter = statesNode.first_child();
    while (stateIter) {
        if (stateIter.attribute("default").as_bool()) {
            // default state was already parsed
            stateIter = stateIter.next_sibling();
            continue;
        }
        ParticleEmitterState curState = parseState(stateIter, defaultState);
        stateMap[curState.name_] = curState;
        stateIter = stateIter.next_sibling();
    }
    
    boost::shared_ptr<world::ParticleEffect> effect(new world::ParticleEffect());
    
    pugi::xml_node emittersIter = doc.document_element().child("emitters").first_child();
    while (emittersIter) {
        boost::shared_ptr<ParticleEmitter> newEmitter = parseEmitter(emittersIter, stateMap);
        effect->addEmitter(newEmitter);
        emittersIter = emittersIter.next_sibling();
    }
    
    return effect;
}

boost::shared_ptr<ParticleEmitter> XmlLoader::parseEmitter(pugi::xml_node& node, std::map<UnicodeString, ParticleEmitterState>& stateMap) const {
    checkAttribute(node, "capacity");
    int capacity = node.attribute("capacity").as_int();
    boost::shared_ptr<ParticleEmitter> emitter(new ParticleEmitter(capacity));
    
    emitter->emittedMoveWithEmitter_ = node.attribute("particlesmovewithemitter").as_bool();
    
    if (!node.child("texture")) {
        throw XmlLoadException("Emitter definition without texture");
    }
    pugi::xml_node textureNode = node.child("texture");
    checkAttribute(textureNode, "source");
    checkAttribute(textureNode, "id");
    UnicodeString textureResource(textureNode.attribute("source").value());
    UnicodeString textureId(textureNode.attribute("id").value());
    emitter->emittedTexture_ = data::Manager::getTexture(textureResource, textureId);
    
    if (!node.child("timeline")) {
        throw XmlLoadException("Emitter definition without timeline");
    }
    
    pugi::xml_node timelineNode = node.child("timeline");
    emitter->timeline_.setRepeat(timelineNode.attribute("repeat").as_int());
    pugi::xml_node timelineIter = timelineNode.first_child();
    while (timelineIter) {
        std::string tlElemName = timelineIter.name();
        if (tlElemName == "static") {
            checkAttribute(timelineIter, "state");
            checkAttribute(timelineIter, "duration");
            
            UnicodeString stateName = StringConverter::fromUtf8(timelineIter.attribute("state").value());
            std::map<UnicodeString, ParticleEmitterState>::iterator stateMapIter = stateMap.find(stateName);
            if (stateMapIter == stateMap.end()) {
                std::string msg("Unknown state in timeline::static: ");
                msg += StringConverter::toUtf8String(stateName);
                throw XmlLoadException(msg);
            }
            
            float duration = timelineIter.attribute("duration").as_float();
            boost::shared_ptr<TimelineStatic> newElem(new TimelineStatic(stateMapIter->second, duration));
            emitter->timeline_.addElement(newElem);
        } else if (tlElemName == "pause") {
            checkAttribute(timelineIter, "duration");
            float duration = timelineIter.attribute("duration").as_float();
            boost::shared_ptr<TimelinePause> newElem(new TimelinePause(duration));
            emitter->timeline_.addElement(newElem);
        } else if (tlElemName == "immediate") {
            checkAttribute(timelineIter, "state");
            checkAttribute(timelineIter, "count");
            
            UnicodeString stateName = StringConverter::fromUtf8(timelineIter.attribute("state").value());
            std::map<UnicodeString, ParticleEmitterState>::iterator stateMapIter = stateMap.find(stateName);
            if (stateMapIter == stateMap.end()) {
                std::string msg("Unknown state in timeline::immediate: ");
                msg += StringConverter::toUtf8String(stateName);
                throw XmlLoadException(msg);
            }
            
            unsigned int count = timelineIter.attribute("count").as_uint();
            boost::shared_ptr<TimelineImmediate> newElem(new TimelineImmediate(stateMapIter->second, count));
            emitter->timeline_.addElement(newElem);
        } else if (tlElemName == "blend") {
            checkAttribute(timelineIter, "from");
            checkAttribute(timelineIter, "to");
            checkAttribute(timelineIter, "duration");
            
            UnicodeString stateName1 = StringConverter::fromUtf8(timelineIter.attribute("from").value());
            std::map<UnicodeString, ParticleEmitterState>::iterator stateMapIter1 = stateMap.find(stateName1);
            if (stateMapIter1 == stateMap.end()) {
                std::string msg("Unknown from state in timeline::blend: ");
                msg += StringConverter::toUtf8String(stateName1);
                throw XmlLoadException(msg);
            }
            
            UnicodeString stateName2 = StringConverter::fromUtf8(timelineIter.attribute("to").value());
            std::map<UnicodeString, ParticleEmitterState>::iterator stateMapIter2 = stateMap.find(stateName2);
            if (stateMapIter2 == stateMap.end()) {
                std::string msg("Unknown to state in timeline::blend: ");
                msg += StringConverter::toUtf8String(stateName1);
                throw XmlLoadException(msg);
            }
            
            float duration = timelineIter.attribute("duration").as_float();
            boost::shared_ptr<TimelineBlend> newElem(new TimelineBlend(stateMapIter1->second, stateMapIter2->second, duration));
            emitter->timeline_.addElement(newElem);
        } else if (tlElemName == "interpolate") {
            checkAttribute(timelineIter, "from");
            checkAttribute(timelineIter, "to");
            checkAttribute(timelineIter, "duration");
            
            UnicodeString stateName1 = StringConverter::fromUtf8(timelineIter.attribute("from").value());
            std::map<UnicodeString, ParticleEmitterState>::iterator stateMapIter1 = stateMap.find(stateName1);
            if (stateMapIter1 == stateMap.end()) {
                std::string msg("Unknown from state in timeline::interpolate: ");
                msg += StringConverter::toUtf8String(stateName1);
                throw XmlLoadException(msg);
            }
            
            UnicodeString stateName2 = StringConverter::fromUtf8(timelineIter.attribute("to").value());
            std::map<UnicodeString, ParticleEmitterState>::iterator stateMapIter2 = stateMap.find(stateName2);
            if (stateMapIter2 == stateMap.end()) {
                std::string msg("Unknown to state in timeline::interpolate: ");
                msg += StringConverter::toUtf8String(stateName1);
                throw XmlLoadException(msg);
            }
            
            float duration = timelineIter.attribute("duration").as_float();
            boost::shared_ptr<TimelineInterpolate> newElem(new TimelineInterpolate(stateMapIter1->second, stateMapIter2->second, duration));
            emitter->timeline_.addElement(newElem);
        } else if (tlElemName == "forever") {
            checkAttribute(timelineIter, "state");
            
            UnicodeString stateName = StringConverter::fromUtf8(timelineIter.attribute("state").value());
            std::map<UnicodeString, ParticleEmitterState>::iterator stateMapIter = stateMap.find(stateName);
            if (stateMapIter == stateMap.end()) {
                std::string msg("Unknown state in timeline::static: ");
                msg += StringConverter::toUtf8String(stateName);
                throw XmlLoadException(msg);
            }
            
            boost::shared_ptr<TimelineForever> newElem(new TimelineForever(stateMapIter->second));
            emitter->timeline_.addElement(newElem);
        } else if (tlElemName == "event") {
            checkAttribute(timelineIter, "name");
            UnicodeString eventName = StringConverter::fromUtf8(timelineIter.attribute("name").value());
            boost::shared_ptr<TimelineEvent> newElem(new TimelineEvent(eventName));
            emitter->timeline_.addElement(newElem);
        } else {
            std::string msg("Unknown timeline element: ");
            msg += tlElemName;
            throw XmlLoadException(msg);
        }
        
        timelineIter = timelineIter.next_sibling();
    }
    
    return emitter;
}

ParticleEmitterState XmlLoader::parseState(pugi::xml_node& node, const ParticleEmitterState& defaultState) const {
    ParticleEmitterState state = defaultState;
    
    checkAttribute(node, "name");
    state.name_ = StringConverter::fromUtf8(node.attribute("name").value());
    
    pugi::xml_node curProp = node.child("location-offset");
    if (curProp) {
        state.emitterLocationOffset_ = CL_Vec3f(
                curProp.attribute("x").as_float(),
                curProp.attribute("y").as_float(),
                curProp.attribute("z").as_float()
        );
    }
    
    curProp = node.child("frequency");
    if (curProp) {
        checkAttribute(curProp, "value");
        state.emitFrequency_ = curProp.attribute("value").as_float();
    }
    
    curProp = node.child("shape");
    if (curProp) {
        checkAttribute(curProp, "type");
        std::string shapeType = curProp.attribute("type").value();
        
        bool shapeHasSize = true;
        if (shapeType == "oval") {
            state.emittedStartLocationProvider_.reset((StartLocationProvider*)new StartLocationProviderOval());
        } else if (shapeType == "oval-outline") {
            state.emittedStartLocationProvider_.reset((StartLocationProvider*)new StartLocationProviderOvalOutline());
        } else if (shapeType == "box") {
            state.emittedStartLocationProvider_.reset((StartLocationProvider*)new StartLocationProviderBox());
        } else if (shapeType == "box-outline") {
            state.emittedStartLocationProvider_.reset((StartLocationProvider*)new StartLocationProviderBoxOutline());
        } else if (shapeType == "point") {
            state.emittedStartLocationProvider_.reset((StartLocationProvider*)new StartLocationProviderEmitter());
            shapeHasSize = false;
        } else {
            std::string msg("Unknown shape type: ");
            msg += shapeType;
            throw XmlLoadException(msg);
        }
        
        if (shapeHasSize) {
            checkAttribute(curProp, "width");
            checkAttribute(curProp, "height");
            
            boost::shared_ptr<StartLocationProviderWithSize> startPosProv = 
                    boost::dynamic_pointer_cast<StartLocationProviderWithSize>(state.emittedStartLocationProvider_);
            if (startPosProv) {
                startPosProv->setSize(
                    curProp.attribute("width").as_float(),
                    curProp.attribute("height").as_float()
                );
            }
        }
    }
    
    curProp = node.child("motion");
    if (curProp) {
        checkAttribute(curProp, "model");
        std::string model = curProp.attribute("model").value();
        if (model == "explosion") {
            state.emittedMotionModel_.reset(new MotionModelAwayFromEmitter());
        } else if (model == "velocities") {
            state.emittedMotionModel_.reset(new MotionModelStartEndVelocity());
        } else if (model == "static") {
            state.emittedMotionModel_.reset(new MotionModelStatic());
        } else {
            std::string msg("Unknown motion model: ");
            msg += model;
            throw XmlLoadException(msg);
        }
    }
    
    pugi::xml_node particlesNode = node.child("particles");
    if (particlesNode) {
        curProp = particlesNode.child("lifetime");
        if (curProp) {
            checkAttribute(curProp, "min");
            checkAttribute(curProp, "max");
            
            state.emittedLifetime_.set(
                curProp.attribute("min").as_float(),
                curProp.attribute("max").as_float()
            );
        }
        
        pugi::xml_node t0 = particlesNode.child("t0");
        if (t0) {
            curProp = t0.child("color");
            if (curProp) {
                checkAttribute(curProp, "min");
                checkAttribute(curProp, "max");
                
                CL_Colorf colMin(curProp.attribute("min").value());
                CL_Colorf colMax(curProp.attribute("max").value());
                state.emittedColorStart_.set(colMin, colMax);
            }
            
            curProp = t0.child("size");
            if (curProp) {
                checkAttribute(curProp, "min");
                checkAttribute(curProp, "max");
                
                state.emittedSizeStart_.set(
                    curProp.attribute("min").as_float(),
                    curProp.attribute("max").as_float()
                );
            }
            
            curProp = t0.child("acceleration");
            if (curProp) {
                if (!node.child("motion")) {
                    throw XmlLoadException("Definition of acceleration property requires definition of motion model");
                }
                checkAttribute(curProp, "min");
                checkAttribute(curProp, "max");
                
                boost::shared_ptr<MotionModelAwayFromEmitter> momo = boost::dynamic_pointer_cast<MotionModelAwayFromEmitter>(state.emittedMotionModel_);
                if (momo) {
                    momo->setAccelerationStart(
                        curProp.attribute("min").as_float(),
                        curProp.attribute("max").as_float()
                    );
                } else {
                    throw XmlLoadException("MotionModel mismatch: accelerator property given, but model != explosion");
                }
            }
            
            curProp = t0.child("velocity-min");
            if (curProp) {
                if (!node.child("motion")) {
                    throw XmlLoadException("Definition of velocity properties requires definition of motion model");
                }
                CL_Vec3f velMin(
                    curProp.attribute("x").as_float(),
                    curProp.attribute("y").as_float(),
                    curProp.attribute("z").as_float()
                );
                
                curProp = t0.child("velocity-max");
                if (!curProp) {
                    throw XmlLoadException("MotionModel error: velocity-min given, but no velocity-max");
                }
                
                CL_Vec3f velMax(
                    curProp.attribute("x").as_float(),
                    curProp.attribute("y").as_float(),
                    curProp.attribute("z").as_float()
                );
                    
                boost::shared_ptr<MotionModelStartEndVelocity> momo = boost::dynamic_pointer_cast<MotionModelStartEndVelocity>(state.emittedMotionModel_);
                if (momo) {
                    momo->setVelocitiesStart(velMin, velMax);
                } else {
                    throw XmlLoadException("MotionModel mismatch: velocity-min property given, but model != velocities");
                }
            } else if (t0.child("velocity-max")) {
                throw XmlLoadException("MotionModel error: velocity-max given, but no velocity-min");
            }
        }
        
        
        pugi::xml_node t1 = particlesNode.child("t1");
        if (t1) {
            curProp = t1.child("color");
            if (curProp) {
                checkAttribute(curProp, "min");
                checkAttribute(curProp, "max");
                
                CL_Colorf colMin(curProp.attribute("min").value());
                CL_Colorf colMax(curProp.attribute("max").value());
                state.emittedColorEnd_.set(colMin, colMax);
            }
            
            curProp = t1.child("size");
            if (curProp) {
                checkAttribute(curProp, "min");
                checkAttribute(curProp, "max");
                
                state.emittedSizeEnd_.set(
                    curProp.attribute("min").as_float(),
                    curProp.attribute("max").as_float()
                );
            }
            
            curProp = t1.child("acceleration");
            if (curProp) {
                if (!node.child("motion")) {
                    throw XmlLoadException("Definition of acceleration propertiy requires definition of motion model");
                }
                checkAttribute(curProp, "min");
                checkAttribute(curProp, "max");
                
                boost::shared_ptr<MotionModelAwayFromEmitter> momo = boost::dynamic_pointer_cast<MotionModelAwayFromEmitter>(state.emittedMotionModel_);
                if (momo) {
                    momo->setAccelerationEnd(
                        curProp.attribute("min").as_float(),
                        curProp.attribute("max").as_float()
                    );
                } else {
                    throw XmlLoadException("MotionModel mismatch: accelerator property given, but model != explosion");
                }
            }
            
            curProp = t1.child("velocity-min");
            if (curProp) {
                if (!node.child("motion")) {
                    throw XmlLoadException("Definition of velocity properties requires definition of motion model");
                }
                CL_Vec3f velMin(
                    curProp.attribute("x").as_float(),
                    curProp.attribute("y").as_float(),
                    curProp.attribute("z").as_float()
                );
                
                curProp = t1.child("velocity-max");
                if (!curProp) {
                    throw XmlLoadException("MotionModel error: velocity-min given, but no velocity-max");
                }
                
                CL_Vec3f velMax(
                    curProp.attribute("x").as_float(),
                    curProp.attribute("y").as_float(),
                    curProp.attribute("z").as_float()
                );
                    
                boost::shared_ptr<MotionModelStartEndVelocity> momo = boost::dynamic_pointer_cast<MotionModelStartEndVelocity>(state.emittedMotionModel_);
                if (momo) {
                    momo->setVelocitiesEnd(velMin, velMax);
                } else {
                    throw XmlLoadException("MotionModel mismatch: velocity-min property given, but model != velocities");
                }
            } else if (t1.child("velocity-max")) {
                throw XmlLoadException("MotionModel error: velocity-max given, but no velocity-min");
            }
        }
    }
    
    return state;
}

void XmlLoader::checkAttribute(pugi::xml_node& node, const char* name) const {
    if (!node.attribute(name)) {
        std::string str("Missing attribute ");
        str += name;
        str += " in node ";
        str += node.name();
        throw XmlLoadException(str);
    }
}

}
}
}
