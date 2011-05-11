#ifndef UOME_UI_ANIMTEXTUREPROVIDER_HPP
#define UOME_UI_ANIMTEXTUREPROVIDER_HPP

#include <boost/shared_ptr.hpp>

#include <vector>
#include <map>

#include <data/animdataloader.hpp>

#include "textureprovider.hpp"
#include "texture.hpp"
#include "animation.hpp"

namespace uome {
namespace ui {

class AnimTextureProvider : public TextureProvider {
public:
    /// loop animation forever
    static const unsigned int REPEAT_MODE_LOOP = 0;

    /// play animation once, then stick with the last frame
    static const unsigned int REPEAT_MODE_LAST = 1;

    /// play animation once, then switch to to default frame
    static const unsigned int REPEAT_MODE_DEFAULT = 2;


    AnimTextureProvider(unsigned int animId);

    virtual boost::shared_ptr<ui::Texture> getTexture() const;
    AnimationFrame getCurrentFrame() const;

    virtual bool update(unsigned int elapsedMillis);

    void setRepeatMode(unsigned int mode);

    void setDirection(unsigned int direction);
    void setAnimId(unsigned int animId);

private:
    // stores for each anim (walk, run, ...) the frames for all directions
    std::map<unsigned int, std::vector<boost::shared_ptr<Animation> > > animations_;
    unsigned int bodyId_;
    unsigned int currentAnimId_;
    unsigned int direction_;
    unsigned int currentIdx_;
    unsigned long millis_;
    unsigned int frameMillis_;
    unsigned int repeatMode_;

    unsigned int defaultAnimId_;
    unsigned int nextAnimId_;
};

}
}

#endif
