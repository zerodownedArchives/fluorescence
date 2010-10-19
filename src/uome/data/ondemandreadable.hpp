#ifndef UOME_DATA_ONDEMANDREADABLE_HPP
#define UOME_DATA_ONDEMANDREADABLE_HPP

namespace uome {
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
