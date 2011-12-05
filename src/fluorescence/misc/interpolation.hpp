#ifndef FLUO_INTERPOLATION_HPP
#define FLUO_INTERPOLATION_HPP

#include <ClanLib/Core/Math/vec3.h>
#include <ClanLib/Core/Math/vec4.h>

namespace fluo {

class Interpolation {
public:
    static float linear(float a, float b, float factor);
    static CL_Vec3f linear(const CL_Vec3f& a, const CL_Vec3f& b, float factor);
    static CL_Vec4f linear(const CL_Vec4f& a, const CL_Vec4f& b, float factor);
};

template<typename T>
class InterpolatedValue {
public:
    InterpolatedValue() {
    }

    InterpolatedValue(const T& t0, const T& t1) : valueT0_(t0),valueT1_(t1), valueCur_(t0) {
    }

    void set(const T& t0, const T& t1) {
        valueT0_ = t0;
        valueT1_ = t1;
    }

    void setT0(const T& t0) {
        valueT0_ = t0;
    }

    void setT1(const T& t1) {
        valueT1_ = t1;
    }

    const T& get() const {
        return valueCur_;
    }

    void setNormalizedAge(float age) {
        valueCur_ = Interpolation::linear(valueT0_, valueT1_, age);
    }

    operator T() const {
        return valueCur_;
    }

private:
    T valueT0_;
    T valueT1_;
    T valueCur_;
};

}

#endif
