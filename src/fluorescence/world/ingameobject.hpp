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


#ifndef FLUO_WORLD_INGAMEOBJECT_HPP
#define FLUO_WORLD_INGAMEOBJECT_HPP

#include <ClanLib/Core/Math/vec2.h>
#include <ClanLib/Core/Math/vec3.h>
#include <ClanLib/Core/Math/point.h>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <list>

#include <ui/render/worldrenderdata.hpp>

namespace fluo {

namespace ui {
    class Texture;
    class RenderQueue;
}

namespace world {

class IngameObject : public boost::enable_shared_from_this<IngameObject> {

friend class ui::RenderQueue;

public:
    enum {
        TYPE_MAP,
        TYPE_STATIC_ITEM,
        TYPE_DYNAMIC_ITEM,
        TYPE_MOBILE,
        TYPE_SPEECH,
        TYPE_PARTICLE_EFFECT,
    };

    IngameObject(unsigned int objectType);
    virtual ~IngameObject();

    float getLocX() const;
    float getLocY() const;
    float getLocZ() const;
    CL_Vec3f getLocation() const;
    void setLocation(float locX, float locY, float locZ);
    void setLocation(CL_Vec3f loc);

    bool isVisible() const;
    void setVisible(bool visible);

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const = 0;
    virtual boost::shared_ptr<ui::Texture> getGumpTexture() const;

    void updateRenderData(unsigned int elapsedMillis); ///< calls updateVertexCoordinates, updateRenderDepth, updateTextureProvider and updateAnimation

    const CL_Vec3f* getVertexCoordinates() const;

    const CL_Vec3f* getVertexNormals() const;
    void setVertexNormals(const CL_Vec3f& top, const CL_Vec3f& right, const CL_Vec3f& bottom, const CL_Vec3f& left);

    const CL_Vec3f& getHueInfo() const;

    /// returns whether or not this item is currently in the drawing area of the game window
    virtual bool isInDrawArea(int leftPixelCoord, int rightPixelCoord, int topPixelCoord, int bottomPixelCoord) const;

    /// returns wheter or not the given pixel coordinate is covered by this object's texture
    virtual bool hasPixel(int pixelX, int pixelY) const;
    virtual bool hasGumpPixel(int pixelX, int pixelY) const;

    virtual bool isMirrored() const;

    bool isDraggable() const;
    virtual void onDraggedOnto(boost::shared_ptr<IngameObject> obj);
    virtual void onDraggedToVoid();
    virtual void onStartDrag(const CL_Point& mousePos);

    virtual void onClick();
    virtual void onDoubleClick();

    virtual boost::shared_ptr<IngameObject> getTopParent();

    void setOverheadMessageOffsets();

    void addToRenderQueue(const boost::shared_ptr<ui::RenderQueue>& rq);
    void removeFromRenderQueue(const boost::shared_ptr<ui::RenderQueue>& rq);
    bool isInRenderQueue(const boost::shared_ptr<ui::RenderQueue>& rq);
    void removeFromAllRenderQueues();

    void addChildObject(boost::shared_ptr<IngameObject> obj);
    void removeChildObject(boost::shared_ptr<IngameObject> obj);

    virtual void onAddedToParent();
    virtual void onRemovedFromParent();
    virtual void onChildObjectAdded(boost::shared_ptr<IngameObject> obj);
    virtual void onChildObjectRemoved(boost::shared_ptr<IngameObject> obj);

    virtual void onDelete();


    bool isMap() const;
    bool isStaticItem() const;
    bool isDynamicItem() const;
    bool isMobile() const;
    bool isSpeech() const;
    bool isParticleEffect() const;

    const ui::WorldRenderData& getWorldRenderData() const;

    void invalidateTextureProvider();
    void invalidateVertexCoordinates();
    void invalidateRenderDepth();

    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator rqBegin();
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator rqEnd();

    const RenderDepth& getRenderDepth() const;

    void printRenderDepth() const;
    
    bool renderDepthChanged() const;
    bool textureOrVerticesChanged() const;

protected:
    ui::WorldRenderData worldRenderData_;

    virtual void updateTextureProvider() = 0;
    virtual bool updateAnimation(unsigned int elapsedMillis) = 0;
    virtual void updateVertexCoordinates() = 0;
    virtual void updateRenderDepth() = 0;


    virtual void updateGumpTextureProvider();

    bool draggable_;

    boost::weak_ptr<IngameObject> parentObject_;
    std::list<boost::shared_ptr<IngameObject> > childObjects_;

    void forceRepaint();

private:
    unsigned int objectType_;
    bool visible_;
    CL_Vec3f location_;

    std::list<boost::shared_ptr<ui::RenderQueue> > renderQueues_;

    void notifyRenderQueuesWorldTexture();
    void notifyRenderQueuesWorldCoordinates();
    void notifyRenderQueuesWorldDepth();
    void notifyRenderQueuesGump();

    void setParentObject();
    void setParentObject(boost::shared_ptr<IngameObject> parent);
};

}
}

#endif
