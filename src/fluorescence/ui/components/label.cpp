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
#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace components {

Label::Label(CL_GUIComponent* parent) : CL_GUIComponent(parent), spanInitialized_(false) {
    func_render().set(this, &Label::onRender);
}

UnicodeString Label::getText() {
    return text_;
}

void Label::setText(const UnicodeString& text) {
    text_ = text;
    text_.findAndReplace("\\n", "\n");

    spanInitialized_ = false;
    request_repaint();
}

void Label::setAlignment(unsigned int align) {
    alignment_ = align;

    spanInitialized_ = false;
    request_repaint();
}

void Label::setFontName(const UnicodeString& name) {
    fontDesc_.set_typeface_name(StringConverter::toUtf8String(name));

    spanInitialized_ = false;
    request_repaint();
}

void Label::setFontHeight(unsigned int height) {
    fontDesc_.set_height(height);

    spanInitialized_ = false;
    request_repaint();
}

void Label::setColor(const CL_Colorf& color) {
    fontColor_ = color;

    spanInitialized_ = false;
    request_repaint();
}

void Label::setHue(unsigned int hue) {
    fontColor_ = data::Manager::getHuesLoader()->getFontClColor(hue);
}

void Label::onRender(CL_GraphicContext& gc, const CL_Rect& update_rect) {
    gc.push_cliprect(get_geometry());

    if (!spanInitialized_) {
        span_ = CL_SpanLayout();

        CL_Font font = ui::Manager::getSingleton()->getFont(fontDesc_);
        span_.add_text(StringConverter::toUtf8String(text_), font, fontColor_);

        span_.set_align((CL_SpanAlign)alignment_);
        span_.layout(gc, get_geometry().get_width());
        span_.set_position(CL_Point(0, 0));
        span_.set_component_geometry();
        spanInitialized_ = true;
    }

    span_.draw_layout(gc);

    gc.pop_cliprect();
}



void Label::setHtmlText(const UnicodeString& text, const CL_Colorf& colorDefault) {
    // TODO: create html component for that (?)
    //UErrorCode status = U_ZERO_ERROR;

    //const UoFont& fontDefault = ui::Manager::getUnifont(1);
    ////const UoFont& fontBold = ui::Manager::getUnifont(3);
    ////const UoFont& fontItalic = ui::Manager::getUnifont(4);

    //static RegexMatcher basefontMatcher("basefont\\s+color=([0-9a-fA-F#]+)", 0, status);

    //CL_SpanLayout span;

    //CL_Font curFont = fontDefault;
    //CL_Colorf curColor = colorDefault;

    //std::stack<CL_Font> fontStack;
    //std::stack<CL_Colorf> colorStack;

    //fontStack.push(curFont);
    //colorStack.push(curColor);

    //int curIndex = 0;

    ////LOG_DEBUG << "complete text: " << text << std::endl;

    //do {
        //int endIndex = text.indexOf('<', curIndex);
        //if (endIndex == -1) {
            //endIndex = text.length();
        //}

        //UnicodeString curText(text, curIndex, endIndex-curIndex);
        //curText.findAndReplace("\n", "");
        //curText.findAndReplace("\r", "");
        //if (curText.length() > 0) {
            //// add text with current properties
            ////LOG_DEBUG << "adding text: " << curText << std::endl;
            //span.add_text(StringConverter::toUtf8String(curText), curFont, curColor);
        //}

        //if (endIndex == text.length()) {
            //break;
        //}

        //curIndex = endIndex + 1;
        //endIndex = text.indexOf('>', curIndex);
        //UnicodeString curTag(text, curIndex, endIndex - curIndex);
        //curTag.toLower();

        //if (curTag.length() > 0) {
            //basefontMatcher.reset(curTag);
            ////LOG_DEBUG << "new tag: " << curTag << std::endl;
            //if (basefontMatcher.find() && basefontMatcher.groupCount() == 1) {
                //curColor = CL_Colorf(StringConverter::toUtf8String(basefontMatcher.group(1, status)));
                //colorStack.push(curColor);
            //} else if (curTag.startsWith("br")) {
                //span.add_text("\n", curFont, curColor);
            //} else if (curTag.startsWith("center")) {
                //span.set_align(cl_center);
                //set_alignment(align_center);

            //} else if (curTag.startsWith("/basefont")) {
                //curColor = colorStack.top();
                //colorStack.pop();
            //} else if (curTag.startsWith("/center")) {
            //} else {
                //LOG_DEBUG << "Unknown/unsupported html tag: <" << curTag << ">, skipping" << std::endl;
            //}
        //}

        //curIndex = endIndex + 1;
    //} while (curIndex < text.length());

    ////span.add_text(StringConverter::toUtf8String(text), fontDefault, CL_Colorf::blue);

    //set_span(span);
}

}
}
}
