#ifndef FLUO_UI_TEXTUREPROVIDER_HPP
#define FLUO_UI_TEXTUREPROVIDER_HPP

namespace fluo {
namespace ui {

class Texture;

class TextureProvider {
public:

    virtual boost::shared_ptr<ui::Texture> getTexture() const = 0;

    /// returns true if the frame changed
    virtual bool update(unsigned int elapsedMillis) = 0;
};

}
}

#endif
