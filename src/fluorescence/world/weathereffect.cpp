/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2010-2013, http://fluorescence-client.org

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

#include "weathereffect.hpp"

#include <ui/particles/emitter.hpp>

namespace fluo {
namespace world {

void WeatherEffect::update(unsigned int elapsedMillis) {
    float elapsedSeconds = elapsedMillis / 1000.f;

    std::list<boost::shared_ptr<ui::particles::Emitter> > expired;
    std::list<boost::shared_ptr<ui::particles::Emitter> >::iterator iter = emitters_.begin();
    std::list<boost::shared_ptr<ui::particles::Emitter> >::iterator end = emitters_.end();

    for (; iter != end; ++iter) {
        (*iter)->step(elapsedSeconds);

        if ((*iter)->isExpired()) {
            expired.push_back(*iter);
        }
    }

    if (!expired.empty()) {
        iter = expired.begin();
        end = expired.end();

        for (; iter != end; ++iter) {
            emitters_.remove(*iter);
        }
    }
}

}
}

