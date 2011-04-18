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

    unsigned int centerX_;
    unsigned int centerY_;
    boost::shared_ptr<ui::Texture> texture_;
};

class Animation : public uome::data::OnDemandReadable {
public:
    void addFrame(const AnimationFrame& frame);

    unsigned int getFrameCount();

    AnimationFrame getFrame(unsigned int idx);

    unsigned int getWidth(unsigned int idx);
    unsigned int getHeight(unsigned int idx);

    bool hasPixel(unsigned int idx, unsigned int pixelX, unsigned int pixelY);

private:
    std::vector<AnimationFrame> frames_;
};

}
}

#endif
