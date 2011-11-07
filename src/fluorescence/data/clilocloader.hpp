#ifndef FLUO_DATA_CLILOCLOADER_HPP
#define FLUO_DATA_CLILOCLOADER_HPP

#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>

#include <misc/string.hpp>

namespace fluo {
namespace data {

struct ClilocEntry {
    ClilocEntry() : id_(0), len_(0), streampos_(0), useCount_(0), fromEnu_(true) {
    }

    unsigned int id_;
    unsigned int len_;
    unsigned int streampos_;
    unsigned int useCount_; // if the string is accessed often, cache it
    UnicodeString cachedString_;
    bool fromEnu_;
};

class ClilocLoader {
public:
    ClilocLoader();
    ~ClilocLoader();

    void indexFile(const boost::filesystem::path& path, bool isEnu);

    UnicodeString get(unsigned int id);
    UnicodeString get(unsigned int id, const UnicodeString& paramString);

private:
    std::map<unsigned int, ClilocEntry> entryMap_;

    static const unsigned int USE_COUNT_CACHE_LIMIT = 3;

    UnicodeString readEntry(const ClilocEntry& entry);

    boost::filesystem::ifstream languageStream_;
    boost::filesystem::ifstream enuStream_;

    unsigned int bufferSize_;
    int8_t* buffer_;
};

}
}

#endif
