#ifndef UOME_UI_GUMPS_SHARDSELECTION_HPP
#define UOME_UI_GUMPS_SHARDSELECTION_HPP

#include <ui/gumpmenu.hpp>

#include <list>

namespace uome {
namespace ui {
namespace gumps {

class ShardSelection : public GumpMenu {
public:
    static ShardSelection* create();

private:
    ShardSelection(const CL_GUITopLevelDescription& desc, const std::list<std::string>& names);
};

}
}
}

#endif
