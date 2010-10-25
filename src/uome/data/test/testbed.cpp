

#include <data/artloader.hpp>

#include <ui/texture.hpp>

int main(int argc, char** argv) {
    uome::data::ArtLoader ldr("/data/games/uo2d/artidx.mul", "/data/games/uo2d/art.mul");

    boost::shared_ptr<uome::ui::Texture> tex = ldr.getMapTexture(4);

    return 0;
}
