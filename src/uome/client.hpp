#ifndef UOME_CLIENT_HPP
#define UOME_CLIENT_HPP

#include <vector>
#include <ClanLib/Core/Text/string8.h>
#include <ClanLib/App/clanapp.h>

#include <boost/program_options.hpp>

namespace uome {

namespace world {
    class Sector;
}

class Client {
public:
    Client();

    static int sMain(const std::vector<CL_String8>& args); ///< static helper to call main
    int main(const std::vector<CL_String8>& args);

private:
    boost::program_options::variables_map config_;
};

}

CL_ClanApplication app(&uome::Client::sMain);

#endif
