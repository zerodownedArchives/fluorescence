
#include "parameter.hpp"



namespace fluo {
namespace ui {
namespace particles {

Parameter::Parameter(float value) : value_(value), min_(value), max_(value) {
}

Parameter::Parameter(float min, float max) : min_(min), max_(max) {
    setRandom();
}

Parameter::Parameter(const Parameter& copy) : min_(copy.min_), max_(copy.max_) {
    setRandom();
}

Parameter& Parameter::operator=(const Parameter& copy) {
    min_ = copy.min_;
    max_ = copy.max_;

    setRandom();

    return *this;
}

void Parameter::setRandom() {
    if (min_ != max_) {
        value_ = min_ + ((float)rand() / RAND_MAX) * (max_ - min_);
    } else {
        value_ = min_;
    }
}

}
}
}
