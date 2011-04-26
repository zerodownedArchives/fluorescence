
#include "shardselection.hpp"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <misc/logger.hpp>

#include <ui/components/localbutton.hpp>

namespace uome {
namespace ui {
namespace gumps {

ShardSelection* ShardSelection::create() {
    namespace bfs = boost::filesystem;

    bfs::path path("shards");

    if (!bfs::exists(path) || !bfs::is_directory(path)) {
        LOG_ERROR(LOGTYPE_UI, "Unable to list shards directory");
        return NULL;
    }

    std::list<std::string> nameList;

    bfs::directory_iterator iter(path);
    bfs::directory_iterator end;

    for (; iter != end; ++iter) {
        if (bfs::is_directory(iter->status())) {
            nameList.push_back(iter->path().filename());
        }
    }

    unsigned int windowHeight = nameList.size() * 28 + 101; // two more buttons and some space top/bottom
    unsigned int windowWidth = 250;

    CL_GUITopLevelDescription desc(CL_Rect(275, 300 - (windowHeight / 2), CL_Size(windowWidth, windowHeight)), true);
    desc.set_decorations(false);

    return new ShardSelection(desc, nameList);
}

ShardSelection::ShardSelection(const CL_GUITopLevelDescription& desc, const std::list<std::string>& names) : GumpMenu(desc) {
    setClosable(false);
    setDraggable(false);

    std::list<std::string>::const_iterator iter = names.begin();
    std::list<std::string>::const_iterator end = names.end();

    unsigned int idx = 0;
    for (; iter != end; ++iter, ++idx) {
        ui::components::LocalButton* button = new ui::components::LocalButton(this, "selectshard", *iter);
        button->set_geometry(CL_Rect(20, 15 + idx * 28, CL_Size(210, 25)));
        button->set_text(*iter);
        //LOGARG_DEBUG(LOGTYPE_UI, "Shard name: %s", iter->c_str());
    }

    ui::components::LocalButton* createButton = new ui::components::LocalButton(this, "createshard");
    createButton->set_geometry(CL_Rect(20, 30 + idx * 28, CL_Size(210, 25)));
    createButton->set_text("Create shard");
    createButton->set_enabled(false);
    ++idx;

    ui::components::LocalButton* closeButton = new ui::components::LocalButton(this, "shutdown");
    closeButton->set_geometry(CL_Rect(20, 30 + idx * 28, CL_Size(210, 25)));
    closeButton->set_text("Close UO:ME");
    ++idx;
}

}
}
}
