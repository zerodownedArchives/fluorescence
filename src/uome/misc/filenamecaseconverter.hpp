#ifndef UOME_MISC_FILECASECONVERTER_HPP
#define UOME_MISC_FILECASECONVERTER_HPP

#include <boost/filesystem/path.hpp>

namespace uome {
namespace misc {

class FileNameCaseConverter {
public:
    static void convert(const boost::filesystem::path& directory);
};

}
}


#endif
