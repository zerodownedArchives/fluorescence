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


#ifndef FLUO_DATA_ONDEMANDREADABLE_HPP
#define FLUO_DATA_ONDEMANDREADABLE_HPP

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace fluo {
namespace data {

template<class T>
class OnDemandReadable {
public:
    typedef boost::function<void (boost::shared_ptr<T>)> Callback;
    
    OnDemandReadable() : readComplete_(false) { }
    
    void setReadComplete(boost::shared_ptr<T> sharedThis = boost::shared_ptr<T>()) { 
        readComplete_ = true; 
        
        if (completeCallback_ && sharedThis) {
            completeCallback_(sharedThis);
        }
    }
    
    bool isReadComplete() { 
        return readComplete_; 
    }
    
    void setCompleteCallback(Callback cb) { completeCallback_ = cb; }

private:
    bool readComplete_;
    
    Callback completeCallback_;
};

}
}

#endif
