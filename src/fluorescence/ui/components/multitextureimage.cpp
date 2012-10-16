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

#include "multitextureimage.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace components {

MultiTextureImage::MultiTextureImage(CL_GUIComponent* parent, unsigned int numTextures) : Image(parent), NUM_TEXTURES(numTextures) {
    allTextures_ = new boost::shared_ptr<ui::Texture>[NUM_TEXTURES];
    allColors_ = new CL_Colorf[NUM_TEXTURES];
    allHueInfos_ = new CL_Vec3f[NUM_TEXTURES];
    allTiled_ = new unsigned int[NUM_TEXTURES];

    for (unsigned int i = 0; i < NUM_TEXTURES; ++i) {
        allColors_[i] = CL_Colorf::white;
        allHueInfos_[i] = CL_Vec3f(0, 0, 1);
        allTiled_[i] = false;
    }
}

MultiTextureImage::~MultiTextureImage() {
    delete [] allTextures_;
    delete [] allColors_;
    delete [] allHueInfos_;
    delete [] allTiled_;
}

void MultiTextureImage::addTexture(unsigned int index, boost::shared_ptr<ui::Texture> texture, unsigned int hue, const std::string& rgba, float alpha, bool tiled) {
    if (index >= NUM_TEXTURES) {
        LOG_ERROR << "Trying to add invalid texture id in MultiTextureImage, id=" << index << std::endl;
        return;
    }

    allTextures_[index] = texture;

    if (rgba.length() > 0) {
        allColors_[index] = CL_Colorf(rgba);
    }

    allHueInfos_[index][1u] = hue;
    if (alpha) {
        allHueInfos_[index][2u] = alpha;
    }

    allTiled_[index] = tiled;

    if (index == 0) {
        // set default properties for all
        for (unsigned int i = 1; i < NUM_TEXTURES; ++i) {
            addTexture(i, texture, hue, rgba, alpha, tiled);
        }
    }
}

void MultiTextureImage::activateTexture(unsigned int idx) {
    if (idx >= NUM_TEXTURES) {
        LOG_ERROR << "Trying to activate invalid texture id in MultiTextureImage, id=" << idx << std::endl;
        idx = 0;
    }

    this->Image::setRgba(allColors_[idx]);

    // TODO:
    //this->Image::setHueInfo(allHueInfos_[idx]);

    this->Image::setTiled(allTiled_[idx]);
    this->Image::setTexture(allTextures_[idx]);
}

void MultiTextureImage::setColorRGBA(const CL_Colorf& color) {
    for (unsigned int i = 0; i < NUM_TEXTURES; ++i) {
        allColors_[i] = color;
    }
}

void MultiTextureImage::setHue(unsigned int hue) {
    for (unsigned int i = 0; i < NUM_TEXTURES; ++i) {
        allHueInfos_[i][0u] = hue;
    }
}

void MultiTextureImage::setAlpha(float alpha) {
    for (unsigned int i = 0; i < NUM_TEXTURES; ++i) {
        allColors_[i].a = alpha;
        allHueInfos_[i][2u] = alpha;
    }
}

}
}
}
