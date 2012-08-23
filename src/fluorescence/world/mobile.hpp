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
    unsigned int getBaseBodyId() const;
    void setBodyId(unsigned int value);

    void setRace(unsigned int race);
    unsigned int getRace() const;

    void setGender(unsigned int gender);
    unsigned int isFemale() const;

    virtual void onClick();
    virtual void onDoubleClick();

    void animate(unsigned int animId, unsigned int delay = 1, unsigned int repeatType = AnimRepeatMode::DEFAULT);
    void stopAnim();
    unsigned int getMoveAnim() const;
    unsigned int getIdleAnim() const;
    void updateIdleAnim();
    unsigned int getMovementDuration() const;

    virtual bool isMirrored() const;
    void setDirection(unsigned int direction);
    unsigned int getDirection() const;
    bool isRunning() const;

    bool hasProperty(const UnicodeString& name) const;
    Variable& getProperty(const UnicodeString& name);
    void onPropertyUpdate();

    void addLinkedGump(ui::GumpMenu* menu);
    void removeLinkedGump(ui::GumpMenu* menu);

    virtual void onStartDrag(const CL_Point& mousePos);

    void openPaperdoll();

    virtual void onDelete();

    virtual void onChildObjectAdded(boost::shared_ptr<IngameObject> obj);
    virtual void onBeforeChildObjectRemoved(boost::shared_ptr<IngameObject> obj);
    virtual void onAfterChildObjectRemoved();

    bool isPlayer() const;
    bool isMounted() const;
    bool isWarMode() const;

    void setWarMode(bool warMode);

    virtual void onAddedToSector(world::Sector* sector);
    virtual void onRemovedFromSector(world::Sector* sector);

    bool hasItemOnLayer(unsigned int layer) const;

    void openStatusGump(const CL_Point& mousePos);
    void openProfile();
    void openSkillsGump();

    void setStatusFlags(uint8_t flags);

    void displayStatChange(unsigned int str, unsigned int dex, unsigned int intel);

    virtual void onLocationChanged(const CL_Vec3f& oldLocation);

    // move the mobile to the given location.
    // depending on the difference to the current location, this is either done with smooth movement or a teleport
    void moveTo(unsigned int locX, unsigned int locY, int locZ, unsigned int direction);

private:
    unsigned int baseBodyId_; // as sent by the server
    unsigned int bodyId_; // after transformation, e.g. by body.def
    unsigned int direction_;
    bool isRunning_;

    void updateVertexCoordinates();
    void updateRenderDepth();
    void updateTextureProvider();
    bool updateAnimation(unsigned int elapsedMillis);
    void updateGumpTextureProvider();

    boost::shared_ptr<ui::AnimTextureProvider> textureProvider_;
    boost::shared_ptr<ui::SingleTextureProvider> gumpTextureProvider_;

    std::map<UnicodeString, Variable> propertyMap_;

    std::list<ui::GumpMenu*> linkedGumps_;
    ui::GumpMenu* findLinkedGump(const UnicodeString& gumpName);
    ui::GumpMenu* findOrCreateLinkedGump(const UnicodeString& gumpName);

    unsigned int race_;
    bool female_;

    bool isWarMode_;

    unsigned int animType_; // high detail, low detail, people

    // these are used to display the halted animations for mobiles != player
    void haltAnimationCallback();
    void resumeAnimationCallback();
};

}
}

#endif
