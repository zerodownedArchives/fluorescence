#ifndef FLUO_DATA_FULLFILELOADER_HPP
#define FLUO_DATA_FULLFILELOADER_HPP

#include <misc/exception.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <boost/function.hpp>

namespace fluo {
namespace data {

/**
 * \brief This class is used to load a full file into memory
 */
class FullFileLoader {
public:

    typedef boost::function<void (int8_t*, unsigned int)> ReadCallback;

    /**
     * \throw DataException when file is nonexistant or directory
     */
    FullFileLoader(const boost::filesystem::path& path);

    /// brief Read full file to memory and call callback
    void read(ReadCallback readCallback);

private:
    boost::filesystem::path path_;
};

}
}

#endif
