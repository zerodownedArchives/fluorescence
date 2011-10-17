
#include "singletextureprovider.hpp"

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/gumpartloader.hpp>

namespace fluo {
namespace ui {

SingleTextureProvider::SingleTextureProvider(unsigned int textureSource, unsigned int id) {
    switch (textureSource) {
    case FROM_ART_MUL:
        texture_ = data::Manager::getArtLoader()->getItemTexture(id);
        break;
    case FROM_GUMPART_MUL:
        texture_ = data::Manager::getGumpArtLoader()->getTexture(id);
        break;
    }
}

boost::shared_ptr<ui::Texture> SingleTextureProvider::getTexture() const {
    return texture_;
}

bool SingleTextureProvider::update(unsigned int elapsedMillis) {
    // do nothing
    return false;
}

}
}
