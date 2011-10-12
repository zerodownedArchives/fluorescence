
#include "filenamecaseconverter.hpp"

#include <boost/filesystem/operations.hpp>
#include <misc/string.hpp>

namespace fluo {
namespace misc {

bool FileNameCaseConverter::convert(const boost::filesystem::path& directory) {
    namespace bfs = boost::filesystem;

    if (!bfs::exists(directory / "art.mul") && !bfs::exists(directory / "Art.mul")) {
        return false;
    } else {
        convertRec(directory);
        return true;
    }
}

void FileNameCaseConverter::convertRec(const boost::filesystem::path& directory) {
    namespace bfs = boost::filesystem;

    if (!bfs::exists(directory) || !bfs::is_directory(directory)) {
        return;
    }

    bfs::directory_iterator nameIter(directory);
    bfs::directory_iterator nameEnd;

    for (; nameIter != nameEnd; ++nameIter) {
        bool wasConverted = false;
        UnicodeString str = StringConverter::fromUtf8(nameIter->path().filename());
        str.toLower();
        bfs::path toPath = nameIter->path().parent_path() / StringConverter::toUtf8String(str);

        if (toPath != nameIter->path()) {
            bfs::rename(nameIter->path(), toPath);
            wasConverted = true;
        }

        if (bfs::is_directory(nameIter->status())) {
            if (wasConverted) {
                convertRec(toPath);
            } else {
                convertRec(nameIter->path());
            }
        }
    }
}

}
}
