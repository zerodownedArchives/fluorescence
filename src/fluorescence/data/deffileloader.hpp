#ifndef FLUO_DATA_DEFFILELOADER_HPP
#define FLUO_DATA_DEFFILELOADER_HPP

#include <map>
#include <boost/filesystem/path.hpp>

namespace fluo {
namespace data {

template<typename ValueType>
class DefFileLoader {
public:
    DefFileLoader(const boost::filesystem::path& path) {
        std::ifstream ifs(path.string().c_str());
        if (!ifs.is_open() || !ifs.good()) {
            LOG_ERROR << "Unable to open DefFileLoader for path: " << path.string() << std::endl;
            return;
        }

        unsigned int numbersPerLine = sizeof(ValueType) / sizeof(int);
        do {
            int peekChar = ifs.peek();
            while (!isdigit(peekChar) && peekChar != '-' && peekChar != '#' && ifs.good()) {
                // jump leading whitespaces and empty lines
                ifs.ignore();
                peekChar = ifs.peek();
            }

            if (!ifs.good()) {
                break;
            }

            if (peekChar == '#') {
                // jump comment
                ifs.ignore(1024, '\n');
                continue;
            }

            ValueType curValue;
            int* ptr = reinterpret_cast<int*>(&curValue);
            bool bracketFlag = false; // used to jump the { } groups

            for (unsigned int num = 0; num < numbersPerLine; ++num) {
                while (!isdigit(ifs.peek()) && ifs.peek() != '-' && ifs.peek() != '{') {
                    ifs.ignore();
                }

                if (ifs.peek() == '{') { // why, osi? ^^
                    bracketFlag = true;
                    while (!isdigit(ifs.peek()) && ifs.peek() != '-') {
                        ifs.ignore();
                    }
                }

                ifs >> (*ptr);
                ++ptr;

                if (bracketFlag) {
                    ifs.ignore(1024, '}');
                    bracketFlag = false;
                }

                if (!ifs.good()) {
                    LOG_ERROR << "Error parsing def file " << path.string() << std::endl;
                    break;
                }
            }

            ifs.ignore(1024, '\n'); // jump to next line

            int id = *(reinterpret_cast<int*>(&curValue));
            //LOG_DEBUG << "store id " << id << std::endl;
            table_[id] = curValue;
        } while (ifs.good());
    }

    bool hasValue(int id) const {
        return table_.find(id) != table_.end();
    }

    ValueType get(int id) const {
        typename std::map<int, ValueType>::const_iterator iter = table_.find(id);
        if (iter != table_.end()) {
            return iter->second;
        } else {
            return ValueType();
        }
    }

private:
    std::map<int, ValueType> table_;

};
}
}


#endif
