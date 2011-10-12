
#include "overheadmessage.hpp"

#include <ui/texture.hpp>
#include <ui/manager.hpp>
#include <ui/fontengine.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace world {

OverheadMessage::OverheadMessage(boost::shared_ptr<IngameObject> parent, const UnicodeString& text, unsigned int font, unsigned int color, bool useRgbColor) :
            parent_(parent) {
    texture_ = ui::Manager::getFontEngine()->getUniFontTexture(font, text, 180, color, useRgbColor);

    addToRenderQueue(ui::Manager::getWorldRenderQueue());
}

boost::shared_ptr<ui::Texture> OverheadMessage::getIngameTexture() const {
    return texture_;
}

void OverheadMessage::updateVertexCoordinates() {
    // do nothing
    CL_Vec2f parentCoords = parent_->getVertexCoordinates()[0];

    int x = parentCoords.x + 22 - texture_->getWidth()/2;
    int y = parentCoords.y + parentPixelOffsetY_;

    CL_Rectf rect(x, y, CL_Sizef(texture_->getWidth(), texture_->getHeight()));

    vertexCoordinates_[0] = CL_Vec2f(rect.left, rect.top);
    vertexCoordinates_[1] = CL_Vec2f(rect.right, rect.top);
    vertexCoordinates_[2] = CL_Vec2f(rect.left, rect.bottom);
    vertexCoordinates_[3] = CL_Vec2f(rect.right, rect.top);
    vertexCoordinates_[4] = CL_Vec2f(rect.left, rect.bottom);
    vertexCoordinates_[5] = CL_Vec2f(rect.right, rect.bottom);
}

void OverheadMessage::updateRenderPriority() {
    // do nothing
    renderPriority_[0] = 0xFFFFFFF;
}

void OverheadMessage::updateTextureProvider() {
    // do nothing
}

bool OverheadMessage::updateAnimation(unsigned int elapsedMillis) {
    // do nothing
    return false;
}

void OverheadMessage::onClick() {
    LOG_DEBUG << "Overhead message clicked" << std::endl;
}

void OverheadMessage::setParentPixelOffset(int y) {
    parentPixelOffsetY_ = y;

    invalidateRenderData();
}

}
}
