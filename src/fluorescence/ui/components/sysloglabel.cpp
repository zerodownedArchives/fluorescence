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


#include "sysloglabel.hpp"

#include <ClanLib/Display/2D/span_layout.h>

#include <data/manager.hpp>
#include <data/huesloader.hpp>
#include <misc/log.hpp>
#include <ui/manager.hpp>
#include <ui/uofont.hpp>
#include <world/manager.hpp>

namespace fluo {
namespace ui {
namespace components {

SysLogLabel::SysLogLabel(CL_GUIComponent* parent) : Label(parent) {
    world::Manager::getSysLog()->registerNotify(this);

    set_type_name("sysloglabel");
}

SysLogLabel::~SysLogLabel() {
    world::Manager::getSysLog()->unregisterNotify(this);
}

void SysLogLabel::setMaxGeometry(const CL_Rectf& rect) {
    maxGeometry_ = rect;
}

void SysLogLabel::notify(std::list<world::SysLogEntry>::const_iterator iter, std::list<world::SysLogEntry>::const_iterator end) {
    span_ = CL_SpanLayout();

    // font property from packet is ignored
    CL_Font font = ui::Manager::getSingleton()->getFont(fontDesc_);

    for (; iter != end; ++iter) {
        span_.add_text("\n", font, CL_Colorf::white);

        // 0x3B2 is the default font used by runuo. use the custom color instead
        if (iter->hue_ == 0x3B2) {
            span_.add_text(StringConverter::toUtf8String(iter->text_), font, rgba_);
        } else {
            span_.add_text(StringConverter::toUtf8String(iter->text_), font, data::Manager::getHuesLoader()->getFontClColor(iter->hue_));
        }
    }

    span_.layout(ui::Manager::getGraphicContext(), maxGeometry_.get_width());

    CL_Rectf newGeom(maxGeometry_.left, maxGeometry_.bottom - span_.get_size().height, maxGeometry_.right, maxGeometry_.bottom);
    set_geometry(newGeom);
    span_.set_position(CL_Point(0, 0));
    span_.set_component_geometry();
}

}
}
}

