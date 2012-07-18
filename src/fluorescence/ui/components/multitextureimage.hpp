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


#ifndef FLUO_UI_COMPONENTS_MULTITEXTUREIMAGE_HPP
#define FLUO_UI_COMPONENTS_MULTITEXTUREIMAGE_HPP

#include <boost/shared_ptr.hpp>

#include "image.hpp"

namespace fluo {
namespace ui {
namespace components {

class MultiTextureImage : public Image {

public:
    MultiTextureImage(CL_GUIComponent* parent, unsigned int numTextures);
    ~MultiTextureImage();

    void addTexture(unsigned int index, boost::shared_ptr<ui::Texture> texture, unsigned int hue = 0, const std::string& rgba = "", float alpha = 1, bool tiled = false);
    void activateTexture(unsigned int idx);

    virtual void setColorRGBA(const CL_Colorf& color);
    virtual void setHue(unsigned int hue);
    virtual void setAlpha(float alpha);

private:
    const unsigned int NUM_TEXTURES;
    boost::shared_ptr<ui::Texture>* allTextures_;
    CL_Colorf* allColors_;
    CL_Vec3f* allHueInfos_;
    unsigned int* allTiled_;
};

}
}
}

#endif

