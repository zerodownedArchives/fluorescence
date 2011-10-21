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

}
}

#endif
