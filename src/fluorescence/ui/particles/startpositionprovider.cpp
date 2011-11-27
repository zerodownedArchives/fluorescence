
#include "startpositionprovider.hpp"

#include <stdlib.h>

#include <misc/random.hpp>

namespace fluo {
namespace ui {
namespace particles {

CL_Vec3f StartPositionProviderEmitter::get(const CL_Vec3f& emitterPosition) const {
    return emitterPosition;
}

void StartPositionProviderEmitter::setNormalizedAge(float age) {
    // do nothing
}


void StartPositionProviderWithSize::setSize(float widthStart, float widthEnd, float heightStart, float heightEnd) {
    widthHalfStart_ = widthStart / 2;
    widthHalfEnd_ = widthEnd / 2;
    heightHalfStart_ = heightStart / 2;
    heightHalfEnd_ = heightEnd / 2;
}

void StartPositionProviderWithSize::setNormalizedAge(float age) {
    curWidthHalf_ = widthHalfStart_ + (widthHalfEnd_ - widthHalfStart_) * age;
    curHeightHalf_ = heightHalfStart_ + (heightHalfEnd_ - heightHalfStart_) * age;
}


CL_Vec3f StartPositionProviderBox::get(const CL_Vec3f& emitterPosition) const {
    CL_Vec3f ret(emitterPosition);

    ret.x += Random::randomMinMax(-curWidthHalf_, curWidthHalf_);
    ret.y += Random::randomMinMax(-curHeightHalf_, curHeightHalf_);

    return ret;
}


CL_Vec3f StartPositionProviderBoxOutline::get(const CL_Vec3f& emitterPosition) const {
    CL_Vec3f ret(emitterPosition);

    if (Random::randomBool()) {
        ret.x += Random::randomBool() ? -curHeightHalf_ : curHeightHalf_;
        ret.y += Random::randomMinMax(-curWidthHalf_, curWidthHalf_);
    } else {
        ret.x += Random::randomMinMax(-curHeightHalf_, curHeightHalf_);
        ret.y += Random::randomBool() ? -curWidthHalf_ : curWidthHalf_;
    }

    return ret;
}


CL_Vec3f StartPositionProviderOval::get(const CL_Vec3f& emitterPosition) const {
    CL_Vec3f ret(emitterPosition);

    float theta = Random::random01() * 6.283185;
    float length = Random::random01();

    ret.x += cos(theta) * curWidthHalf_ * length;
    ret.y += sin(theta) * curHeightHalf_ * length;

    return ret;
}


CL_Vec3f StartPositionProviderOvalOutline::get(const CL_Vec3f& emitterPosition) const {
    CL_Vec3f ret(emitterPosition);

    float theta = Random::random01() * 6.283185;

    ret.x += cos(theta) * curWidthHalf_;
    ret.y += sin(theta) * curHeightHalf_;

    return ret;
}

}
}
}
