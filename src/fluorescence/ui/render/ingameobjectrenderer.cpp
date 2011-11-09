
#include "ingameobjectrenderer.hpp"


namespace fluo {
namespace ui {

IngameObjectRenderer::IngameObjectRenderer(unsigned int rendererType) : rendererType_(rendererType), requireInitialRepaint_(true) {
}

bool IngameObjectRenderer::isWorldRenderer() const {
    return rendererType_ == TYPE_WORLD;
}

bool IngameObjectRenderer::isGumpRenderer() const {
    return rendererType_ == TYPE_GUMP;
}

bool IngameObjectRenderer::requireInitialRepaint() {
    bool ret = requireInitialRepaint_;
    requireInitialRepaint_ = false;
    return ret;
}

}
}
