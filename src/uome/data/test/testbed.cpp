

#include <data/tiledataloader.hpp>

int main(int argc, char** argv) {
    uome::data::TileDataLoader ldr("/data/games/uo2d/tiledata.mul");

    const uome::data::LandTileInfo* inf = ldr.getLandTileInfo(13);
    inf->background();

    return 0;
}
