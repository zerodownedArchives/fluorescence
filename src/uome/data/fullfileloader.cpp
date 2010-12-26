
#include "fullfileloader.hpp"

namespace uome {
namespace data {

FullFileLoader::FullFileLoader(const boost::filesystem::path& path) : path_(path) {
    if (!boost::filesystem::exists(path) || !boost::filesystem::is_regular_file(path)) {
        throw Exception("File not found");
    }
}

void FullFileLoader::read(ReadCallback readCallback) {
    boost::filesystem::ifstream stream(path_, std::ios_base::binary);
    if (stream.is_open()) {
        // calc how many bytes we want to read
        unsigned int filesize = boost::filesystem::file_size(path_);

        int8_t* buf = reinterpret_cast<int8_t*>(malloc(filesize));

        stream.seekg(0, std::ios_base::beg);

        stream.read(reinterpret_cast<char*>(buf), filesize);
        stream.close();

        readCallback(buf, filesize);

        free(buf);
    } else {
        throw Exception("Error opening stream");
    }
}

}
}
