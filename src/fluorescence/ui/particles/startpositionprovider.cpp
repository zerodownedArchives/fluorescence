
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
    widthHalf_ = InterpolatedValue<float>(widthStart / 2, widthEnd / 2);
    heightHalf_ =  InterpolatedValue<float>(heightStart / 2, heightEnd / 2);
}

void StartPositionProviderWithSize::setSizeT1(float width, float height) {
    widthHalf_.setT1(width / 2);
    heightHalf_.setT1(height / 2);
}

void StartPositionProviderWithSize::setNormalizedAge(float age) {
    widthHalf_.setNormalizedAge(age);
    heightHalf_.setNormalizedAge(age);
}


CL_Vec3f StartPositionProviderBox::get(const CL_Vec3f& emitterPosition) const {
    CL_Vec3f ret(emitterPosition);

    ret.x += Random::randomMinMax(-widthHalf_.get(), widthHalf_.get());
    ret.y += Random::randomMinMax(-heightHalf_.get(), heightHalf_.get());

    return ret;
}


CL_Vec3f StartPositionProviderBoxOutline::get(const CL_Vec3f& emitterPosition) const {
    CL_Vec3f ret(emitterPosition);

    if (Random::randomBool()) {
        ret.x += Random::randomBool() ? -heightHalf_.get() : heightHalf_.get();
        ret.y += Random::randomMinMax(-widthHalf_.get(), widthHalf_.get());
    } else {
        ret.x += Random::randomMinMax(-heightHalf_.get(), heightHalf_.get());
        ret.y += Random::randomBool() ? -widthHalf_.get() : widthHalf_.get();
    }

    return ret;
}


CL_Vec3f StartPositionProviderOval::get(const CL_Vec3f& emitterPosition) const {
    CL_Vec3f ret(emitterPosition);

    float theta = Random::random01() * 6.283185;
    float length = Random::random01();

    ret.x += cos(theta) * widthHalf_.get() * length;
    ret.y += sin(theta) * heightHalf_.get() * length;

    return ret;
}


CL_Vec3f StartPositionProviderOvalOutline::get(const CL_Vec3f& emitterPosition) const {
    CL_Vec3f ret(emitterPosition);

    float theta = Random::random01() * 6.283185;

    ret.x += cos(theta) * widthHalf_.get();
    ret.y += sin(theta) * heightHalf_.get();

    return ret;
}

}
}
}
