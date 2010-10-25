#ifndef UOME_WORLD_INGAMEOBJECT_HPP
#define UOME_WORLD_INGAMEOBJECT_HPP

#include <ui/texture.hpp>

#include <ClanLib/Core/Math/vec3.h>

namespace uome {
namespace world {

class Sector;

class IngameObject {
public:
    IngameObject();

    int getLocX() { return location_[0u]; }
    int getLocY() { return location_[1u]; }
    int getLocZ() { return location_[2u]; }

    bool isVisible() { return visible_; }

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const = 0;

    void setLocation(int locX, int locY, int locZ);

protected:
    CL_Vec3<int> location_;

    bool visible_;

    boost::shared_ptr<Sector> sector_;
};

}
}

#endif
