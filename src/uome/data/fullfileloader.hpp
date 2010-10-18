#ifndef UOME_DATA_FULLFILELOADER_HPP
#define UOME_DATA_FULLFILELOADER_HPP

#include <exception.hpp>

// use boost filesystem v3
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <boost/function.hpp>

namespace uome {
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
    FullFileLoader(const boost::filesystem::path& path) throw(Exception);

    /// brief Read full file to memory and call callback
    void read(ReadCallback readCallback) throw(Exception);

private:
    boost::filesystem::path path_;
};

}
}

#endif
