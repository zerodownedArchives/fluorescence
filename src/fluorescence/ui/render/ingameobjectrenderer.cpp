
#include "ingameobjectrenderer.hpp"


namespace fluo {
namespace ui {

IngameObjectRenderer::IngameObjectRenderer(unsigned int rendererType, bool polling) : rendererType_(rendererType), polling_(polling) {
}

bool IngameObjectRenderer::isPolling() const {
    return polling_;
}

bool IngameObjectRenderer::isWorldRenderer() const {
    return rendererType_ == TYPE_WORLD;
}

bool IngameObjectRenderer::isGumpRenderer() const {
    return rendererType_ == TYPE_GUMP;
}

}
}
