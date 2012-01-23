
#include "xmlloader.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <data/manager.hpp>
#include <world/particleeffect.hpp>

#include "particleemitter.hpp"
#include "startpositionprovider.hpp"
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

boost::shared_ptr<world::ParticleEffect> XmlLoader::fromFile(const UnicodeString& name) {
    boost::filesystem::path path = "effects";
    std::string utf8FileName = StringConverter::toUtf8String(name) + ".xml";
    path = path / utf8FileName;
    
    path = data::Manager::getShardFilePath(path);
    
    if (!boost::filesystem::exists(path)) {
        LOG_ERROR << "Unable to particle effect xml, file not found: " << utf8FileName << std::endl;
        boost::shared_ptr<world::ParticleEffect> ret;
        return ret;
    }

    LOG_DEBUG << "Parsing xml particle effect file: " << path << std::endl;
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.string().c_str());

    if (result) {
        return getSingleton()->parse(doc);
    } else {
        LOG_ERROR << "Error parsing particle effect xml file at offset " << result.offset << ": " << result.description() << std::endl;
        boost::shared_ptr<world::ParticleEffect> ret;
        return ret;
    }
}

boost::shared_ptr<world::ParticleEffect> XmlLoader::fromString(const UnicodeString& str) {
    pugi::xml_document doc;
    std::string utf8String = StringConverter::toUtf8String(str);
    pugi::xml_parse_result result = doc.load_buffer(utf8String.c_str(), utf8String.length());

    if (result) {
        return getSingleton()->parse(doc);
    } else {
        LOG_ERROR << "Error parsing particle effect xml string at offset " << result.offset << ": " << result.description() << std::endl;
        boost::shared_ptr<world::ParticleEffect> ret;
        return ret;
    }
}

boost::shared_ptr<world::ParticleEffect> XmlLoader::parse(pugi::xml_document& doc) const {
    boost::shared_ptr<world::ParticleEffect> ret(new world::ParticleEffect());;
    
    pugi::xml_node iter = doc.document_element().first_child();
    while (iter) {
        ret->addEmitter(parseEmitter(iter));
        iter = iter.next_sibling();
    }
    
    return ret;
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
        ret->emittedStartPositionProvider_.reset((StartPositionProvider*)new StartPositionProviderOval());
    } else if (shapeType == "oval-outline") {
        ret->emittedStartPositionProvider_.reset((StartPositionProvider*)new StartPositionProviderOvalOutline());
    } else if (shapeType == "box") {
        ret->emittedStartPositionProvider_.reset((StartPositionProvider*)new StartPositionProviderBox());
    } else if (shapeType == "box-outline") {
        ret->emittedStartPositionProvider_.reset((StartPositionProvider*)new StartPositionProviderBoxOutline());
    } else if (shapeType == "point") {
        ret->emittedStartPositionProvider_.reset((StartPositionProvider*)new StartPositionProviderEmitter());
        shapeHasSize = false;
    } else {
        LOG_WARN << "Unknown particle emitter shape \"" << shapeType << "\", assuming point" << std::endl;
        ret->emittedStartPositionProvider_.reset((StartPositionProvider*)new StartPositionProviderEmitter());
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
            boost::shared_ptr<MotionModelAwayFromEmitter> motionMod = boost::dynamic_pointer_cast<MotionModelAwayFromEmitter>(ret->emittedMotionModel_);
            motionMod->setAccelerationT0(accelerationStartMin, accelerationStartMax, accelerationEndMin, accelerationEndMax);
        } else if (motionModelVelocities) {
            boost::shared_ptr<MotionModelStartEndVelocity> motionMod = boost::dynamic_pointer_cast<MotionModelStartEndVelocity>(ret->emittedMotionModel_);
            motionMod->setVelocitiesT0(velocityStartMin, velocityStartMax, velocityEndMin, velocityEndMax);
        }
        
        if (shapeHasSize) {
            boost::shared_ptr<StartPositionProviderWithSize> startPosProv = 
                    boost::dynamic_pointer_cast<StartPositionProviderWithSize>(ret->emittedStartPositionProvider_);
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
            boost::shared_ptr<MotionModelAwayFromEmitter> motionMod = boost::dynamic_pointer_cast<MotionModelAwayFromEmitter>(ret->emittedMotionModel_);
            motionMod->setAccelerationT1(accelerationStartMin, accelerationStartMax, accelerationEndMin, accelerationEndMax);
        } else if (motionModelVelocities) {
            boost::shared_ptr<MotionModelStartEndVelocity> motionMod = boost::dynamic_pointer_cast<MotionModelStartEndVelocity>(ret->emittedMotionModel_);
            motionMod->setVelocitiesT1(velocityStartMin, velocityStartMax, velocityEndMin, velocityEndMax);
        }
        
        if (shapeHasSize) {
            boost::shared_ptr<StartPositionProviderWithSize> startPosProv = 
                    boost::dynamic_pointer_cast<StartPositionProviderWithSize>(ret->emittedStartPositionProvider_);
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
    
    pugi::xml_attribute r = node.attribute("r");
    pugi::xml_attribute g = node.attribute("g");
    pugi::xml_attribute b = node.attribute("b");
    pugi::xml_attribute a = node.attribute("a");
    
    ret.r = r ? r.as_float() : 0;
    ret.g = g ? g.as_float() : 0;
    ret.b = b ? b.as_float() : 0;
    ret.a = a ? a.as_float() : 0;
    
    return ret;
}

CL_Vec3f XmlLoader::parseVelocity(pugi::xml_node node) const {
    if (!node) {
        return CL_Vec3f(0, 0, 0);
    }
    
    CL_Vec3f ret;
    
    pugi::xml_attribute x = node.attribute("x");
    pugi::xml_attribute y = node.attribute("y");
    pugi::xml_attribute z = node.attribute("z");
    
    ret.x = x ? x.as_float() : 0;
    ret.y = y ? y.as_float() : 0;
    ret.z = z ? z.as_float() : 0;
    
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
