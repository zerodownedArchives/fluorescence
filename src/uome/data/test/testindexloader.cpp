
#include <stdio.h>

#include <data/fullfileloader.hpp>
#include <data/indexblock.hpp>
#include <data/tiledata.hpp>

int main(int argc, char** argv) {
    uome::data::FullFileLoader<uome::data::IndexBlock> indexLoader("/data/games/uo2d/artidx.mul");

    printf("Index loaded (it seems)...\n");
    const uome::data::IndexBlock* first = indexLoader.get(0);

    printf("First offset=%u length=%u extra=%x\n", first->offset_, first->length_, first->extra_);


    printf("sizeof LandTile=%u Block=%u StaticTile=%u Block=%u\n", sizeof(uome::data::LandTileRecord), sizeof(uome::data::LandTileBlock),
            sizeof(uome::data::StaticTileRecord), sizeof(uome::data::StaticTileBlock));

    uome::data::FullFileLoader<uome::data::LandTileBlock> mapTileLoader("/data/games/uo2d/tiledata.mul", 836, 0, 428032);
    printf("%u land blocks loaded\n", mapTileLoader.size());

    uome::data::FullFileLoader<uome::data::StaticTileBlock> staticTileLoader("/data/games/uo2d/tiledata.mul", 1188, 428032, 0);
    printf("%u static blocks loaded\n", staticTileLoader.size());


    return 0;
}

