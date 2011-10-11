
#include "ingameobjectrenderer.hpp"


namespace fluo {
namespace ui {

IngameObjectRenderer::IngameObjectRenderer(bool polling) : polling_(polling) {
}

bool IngameObjectRenderer::isPolling() const {
    return polling_;
}

}
}
