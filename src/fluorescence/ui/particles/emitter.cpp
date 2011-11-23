
#include "emitter.hpp"

#include <algorithm>

namespace fluo {
namespace ui {
namespace particles {

void Emitter::update(unsigned int elapsedMillis) {
    float elapsedSeconds = ((float)elapsedMillis / 1000.f) + elapsedSecondsStore_;
    age_ += elapsedSeconds;
    unsigned int newCount = elapsedSeconds * emitPerSecond_;

    newCount = (std::min)(emittedMaxCount_ - emittedCount(), newCount);

    if (newCount > 0) {
        emit(newCount);
        elapsedSecondsStore_ = 0.f;
    } else {
        elapsedSecondsStore_ += elapsedSeconds;
    }

    onUpdate(elapsedMillis);
}

bool Emitter::isExpired() const {
    return age_ > lifetimeSeconds_;
}

}
}
}
