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


#ifndef FLUO_UI_WORLDRENDERDATA_HPP
#define FLUO_UI_WORLDRENDERDATA_HPP

#include <stdint.h>

#include <boost/shared_ptr.hpp>

#include <ClanLib/Core/Math/vec2.h>
#include <ClanLib/Core/Math/vec3.h>
#include <ClanLib/Core/Math/rect.h>

#include <typedefs.hpp>

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

    void resetPreUpdate();
    bool textureOrVerticesUpdated() const;
    bool renderDepthUpdated() const;

    const CL_Vec3f* getVertexCoordinates() const;
    void setVertexCoordinates(unsigned int idx, float x, float y);
    void setVertexCoordinates(const CL_Rectf& rect);
    void setRenderDepth(uint16_t xPlusY, int8_t z, uint8_t priority, uint8_t byte5, uint8_t byte6);
    const RenderDepth& getRenderDepth() const;

    CL_Vec3f vertexNormals_[6];
    CL_Vec3f hueInfo_;
    
    CL_Rectf previousVertexRect_;
    CL_Rectf getCurrentVertexRect() const;

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
    
    bool renderDepthUpdated_;
    bool textureOrVerticesUpdated_;

    CL_Vec3f vertexCoordinates_[6];

    RenderDepth renderDepth_;
};

}
}

#endif
