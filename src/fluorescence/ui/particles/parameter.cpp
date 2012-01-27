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



#include "parameter.hpp"



namespace fluo {
namespace ui {
namespace particles {

Parameter::Parameter(float value) : value_(value), min_(value), max_(value) {
}

Parameter::Parameter(float min, float max) : min_(min), max_(max) {
    setRandom();
}

Parameter::Parameter(const Parameter& copy) : min_(copy.min_), max_(copy.max_) {
    setRandom();
}

Parameter& Parameter::operator=(const Parameter& copy) {
    min_ = copy.min_;
    max_ = copy.max_;

    setRandom();

    return *this;
}

void Parameter::setRandom() {
    if (min_ != max_) {
        value_ = min_ + ((float)rand() / RAND_MAX) * (max_ - min_);
    } else {
        value_ = min_;
    }
}

}
}
}
