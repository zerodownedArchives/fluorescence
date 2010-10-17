
#include <data/weakptrcache.hpp>
#include <data/fixedsizeondemandfileloader.hpp>
#include <data/mapblock.hpp>


int main(int argc, char** argv) {
    //uome::data::FixedSizeOnDemandFileLoader<uome::data::RawMapBlock> loader("/data/games/uo2d/map0.mul", 196);
    //uome::data::WeakPtrCache<uome::data::RawMapBlock, uome::data::FixedSizeOnDemandFileLoader> myCache(&loader);


    //boost::shared_ptr<uome::data::RawMapBlock> block1 = myCache.get(0);
    //boost::shared_ptr<uome::data::RawMapBlock> block2 = myCache.get(1);
    //boost::shared_ptr<uome::data::RawMapBlock> block3 = myCache.get(2);

    //for (int i = 0; i < 4; i++) {
        //printf("\tblock2->tiles[%i], id=%i z=%i\n", i, block2->tiles_[i].tileId_, block2->tiles_[i].z_);
    //}

    //sleep(1);

    //for (int i = 0; i < 4; i++) {
        //printf("\tblock2->tiles[%i], id=%i z=%i\n", i, block2->tiles_[i].tileId_, block2->tiles_[i].z_);
    //}

    //// fix reference to block 3
    //printf("fixing reference to block 3\n");
    //myCache.fixItem(block3);

    //// remove reference to first block
    //printf("removing reference\n");
    //block1 = block2;
    //block3 = block2;

    //sleep(1);

    //printf("reloading 0\n");
    //boost::shared_ptr<uome::data::RawMapBlock> block1c = myCache.get(0);

    //printf("reloading 3\n");
    //boost::shared_ptr<uome::data::RawMapBlock> block3c = myCache.get(2);

    return 0;
}
