#ifndef UOME_UI_DOUBLECLICKHANDLER_HPP
#define UOME_UI_DOUBLECLICKHANDLER_HPP

#include <boost/program_options/variables_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

namespace boost {
    class thread;
}

namespace uome {

namespace world {
    class IngameObject;
}

namespace ui {

/***
 * This class is used to distinguish double clicks from single clicks.
 * On win, this might not be necessary, but there is no double click support in clanlib
 * linux code.
 * Runs a thread that wait()s for a semaphore to be called (happens at each click on
 * an ingameobject). After the first wait, there is a timewait with the doubleclick
 * timeout. If another post() is made => double click, otherwise => single click
 */
class DoubleClickHandler {
public:
    DoubleClickHandler(const boost::program_options::variables_map& config);
    ~DoubleClickHandler();

    void start();
    void kill();
    void run();

    void notify(boost::shared_ptr<world::IngameObject> obj);

private:
    bool running_;

    boost::interprocess::interprocess_semaphore sema_;
    unsigned int timeoutMillis_;

    boost::shared_ptr<boost::thread> thread_;

    boost::shared_ptr<world::IngameObject> lastObject_;
};

}
}

#endif
