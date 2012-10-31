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

#include "background.hpp"

#include <ClanLib/Display/2D/draw.h>
#include <ClanLib/Core/Math/quad.h>
#include <ClanLib/Core/Math/rect.h>
#include <ClanLib/Display/Render/program_object.h>
#include <boost/python/extract.hpp>

#include <ui/manager.hpp>
#include <ui/render/shadermanager.hpp>
#include <ui/texture.hpp>
#include <data/manager.hpp>
#include <data/huesloader.hpp>
#include <data/gumpartloader.hpp>
#include <misc/log.hpp>

#include "image.hpp"

namespace fluo {
namespace ui {
namespace components {

Background::Background(CL_GUIComponent* parent) : GumpComponent(parent), baseId_(0), coordinateRecalcRequired_(true) {
    func_render().set(this, &Background::render);

    set_type_name("background");

    for (unsigned int i = 0; i < 9; ++i) {
        images_[i] = new components::Image(this);
    }
}

void Background::setBaseId(unsigned int id) {
    baseId_ = id;
    for (unsigned int i = 0; i < 9; ++i) {
        images_[i]->setTexture(data::Manager::getTexture(data::TextureSource::GUMPART, i + id));
    }

    coordinateRecalcRequired_ = true;
}

unsigned int Background::getBaseId() const {
    return baseId_;
}

void Background::render(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    if (coordinateRecalcRequired_) {
        for (unsigned int i = 0; i < 9; ++i) {
            boost::shared_ptr<ui::Texture> cur = images_[i]->getTexture();
            if (!cur || !cur->isReadComplete()) {
                ui::Manager::getSingleton()->queueComponentRepaint(this);
                return; // don't draw until everything is ready
            }
        }

        calculateImageCoordinates();
    }
}

void Background::calculateImageCoordinates() {
    float width = get_geometry().get_width();
    float height = get_geometry().get_height();
    CL_Rectf defaultTexCoords(0.0f, 0.0f, 1.0f, 1.0f);

    float texWidths[9];
    float texHeights[9];

    for (unsigned int i = 0; i < 9; ++i) {
        boost::shared_ptr<ui::Texture> tex = images_[i]->getTexture();
        texWidths[i] = (std::min)(width, tex->getWidth());
        texHeights[i] = (std::min)(height, tex->getHeight());
    }

    float centerBarWidthTop = width - (texWidths[0] + texWidths[2]);
    float centerBarWidthBottom = width - (texWidths[6] + texWidths[8]);
    float middleBarHeightLeft = height - (texHeights[0] + texHeights[6]);
    float middleBarHeightRight = height - (texHeights[2] + texHeights[8]);

    float centerPieceWidth = width - (texWidths[3] + texWidths[5]);
    float centerPieceHeight = height - (texHeights[1] + texHeights[7]);

    images_[0]->set_geometry(CL_Rectf(0, 0, CL_Sizef(texWidths[0], texHeights[0])));

    images_[1]->set_geometry(CL_Rectf(texWidths[0], 0, CL_Sizef(centerBarWidthTop, texHeights[1])));
    images_[1]->setTiled(true);

    images_[2]->set_geometry(CL_Rectf(width - texWidths[2], 0, CL_Sizef(texWidths[2], texHeights[2])));

    images_[3]->set_geometry(CL_Rectf(0, texHeights[0], CL_Sizef(texWidths[3], middleBarHeightLeft)));
    images_[3]->setTiled(true);

    images_[4]->set_geometry(CL_Rectf(texWidths[3], texHeights[1], CL_Sizef(centerPieceWidth, centerPieceHeight)));
    images_[4]->setTiled(true);

    images_[5]->set_geometry(CL_Rectf(width - texWidths[5], texHeights[2], CL_Sizef(texWidths[5], middleBarHeightRight)));
    images_[5]->setTiled(true);

    images_[6]->set_geometry(CL_Rectf(0, height - texHeights[6], CL_Sizef(texWidths[6], texHeights[6])));

    images_[7]->set_geometry(CL_Rectf(texWidths[6], height - texHeights[7], CL_Sizef(centerBarWidthBottom, texHeights[7])));
    images_[7]->setTiled(true);

    images_[8]->set_geometry(CL_Rectf(width - texWidths[8], height - texHeights[8], CL_Sizef(texWidths[8], texHeights[8])));
    images_[8]->setTiled(true);

    coordinateRecalcRequired_ = false;
}

void Background::setAutoResize(bool value) {
    if (value) {
        LOG_WARN << "Auto resizing is not supported on background components" << std::endl;
    }
}

void Background::setHue(unsigned int hue) {
    for (unsigned int i = 0; i < 9; ++i) {
        images_[i]->setHue(hue);
    }
}

unsigned int Background::getHue() const {
    return images_[0]->getHue();
}

void Background::setRgba(const CL_Colorf& rgba) {
    for (unsigned int i = 0; i < 9; ++i) {
        images_[i]->setRgba(rgba);
    }
}

void Background::setRgba(float r, float g, float b, float a) {
    setRgba(CL_Colorf(r, g, b, a));
}

void Background::setRgba(float r, float g, float b) {
    setRgba(CL_Colorf(r, g, b));
}

CL_Colorf Background::getRgba() const {
    return images_[0]->getRgba();
}

void Background::setAlpha(float alpha) {
    for (unsigned int i = 0; i < 9; ++i) {
        images_[i]->setAlpha(alpha);
    }
}

float Background::getAlpha() const {
    return images_[0]->getAlpha();
}

void Background::setPartialHue(bool value) {
    for (unsigned int i = 0; i < 9; ++i) {
        images_[i]->setPartialHue(value);
    }
}

bool Background::getPartialHue() const {
    return images_[0]->getPartialHue();
}

boost::python::tuple Background::pyGetRgba() const {
    CL_Colorf rgba = getRgba();
    return boost::python::make_tuple(rgba.r, rgba.g, rgba.b, rgba.a);
}

void Background::pySetRgba(const boost::python::tuple& rgba) {
    namespace bpy = boost::python;

    float r = bpy::extract<float>(rgba[0]);
    float g = bpy::extract<float>(rgba[1]);
    float b = bpy::extract<float>(rgba[2]);

    if (bpy::len(rgba) > 3) {
        float a = bpy::extract<float>(rgba[3]);
        setRgba(r, g, b, a);
    } else {
        setRgba(r, g, b);
    }
}

}
}
}

