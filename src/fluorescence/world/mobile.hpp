#ifndef FLUO_WORLD_MOBILE_HPP
#define FLUO_WORLD_MOBILE_HPP

#include <list>
#include <map>

#include "serverobject.hpp"

#include <misc/variable.hpp>

namespace fluo {

namespace ui {
    class Texture;
    class GumpMenu;
    class AnimTextureProvider;
    class SingleTextureProvider;
}


namespace world {
class DynamicItem;

class Mobile : public ServerObject {
public:
    Mobile(Serial serial);

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const;
    virtual boost::shared_ptr<ui::Texture> getGumpTexture() const;

    unsigned int getBodyId() const;
    void setBodyId(unsigned int value);

    void setRace(unsigned int race);
    unsigned int getRace() const;

    void setGender(unsigned int gender);
    unsigned int isFemale() const;

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

    void openPaperdoll();

    bool isPlayer() const;

    virtual void onDelete();

private:
    unsigned int baseBodyId_; // as sent by the server
    unsigned int bodyId_; // after transformation, e.g. by body.def
    unsigned int direction_;
    bool isRunning_;

    void updateVertexCoordinates();
    void updateRenderPriority();
    void updateTextureProvider();
    bool updateAnimation(unsigned int elapsedMillis);
    void updateGumpTextureProvider();

    boost::shared_ptr<ui::AnimTextureProvider> textureProvider_;
    boost::shared_ptr<ui::SingleTextureProvider> gumpTextureProvider_;

    std::map<UnicodeString, Variable> propertyMap_;

    std::list<ui::GumpMenu*> linkedGumps_;
    ui::GumpMenu* findOrCreateLinkedGump(const UnicodeString& gumpName);

    unsigned int race_;
    bool female_;
};

}
}

#endif
