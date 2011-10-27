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

// TODO
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

}
}

#endif
