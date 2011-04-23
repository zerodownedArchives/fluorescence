#ifndef UOME_UI_SINGLETEXTUREPROVIDER_HPP
#define UOME_UI_SINGLETEXTUREPROVIDER_HPP

#include <boost/shared_ptr.hpp>

#include "textureprovider.hpp"
#include "texture.hpp"

namespace uome {
namespace ui {

class SingleTextureProvider : public TextureProvider {
public:
    SingleTextureProvider(unsigned int artId);

    virtual boost::shared_ptr<ui::Texture> getTexture() const;

    virtual bool update(unsigned int elapsedMillis);

private:
    boost::shared_ptr<ui::Texture> texture_;
};

}
}

#endif
