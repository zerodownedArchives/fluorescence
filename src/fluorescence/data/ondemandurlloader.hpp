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


#ifndef FLUO_DATA_ONDEMANDURLLOADER_HPP
#define FLUO_DATA_ONDEMANDURLLOADER_HPP

#include "indexloader.hpp"

#include <misc/log.hpp>
#include <misc/exception.hpp>

#include <queue>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

namespace fluo {
namespace data {

/**
 * \brief Used to load parts of files on demand (e.g. animations, arts, ...)
 */
template <
typename UrlType,
typename ValueType
>
class OnDemandUrlLoader {
public:
    typedef boost::function<void (const UrlType&, boost::shared_ptr<ValueType>)> ReadCallback;
    
    OnDemandUrlLoader(ReadCallback callback) : callback_(callback) {
        running_ = true;
        ioThread_ = new boost::thread(boost::bind(&OnDemandUrlLoader::run, this));
    }

    ~OnDemandUrlLoader() {
        if (ioThread_) {
            kill();
            ioThread_->join();
            delete ioThread_;
            ioThread_ = NULL;
        }
    }

    boost::shared_ptr<ValueType> get(const UrlType& url, unsigned int userData) {
        // return dummy object, enqueue for decoding
        boost::shared_ptr<ValueType> obj(new ValueType());

        ReadInformation inf(url, obj);
        boost::mutex::scoped_lock lock(mutex_);
        queue_.push(inf);
        signal_.notify_all();

        return obj;
    }

    void kill() {
        running_ = false;
        signal_.notify_all();
    }
    
protected:
    struct ReadInformation {
        UrlType url_;
        boost::shared_ptr<ValueType> item_;

        ReadInformation() {
        }

        ReadInformation(const UrlType& url, boost::shared_ptr<ValueType> item) :
                url_(url), item_(item) {
        }
    };

private:
    void run() {
        while (running_) {
            ReadInformation next;
            {
                boost::mutex::scoped_lock lock(mutex_);
                if (!queue_.empty()) {
                    next = queue_.front();
                    queue_.pop();
                } else {
                    signal_.wait(lock);
                    continue;
                }
            }

            callback_(next.url_, next.item_);
        }
    }

    boost::thread* ioThread_;
    bool running_;
    boost::condition_variable signal_;
    boost::mutex mutex_;

    std::queue<ReadInformation> queue_;
    
    ReadCallback callback_;
};

}
}

#endif
