#ifndef FLUO_UI_PARTICLES_STARTPOSITIONPROVIDER_HPP
#define FLUO_UI_PARTICLES_STARTPOSITIONPROVIDER_HPP

#include <ClanLib/Core/Math/vec3.h>

namespace fluo {
namespace ui {
namespace particles {

class StartPositionProvider {
public:
    virtual CL_Vec3f get(const CL_Vec3f& emitterPosition) const = 0;
    virtual void setNormalizedAge(float age) = 0;
};


class StartPositionProviderEmitter {
public:
    virtual CL_Vec3f get(const CL_Vec3f& emitterPosition) const;
    virtual void setNormalizedAge(float age);
};


class StartPositionProviderWithSize : public StartPositionProvider {
public:
    void setSize(float widthStart, float widthEnd, float heightStart, float heightEnd);
    virtual void setNormalizedAge(float age);

protected:
    float curWidthHalf_;
    float curHeightHalf_;

private:
    float widthHalfStart_;
    float widthHalfEnd_;
    float heightHalfStart_;
    float heightHalfEnd_;
};

class StartPositionProviderRandomBox : public StartPositionProviderWithSize {
public:
    virtual CL_Vec3f get(const CL_Vec3f& emitterPosition) const;
};

class StartPositionProviderRandomBoxOutline : public StartPositionProviderWithSize {
public:
    virtual CL_Vec3f get(const CL_Vec3f& emitterPosition) const;
};


class StartPositionProviderRandomOval : public StartPositionProviderWithSize {
public:
    virtual CL_Vec3f get(const CL_Vec3f& emitterPosition) const;
};

class StartPositionProviderRandomOvalOutline : public StartPositionProviderWithSize {
public:
    virtual CL_Vec3f get(const CL_Vec3f& emitterPosition) const;
};

}
}
}


#endif
