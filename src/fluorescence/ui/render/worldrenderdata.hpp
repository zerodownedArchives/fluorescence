#ifndef FLUO_UI_WORLDRENDERDATA_HPP
#define FLUO_UI_WORLDRENDERDATA_HPP

#include <boost/shared_ptr.hpp>

#include <ClanLib/Core/Math/vec2.h>
#include <ClanLib/Core/Math/vec3.h>
#include <ClanLib/Core/Math/rect.h>

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

    const CL_Vec3f* getVertexCoordinates() const;
    void setVertexCoordinates(unsigned int idx, float x, float y);
    void setVertexCoordinates(const CL_Rectf& rect);
    void setDepth(unsigned long long d);
    void setDepth(uint16_t xPlusY, int8_t z, uint8_t priority, uint8_t byte5, uint8_t byte6);
    unsigned long long getDepth() const;

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

    CL_Vec3f vertexCoordinates_[6];

    unsigned long long depth_;
};

}
}

#endif
