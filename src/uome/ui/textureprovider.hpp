#ifndef UOME_UI_TEXTUREPROVIDER_HPP
#define UOME_UI_TEXTUREPROVIDER_HPP

namespace uome {
namespace ui {

class Texture;

class TextureProvider {
public:

    virtual boost::shared_ptr<ui::Texture> getTexture() = 0;

    virtual void update(unsigned int elapsedMillis) = 0;
};

}
}

#endif
