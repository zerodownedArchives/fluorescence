#ifndef FLUO_DATA_ONDEMANDREADABLE_HPP
#define FLUO_DATA_ONDEMANDREADABLE_HPP

namespace fluo {
namespace data {

class OnDemandReadable {
public:
    OnDemandReadable() : readComplete_(false) { }
    void setReadComplete() { readComplete_ = true; }
    bool isReadComplete() { return readComplete_; }

private:
    bool readComplete_;
};

}
}

#endif
