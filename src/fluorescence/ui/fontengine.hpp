#ifndef FLUO_UI_FONTENGINE_HPP
#define FLUO_UI_FONTENGINE_HPP

#include <list>
#include <boost/shared_ptr.hpp>
#include <ClanLib/Display/Font/font.h>
#include <ClanLib/Display/Render/graphic_context.h>

#include <misc/config.hpp>
#include <misc/string.hpp>

namespace fluo {
namespace ui {

class Texture;

class FontEngine {
public:
    FontEngine(Config& config, CL_GraphicContext& gc);

    // If parameter useRgbColor is true, the color value is interpreted as a 32bit rgba value. If false, like a uo hue id
    boost::shared_ptr<ui::Texture> getDefaultTexture(const UnicodeString& text, unsigned int maxWidth, uint32_t color, bool useRgbColor = true, unsigned int borderWidth = 1, uint32_t borderColor = 0x000000FF);

    boost::shared_ptr<ui::Texture> getUniFontTexture(unsigned int uniFontId, const UnicodeString& text, unsigned int maxWidth, uint32_t color, bool useRgbColor = true, unsigned int borderWidth = 1, uint32_t borderColor = 0x000000FF);
    boost::shared_ptr<ui::Texture> getFontTexture(CL_Font& font, const UnicodeString& text, unsigned int maxWidth, uint32_t color, bool useRgbColor = true, unsigned int borderWidth = 1, uint32_t borderColor = 0x000000FF);

private:
    bool useDefaultUniFont_;
    unsigned int defaultUniFont_;
    CL_Font defaultFont_;

    static const unsigned int uniCharSpacing_ = 1;
    static const unsigned int uniLineSpacing_ = 0;

    void applyBorder(boost::shared_ptr<CL_PixelBuffer> tex, uint32_t color, unsigned int borderWidth, uint32_t borderColor);

    void calculateSizeAndLinebreaks(unsigned int fontId, const UnicodeString& text, unsigned int maxWidth, unsigned int borderWidth,
        unsigned int& width, unsigned int& height, std::list<unsigned int>& lineBreakIndices);
    UnicodeString calculateSizeAndLinebreaks(CL_Font& font, const UnicodeString& text, unsigned int maxWidth, unsigned int borderWidth,
        unsigned int& width, unsigned int& height);
};

}
}

#endif
