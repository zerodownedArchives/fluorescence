#ifndef UOME_UI_ANIMTEXTUREPROVIDER_HPP
#define UOME_UI_ANIMTEXTUREPROVIDER_HPP

#include <boost/shared_ptr.hpp>

#include <vector>

#include <data/animdataloader.hpp>

#include "textureprovider.hpp"
#include "texture.hpp"
#include "animation.hpp"

namespace uome {
namespace ui {

class AnimTextureProvider : public TextureProvider {
public:
    AnimTextureProvider(unsigned int animId);

    virtual boost::shared_ptr<ui::Texture> getTexture();

    virtual void update(unsigned int elapsedMillis);

private:
    std::vector<boost::shared_ptr<Animation> > animations_;
    unsigned int currentAnimId_;
    unsigned int currentIdx_;
    unsigned long millis_;
    unsigned int frameMillis_;
};

}
}

#endif
