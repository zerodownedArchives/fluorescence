#ifndef UOME_DATA_ONDEMANDFILELOADER_HPP
#define UOME_DATA_ONDEMANDFILELOADER_HPP

#include "indexloader.hpp"

#include <logger.hpp>
#include <exception.hpp>

#include <queue>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

namespace uome {
namespace data {

/**
 * \brief Used to load parts of files on demand (e.g. animations, arts, ...)
 */
template <
typename ValueType
>
class OnDemandFileLoader {
public:

    typedef boost::function<void (unsigned int, int8_t*, unsigned int, boost::shared_ptr<ValueType>, unsigned int)> ReadCallback;

    OnDemandFileLoader(const boost::filesystem::path& path, ReadCallback readCallback) throw(Exception) : readCallback_(readCallback) {
        if (!boost::filesystem::exists(path) || !boost::filesystem::is_regular_file(path)) {
            throw Exception("File not found");
        }

        stream_.open(path, std::ios_base::binary);
        if (!stream_.is_open()) {
            throw Exception("Error opening stream");
        }

        fileSize_ = boost::filesystem::file_size(path);

        running_ = true;
        ioThread_ = new boost::thread(boost::bind(&OnDemandFileLoader::run, this));
    }

    ~OnDemandFileLoader() {
        if (ioThread_) {
            kill();
            ioThread_->join();
            delete ioThread_;
            ioThread_ = NULL;
        }

        if (stream_.is_open()) {
            stream_.close();
        }
    }

    boost::shared_ptr<ValueType> get(unsigned int index, const IndexBlock* indexBlock) {
        // return dummy object, enqueue for decoding
        boost::shared_ptr<ValueType> obj(new ValueType);

        ReadInformation inf(index, indexBlock, obj);
        boost::mutex::scoped_lock lock(mutex_);
        queue_.push(inf);
        signal_.notify_all();

        return obj;
    }

    void kill() {
        running_ = false;
        signal_.notify_all();
    }

private:
    struct ReadInformation {
        unsigned int index_;
        unsigned int offset_;
        unsigned int readLen_;
        unsigned int extra_;
        boost::shared_ptr<ValueType> item_;

        ReadInformation() {
        }

        ReadInformation(unsigned int index, const IndexBlock* indexBlock, boost::shared_ptr<ValueType> item) :
                index_(index), offset_(indexBlock->offset_), readLen_(indexBlock->length_), extra_(indexBlock->extra_), item_(item) {
        }
    };

    void run() {
        unsigned int curBufferSize = 2048;
        int8_t* buf = reinterpret_cast<int8_t*>(malloc(curBufferSize));
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

            // trying to read out of file bounds
            if (next.offset_ > fileSize_ || (next.offset_ + next.readLen_) > fileSize_) {
                LOG_WARN(LOGTYPE_DATA, "Trying to read out of file bounds\n");
                continue;
            }

            if (next.offset_ != stream_.tellg()) {
                stream_.seekg(next.offset_, std::ios_base::beg);
            }

            if (next.readLen_ > curBufferSize) {
                //printf("Resizing buffer to %u\n", next.readLen_ * 2);
                buf = reinterpret_cast<int8_t*>(realloc(buf, next.readLen_ * 2));
                curBufferSize = next.readLen_;
            }

            //printf("decode thread read %u bytes from %u\n", next.readLen_, next.offset_);
            stream_.read(reinterpret_cast<char*>(buf), next.readLen_);
            readCallback_(next.index_, buf, next.readLen_, next.item_, next.extra_);
            next.item_->setReadComplete();
        }
    }

    boost::thread* ioThread_;
    bool running_;
    boost::condition_variable signal_;
    boost::mutex mutex_;

    boost::filesystem::ifstream stream_;
    unsigned int fileSize_;

    std::queue<ReadInformation> queue_;

    ReadCallback readCallback_;
};

}
}

#endif
