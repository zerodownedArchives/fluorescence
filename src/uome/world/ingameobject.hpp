#ifndef UOME_WORLD_INGAMEOBJECT_HPP
#define UOME_WORLD_INGAMEOBJECT_HPP

#include <ClanLib/Core/Math/vec2.h>
#include <ClanLib/Core/Math/vec3.h>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace uome {

namespace ui {
    class Texture;
    class RenderQueue;
}

namespace world {

class IngameObject : public boost::enable_shared_from_this<IngameObject> {

friend class ui::RenderQueue;

public:
    IngameObject();
    virtual ~IngameObject();

    int getLocX() const { return location_[0u]; }
    int getLocY() const { return location_[1u]; }
    int getLocZ() const { return location_[2u]; }

    bool isVisible() const;
    void setVisible(bool visible);

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const = 0;

    void setLocation(int locX, int locY, int locZ);

    bool isRenderDataValid() const;
    void invalidateRenderData(bool updateTextureProvider = false);
    void updateRenderData(); ///< calls updateVertexCoordinates, updateRenderPriority and updateTexture

    const CL_Vec2f* getVertexCoordinates() const;

    const CL_Vec3f* getVertexNormals() const;
    void setVertexNormals(const CL_Vec3f& top, const CL_Vec3f& right, const CL_Vec3f& bottom, const CL_Vec3f& left);

    int getRenderPriority(unsigned int lvl) const;
    const int* getRenderPriorities() const;

    /// returns whether or not this item is currently in the drawing area of the game window
    virtual bool isInDrawArea(int leftPixelCoord, int rightPixelCoord, int topPixelCoord, int bottomPixelCoord) const;

    /// returns wheter or not the given pixel coordinate is covered by this object's texture
    virtual bool hasPixel(int pixelX, int pixelY) const;

protected:
    CL_Vec2f vertexCoordinates_[6];
    CL_Vec3f vertexNormals_[6];
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
