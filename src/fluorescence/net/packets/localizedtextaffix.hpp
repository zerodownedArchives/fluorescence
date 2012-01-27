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


#ifndef FLUO_NET_PACKETS_LOCALIZEDTEXTAFFIX_HPP
#define FLUO_NET_PACKETS_LOCALIZEDTEXTAFFIX_HPP

#include "basetext.hpp"

namespace fluo {
namespace net {

namespace packets {

class LocalizedTextAffix : public BaseText {
public:
    LocalizedTextAffix();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

private:
    uint32_t clilocId_;
    uint8_t affixType_;
    UnicodeString affix_;
    UnicodeString arguments_;
};

}
}
}

#endif
