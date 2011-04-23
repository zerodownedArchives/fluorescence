#ifndef UOME_UI_TEXTURESEQUENCE_HPP
#define UOME_UI_TEXTURESEQUENCE_HPP

#include <boost/shared_ptr.hpp>

#include <data/ondemandreadable.hpp>

#include <stdint.h>

#include "texture.hpp"

namespace uome {
namespace ui {

struct AnimationFrame {
    AnimationFrame() : centerX_(0), centerY_(0), texture_(new ui::Texture) { }

    int centerX_;
    int centerY_;
    boost::shared_ptr<ui::Texture> texture_;
};

class Animation : public uome::data::OnDemandReadable {
public:
    void addFrame(const AnimationFrame& frame);

    unsigned int getFrameCount() const;

    AnimationFrame getFrame(unsigned int idx) const;

    unsigned int getWidth(unsigned int idx) const;
    unsigned int getHeight(unsigned int idx) const;

    bool hasPixel(unsigned int idx, unsigned int pixelX, unsigned int pixelY) const;

private:
    std::vector<AnimationFrame> frames_;
};

}
}

#endif
