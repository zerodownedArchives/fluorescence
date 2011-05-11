
#include "filenamecaseconverter.hpp"

#include <boost/filesystem/operations.hpp>
#include <misc/string.hpp>

namespace uome {
namespace misc {

void FileNameCaseConverter::convert(const boost::filesystem::path& directory) {
    namespace bfs = boost::filesystem;

    if (!bfs::exists(directory) || !bfs::is_directory(directory)) {
        return;
    }

    bfs::directory_iterator nameIter(directory);
    bfs::directory_iterator nameEnd;

    for (; nameIter != nameEnd; ++nameIter) {
        if (bfs::is_directory(nameIter->status())) {
            convert(nameIter->path());
        }


        UnicodeString str = StringConverter::fromUtf8(nameIter->path().filename());
        str.toLower();
        bfs::path toPath = nameIter->path().parent_path() / StringConverter::toUtf8String(str);

        if (toPath != nameIter->path()) {
            bfs::rename(nameIter->path(), toPath);
        }
    }
}

}
}
