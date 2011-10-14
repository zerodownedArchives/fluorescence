#ifndef FLUO_WORLD_MOBILE_HPP
#define FLUO_WORLD_MOBILE_HPP

#include <list>

#include "serverobject.hpp"

#include <misc/variable.hpp>
#include <ui/animtextureprovider.hpp>

namespace fluo {

namespace ui {
    class Texture;
    class GumpMenu;
}


namespace world {
class DynamicItem;

class Mobile : public ServerObject {
public:
    Mobile(Serial serial);

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const;

    unsigned int getBodyId() const;
    void setBodyId(unsigned int value);

    virtual void onClick();
    virtual void onDoubleClick();

    void playAnim(unsigned int animId);

    virtual bool isMirrored() const;
    void setDirection(unsigned int direction);
    unsigned int getDirection() const;

    bool hasProperty(const UnicodeString& name) const;
    Variable& getProperty(const UnicodeString& name);
    void onPropertyUpdate();

    void addLinkedGump(ui::GumpMenu* menu);
    void removeLinkedGump(ui::GumpMenu* menu);

    virtual void onStartDrag(const CL_Point& mousePos);

private:
    unsigned int bodyId_;
    unsigned int direction_;
    bool isRunning_;

    void updateVertexCoordinates();
    void updateRenderPriority();
    void updateTextureProvider();
    bool updateAnimation(unsigned int elapsedMillis);

    boost::shared_ptr<ui::AnimTextureProvider> textureProvider_;

    std::map<UnicodeString, Variable> propertyMap_;

    std::list<ui::GumpMenu*> linkedGumps_;
};

}
}

#endif
