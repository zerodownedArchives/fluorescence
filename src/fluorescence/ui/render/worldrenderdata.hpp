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
    void invalidateTextureProvider();
    void invalidateVertexCoordinates();
    void invalidateRenderPriority();

    bool textureProviderUpdateRequired() const;
    bool vertexCoordinatesUpdateRequired() const;
    bool renderPriorityUpdateRequired() const;

    void onTextureProviderUpdate();
    void onVertexCoordinatesUpdate();
    void onRenderPriorityUpdate();

    bool textureProviderUpdated() const;
    bool vertexCoordinatesUpdated() const;
    bool renderPriorityUpdated() const;

    void reset();

    boost::shared_ptr<ui::Texture> texture_;
    int renderPriority_[6];
    CL_Vec2f vertexCoordinates_[6];
    CL_Vec3f vertexNormals_[6];
    CL_Vec2f hueInfo_;
    bool mirrored_;

private:
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
