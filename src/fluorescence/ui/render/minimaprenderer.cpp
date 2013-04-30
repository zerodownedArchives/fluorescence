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



#include "minimaprenderer.hpp"

#include <ClanLib/Display/Render/program_object.h>
#include <ClanLib/Display/2D/draw.h>

#include <client.hpp>

#include <ui/manager.hpp>
#include <ui/render/shadermanager.hpp>
#include <ui/texture.hpp>
#include <ui/components/minimapview.hpp>

#include <misc/log.hpp>

#include <world/ingameobject.hpp>
#include <world/manager.hpp>
#include <world/sectormanager.hpp>
#include <world/sector.hpp>
#include <world/minimapblock.hpp>


namespace fluo {
namespace ui {
namespace render {

MiniMapRenderer::MiniMapRenderer(components::MiniMapView* minimapView) :
        miniMapView_(minimapView),
        batchFill_(0), forceRepaint_(false) {
    initBufferControls();
}

MiniMapRenderer::~MiniMapRenderer() {
}

void MiniMapRenderer::checkTextureSize(CL_GraphicContext& gc) {
    if (texture_.is_null() || texture_.get_size() != miniMapView_->get_size()) {
        texture_ = ui::Manager::getSingleton()->providerRenderBufferTexture(miniMapView_->get_size());

        frameBuffer_ = CL_FrameBuffer(gc);
        frameBuffer_.attach_color_buffer(0, texture_);
    }
}

CL_Texture MiniMapRenderer::getTexture(CL_GraphicContext& gc) {
    if (texture_.is_null() || forceRepaint_) {
        checkTextureSize(gc);
        CL_FrameBuffer origBuffer = gc.get_write_frame_buffer();

        gc.set_frame_buffer(frameBuffer_);

        render(gc);

        gc.set_frame_buffer(origBuffer);
    }

    return texture_;
}

void MiniMapRenderer::render(CL_GraphicContext& gc) {
    gc.set_buffer_control(bufferControlObjects_);

    gc.clear(CL_Colorf(0.f, 0.f, 0.f, 0.f));

    boost::shared_ptr<CL_ProgramObject> shader = ui::Manager::getShaderManager()->getGumpShader();
    gc.set_program_object(*shader, cl_program_matrix_modelview_projection);

    gc.push_modelview();
    gc.set_modelview(miniMapView_->getViewMatrix());
    gc.push_cliprect(miniMapView_->get_geometry());

    shader->set_uniform1i("ObjectTexture", 1);

    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator secIter = world::Manager::getSectorManager()->begin();
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator secEnd = world::Manager::getSectorManager()->end();

    std::set<IsoIndex> miniMapBlocksRendered;

    for (; secIter != secEnd; ++secIter) {
        // only draw sectors required by the worldview
        if (!miniMapView_->shouldDrawSector(secIter->first)) {
            continue;
        }

        // check if this block was already drawn
        boost::shared_ptr<world::MiniMapBlock> curBlock = secIter->second->getMiniMapBlock();
        if (!curBlock || miniMapBlocksRendered.count(curBlock->getTopLeftIndex())) {
            continue;
        }

        miniMapBlocksRendered.insert(curBlock->getTopLeftIndex());

        renderMiniBlock(gc, curBlock.get());

        //CL_Draw::texture(gc, curBlock->getTexture()->getTexture(), CL_Quadf(CL_Rectf(1504, 1600, CL_Sizef(32, 32))), CL_Colorf::white, curBlock->getTexture()->getNormalizedTextureCoords());
    }

    batchFlush(gc);

    gc.pop_cliprect();
    gc.pop_modelview();
}


void MiniMapRenderer::renderMiniBlock(CL_GraphicContext& gc, world::MiniMapBlock* block) {
    ui::Texture* tex = block->getTexture().get();

    if (lastTexture_ != tex->getTexture()) {
        batchFlush(gc);

        lastTexture_ = tex->getTexture();
    }

    memcpy(&batchPositions_[batchFill_], block->getVertexCoordinates(), sizeof(CL_Vec3f) * 6);

    CL_Rectf texCoordHelper = tex->getNormalizedTextureCoords();
    CL_Vec2f texCoords[6] = {
        CL_Vec2f(texCoordHelper.left, texCoordHelper.top),
        CL_Vec2f(texCoordHelper.right, texCoordHelper.top),
        CL_Vec2f(texCoordHelper.left, texCoordHelper.bottom),
        CL_Vec2f(texCoordHelper.right, texCoordHelper.top),
        CL_Vec2f(texCoordHelper.left, texCoordHelper.bottom),
        CL_Vec2f(texCoordHelper.right, texCoordHelper.bottom)
    };
    memcpy(&batchTexCoords_[batchFill_], texCoords, sizeof(CL_Vec2f) * 6);

    static CL_Vec3f hueInfo(0, 0, 1);
    for (unsigned int i = 0; i < 6; ++i) {
        batchHueInfos_[batchFill_ + i] = hueInfo;
    }

    batchFill_ += 6;

    if (batchFill_ == BATCH_NUM_VERTICES) {
        batchFlush(gc);
    }
}

void MiniMapRenderer::batchFlush(CL_GraphicContext& gc) {
    if (batchFill_ > 0 && !lastTexture_.is_null()) {
        //LOG_DEBUG << "batch flush: " << (batchFill_ / 6) << std::endl;
        CL_PrimitivesArray primarray(gc);
        primarray.set_attributes(0, batchPositions_);
        primarray.set_attributes(1, batchTexCoords_);
        primarray.set_attributes(2, batchHueInfos_);

        gc.set_texture(1, lastTexture_);

        gc.draw_primitives(cl_triangles, batchFill_, primarray);

        batchFill_ = 0;
    }
}

void MiniMapRenderer::initBufferControls() {
    bufferControlObjects_.enable_depth_write(false);
    bufferControlObjects_.enable_depth_test(false);
    bufferControlObjects_.enable_color_write(true);
    bufferControlObjects_.enable_stencil_test(false);
    bufferControlObjects_.enable_logic_op(false);
}

void MiniMapRenderer::forceRepaint() {
    forceRepaint_ = true;
}

}
}
}
