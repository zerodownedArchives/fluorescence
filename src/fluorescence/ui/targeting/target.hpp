#ifndef FLUO_UI_TARGET_HPP
#define FLUO_UI_TARGET_HPP

#include <boost/shared_ptr.hpp>

namespace fluo {

namespace world {
    class IngameObject;
}

namespace ui {
namespace targeting {

class Target {
public:
    Target(bool allowGround);

    bool acceptsTarget(boost::shared_ptr<world::IngameObject> obj) const;

    virtual void onTarget(boost::shared_ptr<world::IngameObject> obj) = 0;
    virtual void onCancel() = 0;

private:
    bool allowGround_;
};

}
}
}

#endif
