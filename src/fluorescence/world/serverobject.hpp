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


#ifndef FLUO_WORLD_SERVEROBJECT_HPP
#define FLUO_WORLD_SERVEROBJECT_HPP

#include "ingameobject.hpp"

#include <typedefs.hpp>

namespace fluo {
namespace world {

class ServerObject : public IngameObject {
public:
    ServerObject(Serial serial, unsigned int objectType);

    Serial getSerial() const;

    void setHue(unsigned int hue);

    virtual void onLocationChanged(const CL_Vec3f& oldLocation);
    virtual void onDelete();

    void clearClilocProperties();
    void addClilocProperty(unsigned int cliloc, const UnicodeString& args);
    void setClilocPropertiesHash(uint32_t hash);
    bool checkClilocPropertiesHash(uint32_t hash);
    void requestClilocProperties();

    virtual void openPropertyListGump(const CL_Point& mousePos);

private:
    Serial serial_;
    unsigned int hue_;

    uint32_t clilocPropertiesHash_;
    std::list<UnicodeString> clilocProperties_;
};

}
}

#endif
