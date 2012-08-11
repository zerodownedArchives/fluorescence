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

#include "button.hpp"

#include <ClanLib/Display/Window/keys.h>

#include <data/manager.hpp>
#include <data/huesloader.hpp>
#include <ui/gumpmenu.hpp>
#include <ui/manager.hpp>

#include "scrollarea.hpp"

namespace fluo {
namespace ui {
namespace components {

Button::Button(CL_GUIComponent* parent) : MultiTextureImage(parent, 3), mouseOver_(false), mouseDown_(false) {
    func_input_pressed().set(this, &Button::onInputPressed);
    func_input_released().set(this, &Button::onInputReleased);
    func_pointer_enter().set(this, &Button::onPointerEnter);
    func_pointer_exit().set(this, &Button::onPointerExit);
    func_render().set(this, &Button::render);

    set_double_click_enabled(false);

    set_type_name("button");

    hasScrollareaParent_ = false;
    CL_GUIComponent* comp = parent;
    while (comp) {
        if (dynamic_cast<components::ScrollArea*>(comp)) {
            hasScrollareaParent_ = true;
            break;
        }
        comp = comp->get_parent_component();
    }

    fontDesc_.set_height(12);
    fontDesc_.set_subpixel(false);
}

bool Button::onInputReleased(const CL_InputEvent & e) {
    if (e.id == CL_MOUSE_LEFT) {
        mouseDown_ = false;
        updateTexture();
        handleClick();
        return true;
    } else {
        return false;
    }
}

bool Button::onInputPressed(const CL_InputEvent & e) {
    if (e.id == CL_MOUSE_LEFT) {
        mouseDown_ = true;
        updateTexture();
        return true;
    } else {
        return false;
    }
}

GumpMenu* Button::getTopLevelMenu() {
    CL_GUIComponent* topLevel = get_top_level_component();
    return dynamic_cast<GumpMenu*>(topLevel);
}

bool Button::onPointerEnter() {
    mouseOver_ = true;
    updateTexture();

    return true;
}

bool Button::onPointerExit() {
    mouseOver_ = false;
    mouseDown_ = false;
    updateTexture();

    return true;
}

void Button::updateTexture() {
    unsigned int idx = calcTextureId();
    activateTexture(idx);
    activateText(idx);
}

unsigned int Button::calcTextureId() const {
    if (mouseDown_) {
        return Button::TEX_INDEX_DOWN;
    } else {
        if (mouseOver_) {
            return Button::TEX_INDEX_MOUSEOVER;
        } else {
            return Button::TEX_INDEX_UP;
        }
    }
}

void Button::activateText(unsigned int index) {
    if (texts_[index].length() > 0) {
        span_ = CL_SpanLayout();

        CL_Font font = ui::Manager::getSingleton()->getFont(fontDesc_);
        span_.add_text(StringConverter::toUtf8String(texts_[index]), font, fontColors_[index]);

        span_.set_align((CL_SpanAlign)alignment_);

        displayText_ = true;
    } else {
        displayText_ = false;
    }
}

void Button::render(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    // render base image
    MultiTextureImage::render(gc, clipRect);

    if (displayText_) {
        if (!hasScrollareaParent_) {
            gc.push_cliprect(get_geometry());
        }

        span_.layout(gc, get_geometry().get_width());
        CL_Size spanSize = span_.get_size();
        // span aligns only horizontally. vertical alignment needs to be done manually
        span_.set_position(CL_Point(0, (get_height() - spanSize.height) / 2));
        span_.draw_layout(gc);
        span_.set_component_geometry();

        if (!hasScrollareaParent_) {
            gc.pop_cliprect();
        }
    }
}

void Button::setText(unsigned int index, const UnicodeString& text) {
    texts_[index] = text;
}

void Button::setFontColor(unsigned int index, const CL_Colorf& color) {
    fontColors_[index] = color;
}

void Button::setFontHue(unsigned int index, unsigned int hue) {
    fontColors_[index] = data::Manager::getHuesLoader()->getFontClColor(hue);;
}

void Button::setFont(const UnicodeString& name, unsigned int height) {
    fontDesc_.set_typeface_name(StringConverter::toUtf8String(name));
    fontDesc_.set_height(height);
}

void Button::setFontAlignment(unsigned int align) {
    alignment_ = align;
}

}
}
}
