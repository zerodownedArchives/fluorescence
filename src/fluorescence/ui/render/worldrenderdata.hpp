#ifndef FLUO_UI_WORLDRENDERDATA_HPP
#define FLUO_UI_WORLDRENDERDATA_HPP

#include <boost/shared_ptr.hpp>

#include <ClanLib/Core/Math/vec2.h>
#include <ClanLib/Core/Math/vec3.h>

namespace fluo {

namespace world {
    class IngameObject;
}

namespace ui {

class Texture;

class WorldRenderData {

friend class world::IngameObject;

public:
    WorldRenderData();

    bool renderDataValid() const;

    bool textureProviderUpdateRequired() const;
    bool vertexCoordinatesUpdateRequired() const;
    bool renderPriorityUpdateRequired() const;

    bool textureProviderUpdated() const;
    bool vertexCoordinatesUpdated() const;
    bool renderPriorityUpdated() const;

    void reset();

    int renderPriority_[6];
    CL_Vec3f vertexCoordinates_[6];
    CL_Vec3f vertexNormals_[6];
    CL_Vec3f hueInfo_;

private:
    void invalidateTextureProvider();
    void invalidateVertexCoordinates();
    void invalidateRenderPriority();

    void onTextureProviderUpdate();
    void onVertexCoordinatesUpdate();
    void onRenderPriorityUpdate();

    bool textureProviderUpdateRequired_;
    bool vertexCoordinatesUpdateRequired_;
    bool renderPriorityUpdateRequired_;

    bool textureProviderUpdated_;
    bool vertexCoordinatesUpdated_;
    bool renderPriorityUpdated_;
};

}
}

#endif
