#ifndef UOME_WORLD_INGAMEOBJECT_HPP
#define UOME_WORLD_INGAMEOBJECT_HPP

#include <ClanLib/Core/Math/vec2.h>
#include <ClanLib/Core/Math/vec3.h>

#include <boost/shared_ptr.hpp>

namespace uome {

namespace ui {
    class Texture;
    class RenderQueue;
}

namespace world {

class IngameObject {

friend class ui::RenderQueue;

public:
    IngameObject();
    virtual ~IngameObject();

    int getLocX() const { return location_[0u]; }
    int getLocY() const { return location_[1u]; }
    int getLocZ() const { return location_[2u]; }

    bool isVisible() const { return visible_; }

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const = 0;

    void setLocation(int locX, int locY, int locZ);

    bool isRenderDataValid() const;
    void invalidateRenderData(bool updateTextureProvider = false);
    void updateRenderData(); ///< calls updateVertexCoordinates, updateRenderPriority and updateTexture

    const CL_Vec2f* getVertexCoordinates() const;

    int getRenderPriority(unsigned int lvl) const;
    const int* getRenderPriorities() const;

    /// returns whether or not this item is currently in the drawing area of the game window
    bool isInDrawArea(int leftPixelCoord, int rightPixelCoord, int topPixelCoord, int bottomPixelCoord) const;

protected:
    CL_Vec2f vertexCoordinates_[6];
    int renderPriority_[6];

    void addToRenderQueue();
    void removeFromRenderQueueImmediately();

    void requestUpdateTextureProvider();
    virtual void updateTextureProvider() = 0;

    virtual void updateVertexCoordinates() = 0;

    virtual void updateRenderPriority() = 0;

private:
    bool visible_;

    bool renderDataValid_; ///< whether or not the vertex positions and render priorities are correct

    CL_Vec3<int> location_;
    bool textureProviderUpdateRequired_;

    bool addedToRenderQueue_;
};

}
}

#endif
