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

#include <ui/uofont.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace components {

Label::Label(CL_GUIComponent* parent) : CL_Label(parent) {
}

UnicodeString Label::getText() {
    return StringConverter::fromUtf8(get_text());
}

void Label::setText(const UnicodeString& text) {
    set_text(StringConverter::toUtf8String(text));
}

void Label::setHtmlText(const UnicodeString& text, const CL_Colorf& colorDefault) {
    UErrorCode status = U_ZERO_ERROR;
    
    static UoFont fontDefault(1);
    static UoFont fontBold(3);
    static UoFont fontItalic(4);
    
    static RegexMatcher basefontMatcher("basefont\\s+color=([0-9a-fA-F#]+)", 0, status);
    
    CL_SpanLayout span;
    
    CL_Font curFont = fontDefault;
    CL_Colorf curColor = colorDefault;
    
    std::stack<CL_Font> fontStack;
    std::stack<CL_Colorf> colorStack;
    
    fontStack.push(curFont);
    colorStack.push(curColor);
    
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
            span.add_text(StringConverter::toUtf8String(curText), curFont, curColor);
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
                span.add_text("\n", curFont, curColor);
            } else if (curTag.startsWith("center")) {
                span.set_align(cl_center);
                set_alignment(align_center);
                
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
    
    //span.add_text(StringConverter::toUtf8String(text), fontDefault, CL_Colorf::blue);
    
    set_span(span);
}

}
}
}
