#ifndef FLUO_MISC_FILECASECONVERTER_HPP
#define FLUO_MISC_FILECASECONVERTER_HPP

#include <boost/filesystem/path.hpp>

namespace fluo {
namespace misc {

class FileNameCaseConverter {
public:
    static void convert(const boost::filesystem::path& directory);
};

}
}


#endif
