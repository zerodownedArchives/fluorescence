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


#ifndef FLUO_DATA_DEFSTRUCTS_HPP
#define FLUO_DATA_DEFSTRUCTS_HPP

#include <misc/log.hpp>

namespace fluo {
namespace data {

struct BodyDef {
    unsigned int origBody_;
    unsigned int newBody_;
    unsigned int hue_;
};

struct BodyConvDef {
    unsigned int bodyId_;
    int anim2_;
    int anim3_;
    int anim4_;
    int anim5_;

    unsigned int getAnimFileIdx() {
        if (bodyId_ == 0) {
            return 0;
        } else if (anim2_ != -1) {
            return 1;
        } else if (anim3_ != -1) {
            return 2;
        } else if (anim4_ != -1) {
            return 3;
        } else if (anim5_ != -1) {
            return 4;
        } else {
            return 0;
        }
    }

    unsigned int getAnimIdxInFile() {
        if (bodyId_ == 0) {
            return 0;
        } else if (anim2_ != -1) {
            return anim2_;
        } else if (anim3_ != -1) {
            return anim3_;
        } else if (anim4_ != -1) {
            return anim4_;
        } else if (anim5_ != -1) {
            return anim5_;
        } else {
            return bodyId_;
        }
    }
};

struct EquipConvDef {
    int bodyId_;
    int itemId_;
    int itemIdTranslate_;
    int gumpId_;
    int hue_;

    unsigned int getGumpId() {
        if (gumpId_ == 0) {
            return itemId_ + 50000;
        } else if (gumpId_ == -1) {
            return itemIdTranslate_ + 50000;
        } else {
            return gumpId_;
        }
    }
};

struct PaperdollDef {
    unsigned int bodyId_;
    unsigned int gumpId_;
    unsigned int gumpOffset_;
    unsigned int gumpOffsetFallback_;
};

struct GumpDef {
    unsigned int gumpId_;
    unsigned int translateId_;
    unsigned int hue_;
};

struct MountDef {
    unsigned int mountItemId_;
    unsigned int animId_;
};

struct MobTypeDef {
    unsigned int bodyId_;
    unsigned int mobType_;
    unsigned int flags_;
};

}
}

#endif
