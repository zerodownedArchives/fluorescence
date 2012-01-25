#ifndef FLUO_UI_WORLDRENDERDATA_HPP
#define FLUO_UI_WORLDRENDERDATA_HPP

#include <stdint.h>

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
    bool renderDepthUpdateRequired() const;

    bool textureProviderUpdated() const;
    bool vertexCoordinatesUpdated() const;
    bool renderDepthUpdated() const;

    void reset();

    const CL_Vec3f* getVertexCoordinates() const;
    void setVertexCoordinates(unsigned int idx, float x, float y);
    void setVertexCoordinates(const CL_Rectf& rect);
    void setRenderDepth(unsigned long long d);
    void setRenderDepth(uint16_t xPlusY, int8_t z, uint8_t priority, uint8_t byte5, uint8_t byte6);
    unsigned long long getRenderDepth() const;

    CL_Vec3f vertexNormals_[6];
    CL_Vec3f hueInfo_;

private:
    void invalidateTextureProvider();
    void invalidateVertexCoordinates();
    void invalidateRenderDepth();

    void onTextureProviderUpdate();
    void onVertexCoordinatesUpdate();
    void onRenderDepthUpdate();

    bool textureProviderUpdateRequired_;
    bool vertexCoordinatesUpdateRequired_;
    bool renderDepthUpdateRequired_;

    bool textureProviderUpdated_;
    bool vertexCoordinatesUpdated_;
    bool renderDepthUpdated_;

    CL_Vec3f vertexCoordinates_[6];

    unsigned long long renderDepth_;
};

}
}

#endif
