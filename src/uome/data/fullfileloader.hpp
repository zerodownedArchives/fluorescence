#ifndef UOME_DATA_FULLFILELOADER_HPP
#define UOME_DATA_FULLFILELOADER_HPP

#include <exception.hpp>

// use boost filesystem v3
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace uome {
namespace data {

/**
 * \brief This class is used to load a full file of same-sized data blocks into memory (and keep them there), e.g. index files
 */

template <
typename ValueType
>
class FullFileLoader {
public:
    /**
     * \brief Read full file and store in memory
     * \throw DataException when file is nonexistant or directory; when file size is not multiple of sizeof(ValueType); on general read error
     */
    FullFileLoader(const boost::filesystem::path& path, unsigned int itemSize = sizeof(ValueType), unsigned int fileOffsetStart = 0, unsigned int fileOffsetEnd = 0) throw(Exception) {
        if (!boost::filesystem::exists(path) || !boost::filesystem::is_regular_file(path)) {
            throw Exception("File not found");
        }

        // calc how many bytes we want to read
        unsigned int filesize = boost::filesystem::file_size(path);
        unsigned int readLength = (fileOffsetEnd > 0) ? fileOffsetEnd : filesize;
        readLength -= fileOffsetStart;

        printf("filesize: %u start: %u end: %u readLength: %u\n", filesize, fileOffsetStart, fileOffsetEnd, readLength);

        if (readLength % itemSize != 0) {
            throw Exception("File size (resp. read length) is not multiple of sizeof(ValueType)");
        }

        size_ = readLength / itemSize;
        values_ = new ValueType[size_];
        int8_t buf[readLength];

        boost::filesystem::ifstream stream(path, std::ios_base::binary);
        if (stream.is_open()) {
            stream.seekg(fileOffsetStart, std::ios_base::beg);

            stream.read(reinterpret_cast<char*>(buf), readLength);
            stream.close();

            for (unsigned int i = 0; i < size_; ++i) {
                values_[i].read( &buf[i * itemSize], itemSize );
            }
        } else {
            throw Exception("Error opening stream");
        }
    }

    ~FullFileLoader() {
        delete [] values_;
    }

    const ValueType* get(unsigned int idx) const {
        if (idx < size_) {
            return &values_[idx];
        } else {
            return NULL;
        }
    }

    unsigned int size() {
        return size_;
    }

private:
    unsigned int size_;
    ValueType* values_;
};

}
}

#endif
