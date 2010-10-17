#ifndef UOME_WORLD_INGAMEOBJECT_HPP
#define UOME_WORLD_INGAMEOBJECT_HPP

#include <ClanLib/Core/Math/vec3.h>

namespace uome {

namespace ui {
    class Texture;
}

namespace world {

class Sector;

class IngameObject {
public:
    IngameObject();

    int getLocX() { return location_[0]; }
    int getLocY() { return location_[1]; }
    int getLocZ() { return location_[2]; }

    bool isVisible() { return visible_; }

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() = 0;

    void setLocation(int locX, int locY, int locZ);

protected:
    CL_Vec3<int> location_;

    bool visible_;

    boost::shared_ptr<Sector> sector_;
};

}
}

#endif
