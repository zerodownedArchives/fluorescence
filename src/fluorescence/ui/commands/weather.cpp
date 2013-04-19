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

#include "weather.hpp"

#include <unicode/regex.h>

#include <typedefs.hpp>
#include <world/manager.hpp>

namespace fluo {
namespace ui {
namespace commands {

Weather::Weather() : ClientCommand("Usage: weather <type> <intensity>. Sets the weather") {
}

void Weather::execute(const UnicodeString& args) {
    UErrorCode status = U_ZERO_ERROR;
    RegexMatcher matcher("\\s*(\\d+)\\s+(\\d+)\\s*", 0, status);
    matcher.reset(args);

    int weather, intensity;
    if (matcher.find() && matcher.groupCount() == 2) {
        weather = StringConverter::toInt(matcher.group(1, status));
        intensity = StringConverter::toInt(matcher.group(2, status));
        world::Manager::getSingleton()->setWeather(weather, intensity, 0);
    } else {
        world::Manager::getSingleton()->systemMessage("Usage: weather <type (number)> <intensity (number)>");
    }
}

}
}
}

