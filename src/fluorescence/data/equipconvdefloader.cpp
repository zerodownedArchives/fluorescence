
#include "equipconvdefloader.hpp"

namespace fluo {
namespace data {


EquipConvDefLoader::EquipConvDefLoader(const boost::filesystem::path& path) {
    std::ifstream ifs(path.string().c_str());
    if (!ifs.is_open() || !ifs.good()) {
        LOG_ERROR << "Unable to open EquipConvDefLoader for path: " << path.string() << std::endl;
        return;
    }

    unsigned int numbersPerLine = 5;
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

        EquipConvDef curValue;
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

        std::pair<unsigned int, unsigned int> id = std::make_pair(curValue.bodyId_, curValue.gumpId_);
        //LOG_DEBUG << "store id " << id << std::endl;
        table_[id] = curValue;
    } while (ifs.good());
}

bool EquipConvDefLoader::hasValue(std::pair<unsigned int, unsigned int> id) const {
    return table_.find(id) != table_.end();
}

EquipConvDef EquipConvDefLoader::get(std::pair<unsigned int, unsigned int> id) const {
    std::map<std::pair<unsigned int, unsigned int>, EquipConvDef>::const_iterator iter = table_.find(id);
    if (iter != table_.end()) {
        return iter->second;
    } else {
        return EquipConvDef();
    }
}

}
}
