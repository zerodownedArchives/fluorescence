/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef FLUO_DATA_DEFFILELOADER_HPP
#define FLUO_DATA_DEFFILELOADER_HPP

#include <map>
#include <boost/filesystem/path.hpp>
#include <boost/function.hpp>

#include <misc/exception.hpp>

namespace fluo {
namespace data {

template<typename ValueType>
class DefFileLoader {

private:
    typedef boost::function<void (ValueType&, unsigned int, const char*, int*&)> StringParseFunction;

public:
    DefFileLoader(const boost::filesystem::path& path, const char* pattern, StringParseFunction stringParseFunction = StringParseFunction()) {
        std::ifstream ifs(path.string().c_str());
        if (!ifs.is_open() || !ifs.good()) {
            LOG_ERROR << "Unable to open DefFileLoader for path: " << path.string() << std::endl;
            return;
        }
        
        unsigned int patternCount = strlen(pattern);
        char strBuf[256];
        while (ifs.good()) {
            if (ifs.peek() == '#' || ifs.peek() == '\n' || ifs.peek() == '\r') {
                ifs.ignore(1000, '\n');
                continue;
            }
            
            ValueType curValue;
            int* ptr = reinterpret_cast<int*>(&curValue);
            
            for (unsigned int patternIdx = 0; patternIdx < patternCount; ++patternIdx) {
                if (!ifs.good()) {
                    break;
                }
                
                switch (pattern[patternIdx]) {
                    case 'i': {
                        int newNum = 0;
                        ifs >> newNum;
                        *ptr = newNum;
                        ++ptr;
                        
                        break;
                    }
                    
                    case 'r': {
                        ifs.ignore(100, '{');
                        ifs.get(strBuf, 256, '}');
                        ifs.ignore(1);
                        std::istringstream iss(strBuf);
                        
                        std::vector<int> randNrs;
                        while (iss.good()) {
                            int newRand;
                            iss >> newRand;
                            if (!iss.fail()) {
                                randNrs.push_back(newRand);
                            }
                            iss.ignore(100, ',');
                        }
                        
                        if (randNrs.size() == 0 && ifs.good()) {
                            LOG_ERROR << "Unable to extract number from random group {" << strBuf << "} in file " << path << std::endl;
                            throw Exception("Error parsing def file");
                        } else if (randNrs.size() >= 1) {
                            *ptr = randNrs[0];
                            ++ptr;
                        }
                        // TODO: random information is discarded here
                        
                        break;
                    }
                    
                    case 's': {
                        ifs >> strBuf;
                        if (stringParseFunction) {
                            stringParseFunction(curValue, patternIdx, strBuf, ptr);
                        } else {
                            LOG_ERROR << "No string parse function given for pattern " << pattern << " for file " << path << std::endl;
                            throw Exception("Error parsing def file");
                        }
                        break;
                    }
                }
            }
            
            if (!ifs.fail()) {
                // reading new value was ok, insert now
                int id = *(reinterpret_cast<int*>(&curValue));
                //LOG_DEBUG << "store id " << id << std::endl;
                table_[id] = curValue;
            }
            
            ifs.ignore(1000, '\n');
        }
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
    
    typename std::map<int, ValueType>::const_iterator begin() {
        return table_.begin();
    }
    
    typename std::map<int, ValueType>::const_iterator end() {
        return table_.end();
    }

private:
    std::map<int, ValueType> table_;

};
}
}


#endif
