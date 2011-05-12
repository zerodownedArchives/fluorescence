#ifndef UOME_WORLD_MOBILE_HPP
#define UOME_WORLD_MOBILE_HPP


#include "serverobject.hpp"

#include <ui/animtextureprovider.hpp>

namespace uome {

namespace ui {
    class Texture;
}


namespace world {

class Mobile : public ServerObject {
public:
    Mobile(Serial serial);

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const;

    unsigned int getBodyId() const;
    void setBodyId(unsigned int value);

    void setHue(unsigned int value);

    virtual void onClick();
    virtual void onDoubleClick();

    void playAnim(unsigned int animId);

    virtual bool isMirrored() const;
    void setDirection(unsigned int direction);

private:
    unsigned int bodyId_;
    unsigned int hue_;
    unsigned int direction_;
    bool isRunning_;

    void updateVertexCoordinates();
    void updateRenderPriority();
    void updateTextureProvider();
    bool updateAnimation(unsigned int elapsedMillis);

    boost::shared_ptr<ui::AnimTextureProvider> textureProvider_;
};

}
}

#endif
