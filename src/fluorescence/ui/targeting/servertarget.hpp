#ifndef FLUO_UI_SERVERTARGET_HPP
#define FLUO_UI_SERVERTARGET_HPP

#include "target.hpp"

namespace fluo {
namespace ui {
namespace targeting {

class ServerTarget : public Target {
public:
    ServerTarget(bool allowGround);

    virtual void onTarget(boost::shared_ptr<world::IngameObject> obj);
    virtual void onCancel();
};

}
}
}

#endif
