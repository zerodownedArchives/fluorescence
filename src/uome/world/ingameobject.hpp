#ifndef UOME_WORLD_INGAMEOBJECT_HPP
#define UOME_WORLD_INGAMEOBJECT_HPP

#include <ui/texture.hpp>

#include <ClanLib/Core/Math/vec2.h>
#include <ClanLib/Core/Math/vec3.h>

namespace uome {
namespace world {

class Sector;

class IngameObject {
public:
    IngameObject();

    int getLocX() const { return location_[0u]; }
    int getLocY() const { return location_[1u]; }
    int getLocZ() const { return location_[2u]; }

    bool isVisible() const { return visible_; }

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const = 0;

    void setLocation(int locX, int locY, int locZ);

    bool isRenderDataValid() const;
    void invalidateRenderData();
    void updateRenderData(); ///< calls updateVertexCoordinates and updateRenderPriority

    const CL_Vec2f* getVertexCoordinates() const;
    virtual void updateVertexCoordinates() = 0;

    int getRenderPriority(unsigned int lvl) const;
    virtual void updateRenderPriority() = 0;

protected:
    CL_Vec3<int> location_;

    bool visible_;

    boost::shared_ptr<Sector> sector_;

    bool renderDataValid_; ///< wheter or not the vertex positions and render priorities are correct
    CL_Vec2f vertexCoordinates_[6];
    int renderPriority_[6];
};

}
}

#endif
