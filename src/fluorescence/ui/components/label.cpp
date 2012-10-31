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



#include "label.hpp"

#include <stack>
#include <ClanLib/Display/2D/span_layout.h>
#include <unicode/regex.h>

#include <data/manager.hpp>
#include <data/huesloader.hpp>
#include <ui/uofont.hpp>
#include <ui/manager.hpp>
#include <ui/gumpmenu.hpp>
#include <misc/log.hpp>

#include "scrollarea.hpp"

namespace fluo {
namespace ui {
namespace components {

Label::Label(CL_GUIComponent* parent) : GumpComponent(parent),
        overrideGumpFont_(false), fontColor_(CL_Colorf::black), vAlign_(VAlign::MIDDLE), hAlign_(HAlign::LEFT), autoResize_(false) {
    func_render().set(this, &Label::onRender);

    fontDesc_.set_height(12);
    fontDesc_.set_subpixel(false);

    hasScrollareaParent_ = false;
    CL_GUIComponent* comp = parent;
    while (comp) {
        if (dynamic_cast<components::ScrollArea*>(comp)) {
            hasScrollareaParent_ = true;
            break;
        }
        comp = comp->get_parent_component();
    }

    set_type_name("label");
}

UnicodeString Label::getText() {
    return text_;
}

void Label::setText(const UnicodeString& text) {
    text_ = text;
    text_.findAndReplace("\\n", "\n");

    layout();
}

void Label::setFont(const UnicodeString& name, unsigned int height) {
    fontDesc_.set_typeface_name(StringConverter::toUtf8String(name));
    fontDesc_.set_height(height);

    cachedFont_ = ui::Manager::getSingleton()->getFont(fontDesc_);
    overrideGumpFont_ = true;

    layout();
}

void Label::setColor(const CL_Colorf& color) {
    fontColor_ = color;
}

void Label::setHue(unsigned int hue) {
    fontColor_ = data::Manager::getHuesLoader()->getFontClColor(hue);
}

void Label::onRender(CL_GraphicContext& gc, const CL_Rect& update_rect) {
    // clipping the text inside the scrollareas is rather complicated :/
    // quick hack: if there is a scrollbar, don't clip (will affect mostly html gumps)
    if (!hasScrollareaParent_) {
        gc.push_cliprect(get_geometry());
        span_.draw_layout(gc);
        gc.pop_cliprect();
    } else {
        span_.draw_layout(gc);
    }
}

void Label::layout() {
    CL_Font font = overrideGumpFont_ ? cachedFont_ : getGumpMenu()->getFont();
    span_ = CL_SpanLayout();
    span_.add_text(StringConverter::toUtf8String(text_), font, fontColor_);

    if (!autoResize_) {
        span_.set_align((CL_SpanAlign)hAlign_);
    }

    if (autoResize_) {
        CL_Rectf geom = get_geometry();

        // adjust width automatically
        span_.layout(ui::Manager::getGraphicContext(), 999999);
        int width = span_.get_size().width;
        int height = span_.get_size().height;

        geom.set_width(width);
        geom.set_height(height);
        set_geometry(geom);

        //valign
        span_.set_position(CL_Point(0, 0));

        span_.set_component_geometry();
    } else {
        span_.layout(ui::Manager::getGraphicContext(), get_geometry().get_width());
    }

    request_repaint();
}



void Label::setHtmlText(const UnicodeString& text) {
    UErrorCode status = U_ZERO_ERROR;

    static RegexMatcher basefontMatcher("basefont\\s+color=\"{0,1}([0-9a-fA-F#]+)\"{0,1}", 0, status);

    CL_Font curFont = overrideGumpFont_ ? cachedFont_ : getGumpMenu()->getFont();
    CL_Colorf curColor = fontColor_;

    std::stack<CL_Font> fontStack;
    std::stack<CL_Colorf> colorStack;

    fontStack.push(curFont);
    colorStack.push(curColor);

    span_ = CL_SpanLayout();

    int curIndex = 0;

    //LOG_DEBUG << "complete text: " << text << std::endl;

    do {
        int endIndex = text.indexOf('<', curIndex);
        if (endIndex == -1) {
            endIndex = text.length();
        }

        UnicodeString curText(text, curIndex, endIndex-curIndex);
        curText.findAndReplace("\n", "");
        curText.findAndReplace("\r", "");
        if (curText.length() > 0) {
            // add text with current properties
            //LOG_DEBUG << "adding text: " << curText << std::endl;
            span_.add_text(StringConverter::toUtf8String(curText), curFont, curColor);
        }

        if (endIndex == text.length()) {
            break;
        }

        curIndex = endIndex + 1;
        endIndex = text.indexOf('>', curIndex);
        UnicodeString curTag(text, curIndex, endIndex - curIndex);
        curTag.toLower();

        if (curTag.length() > 0) {
            basefontMatcher.reset(curTag);
            //LOG_DEBUG << "new tag: " << curTag << std::endl;
            if (basefontMatcher.find() && basefontMatcher.groupCount() == 1) {
                curColor = CL_Colorf(StringConverter::toUtf8String(basefontMatcher.group(1, status)));
                colorStack.push(curColor);
            } else if (curTag.startsWith("br")) {
                span_.add_text("\n", curFont, curColor);
            } else if (curTag.startsWith("center")) {
                span_.set_align(cl_center);

            } else if (curTag.startsWith("/basefont")) {
                curColor = colorStack.top();
                colorStack.pop();
            } else if (curTag.startsWith("/center")) {
            } else {
                LOG_DEBUG << "Unknown/unsupported html tag: <" << curTag << ">, skipping" << std::endl;
            }
        }

        curIndex = endIndex + 1;
    } while (curIndex < text.length());

    layout();
}

void Label::setAutoResize(bool value) {
    autoResize_ = value;

    if (value) {
        layout();
    }
}

void Label::setVAlign(VAlign align) {
    vAlign_ = align;

    request_repaint();
}

VAlign Label::getVAlign() const {
    return vAlign_;
}

void Label::setHAlign(HAlign align) {
    hAlign_ = align;
    layout();
}

HAlign Label::getHAlign() const {
    return hAlign_;
}

}
}
}
