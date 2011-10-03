#ifndef FLUO_DATA_TILEDATALOADER_HPP
#define FLUO_DATA_TILEDATALOADER_HPP

#include <misc/string.hpp>
#include <misc/exception.hpp>

#include <boost/filesystem.hpp>

namespace fluo {
namespace data {

#define FLUO_FLAG_GETTER(_flagName, _flagValue)     bool _flagName() const { return (flags_ & _flagValue) != 0; }

struct LandTileInfo {
    uint32_t flags_;
    uint16_t textureId_;
    UnicodeString name_;

    FLUO_FLAG_GETTER(background,    0x00000001)
    FLUO_FLAG_GETTER(weapon,        0x00000002)
    FLUO_FLAG_GETTER(transparent,   0x00000004)
    FLUO_FLAG_GETTER(translucent,   0x00000008)
    FLUO_FLAG_GETTER(wall,          0x00000010)
    FLUO_FLAG_GETTER(damaging,      0x00000020)
    FLUO_FLAG_GETTER(impassable,    0x00000040)
    FLUO_FLAG_GETTER(liquid,        0x00000080)

    FLUO_FLAG_GETTER(unknown1,      0x00000100)
    FLUO_FLAG_GETTER(surface,       0x00000200)
    FLUO_FLAG_GETTER(bridge,        0x00000400)
    FLUO_FLAG_GETTER(generic,       0x00000800)
    FLUO_FLAG_GETTER(window,        0x00001000)
    FLUO_FLAG_GETTER(noShoot,       0x00002000)
    FLUO_FLAG_GETTER(articleA,      0x00004000)
    FLUO_FLAG_GETTER(articleAn,     0x00008000)

    FLUO_FLAG_GETTER(mongen,        0x00010000)
    FLUO_FLAG_GETTER(foliage,       0x00020000)
    FLUO_FLAG_GETTER(partialHue,    0x00040000)
    FLUO_FLAG_GETTER(unknown2,      0x00080000)
    FLUO_FLAG_GETTER(map,           0x00100000)
    FLUO_FLAG_GETTER(container,     0x00200000)
    FLUO_FLAG_GETTER(wearable,      0x00400000)
    FLUO_FLAG_GETTER(lightSource,   0x00800000)

    FLUO_FLAG_GETTER(animation,     0x01000000)
    FLUO_FLAG_GETTER(noDiagonal,    0x02000000)
    FLUO_FLAG_GETTER(unknown3,      0x04000000)
    FLUO_FLAG_GETTER(armor,         0x08000000)
    FLUO_FLAG_GETTER(roof,          0x10000000)
    FLUO_FLAG_GETTER(door,          0x20000000)
    FLUO_FLAG_GETTER(stairBack,     0x40000000)
    FLUO_FLAG_GETTER(stairRight,    0x80000000)
};

struct StaticTileInfo {
    uint32_t flags_;
    uint8_t weight_;
    uint8_t quality_;
    uint16_t unknown1_;
    uint8_t unknown2_;
    uint8_t quantity_;
    uint16_t animId_;
    uint8_t unknown3_;
    uint8_t hue_;
    uint8_t unknown4_;
    uint8_t unknown5_;
    uint8_t height_;
    UnicodeString name_;

    FLUO_FLAG_GETTER(background,    0x00000001)
    FLUO_FLAG_GETTER(weapon,        0x00000002)
    FLUO_FLAG_GETTER(transparent,   0x00000004)
    FLUO_FLAG_GETTER(translucent,   0x00000008)
    FLUO_FLAG_GETTER(wall,          0x00000010)
    FLUO_FLAG_GETTER(damaging,      0x00000020)
    FLUO_FLAG_GETTER(impassable,    0x00000040)
    FLUO_FLAG_GETTER(liquid,        0x00000080)

    FLUO_FLAG_GETTER(unknown1,      0x00000100)
    FLUO_FLAG_GETTER(surface,       0x00000200)
    FLUO_FLAG_GETTER(bridge,        0x00000400)
    FLUO_FLAG_GETTER(generic,       0x00000800)
    FLUO_FLAG_GETTER(window,        0x00001000)
    FLUO_FLAG_GETTER(noShoot,       0x00002000)
    FLUO_FLAG_GETTER(articleA,      0x00004000)
    FLUO_FLAG_GETTER(articleAn,     0x00008000)

    FLUO_FLAG_GETTER(mongen,        0x00010000)
    FLUO_FLAG_GETTER(foliage,       0x00020000)
    FLUO_FLAG_GETTER(partialHue,    0x00040000)
    FLUO_FLAG_GETTER(unknown2,      0x00080000)
    FLUO_FLAG_GETTER(map,           0x00100000)
    FLUO_FLAG_GETTER(container,     0x00200000)
    FLUO_FLAG_GETTER(wearable,      0x00400000)
    FLUO_FLAG_GETTER(lightSource,   0x00800000)

    FLUO_FLAG_GETTER(animation,     0x01000000)
    FLUO_FLAG_GETTER(noDiagonal,    0x02000000)
    FLUO_FLAG_GETTER(unknown3,      0x04000000)
    FLUO_FLAG_GETTER(armor,         0x08000000)
    FLUO_FLAG_GETTER(roof,          0x10000000)
    FLUO_FLAG_GETTER(door,          0x20000000)
    FLUO_FLAG_GETTER(stairBack,     0x40000000)
    FLUO_FLAG_GETTER(stairRight,    0x80000000)
};

#undef FLUO_FLAG_GETTER

class TileDataLoader {
public:
    enum { LAND_TILE_COUNT = 0x4000 };

    TileDataLoader(const boost::filesystem::path& path);
    ~TileDataLoader();

    const LandTileInfo* getLandTileInfo(unsigned int id);

    const StaticTileInfo* getStaticTileInfo(unsigned int id);

    void read(int8_t* buf, unsigned int len);

private:
    unsigned int landTileCount_;
    LandTileInfo* landTileInfos_;

    unsigned int staticTileCount_;
    StaticTileInfo* staticTileInfos_;
};

}
}

#endif
