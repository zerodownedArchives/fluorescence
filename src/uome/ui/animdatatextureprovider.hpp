#ifndef UOME_UI_ANIMDATATEXTUREPROVIDER_HPP
#define UOME_UI_ANIMDATATEXTUREPROVIDER_HPP

#include <boost/shared_ptr.hpp>

#include <vector>

#include <data/animdataloader.hpp>

#include "textureprovider.hpp"
#include "texture.hpp"

namespace uome {
namespace ui {

class AnimDataTextureProvider : public TextureProvider {
public:
    AnimDataTextureProvider(unsigned int artId);

    virtual boost::shared_ptr<ui::Texture> getTexture();

    virtual void update(unsigned int elapsedMillis);

private:
    std::vector<boost::shared_ptr<ui::Texture> > textures_;
    unsigned int currentIdx_;
    unsigned long millis_;
    data::AnimDataInfo info_;
};

}
}

#endif
