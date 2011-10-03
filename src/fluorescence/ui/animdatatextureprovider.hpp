#ifndef FLUO_UI_ANIMDATATEXTUREPROVIDER_HPP
#define FLUO_UI_ANIMDATATEXTUREPROVIDER_HPP

#include <boost/shared_ptr.hpp>

#include <vector>

#include <data/animdataloader.hpp>

#include "textureprovider.hpp"
#include "texture.hpp"

namespace fluo {
namespace ui {

class AnimDataTextureProvider : public TextureProvider {
public:
    AnimDataTextureProvider(unsigned int artId);

    virtual boost::shared_ptr<ui::Texture> getTexture() const;

    virtual bool update(unsigned int elapsedMillis);

private:
    std::vector<boost::shared_ptr<ui::Texture> > textures_;
    unsigned int currentIdx_;
    unsigned long millis_;
    data::AnimDataInfo info_;
};

}
}

#endif
