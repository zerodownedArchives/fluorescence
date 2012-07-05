/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "stringparser.hpp"

#include <ClanLib/gui.h>
#include <unicode/regex.h>
#include <boost/bind.hpp>

#include <misc/log.hpp>
#include <data/manager.hpp>
#include <data/util.hpp>
#include <data/clilocloader.hpp>

#include "manager.hpp"
#include "fontengine.hpp"
#include "gumpmenu.hpp"
#include "components/background.hpp"
#include "components/image.hpp"
#include "components/uobutton.hpp"
#include "components/uocheckbox.hpp"
#include "components/textentry.hpp"
#include "components/label.hpp"
#include "components/uoradiobutton.hpp"

namespace fluo {
namespace ui {

StringParser* StringParser::singleton_ = NULL;

StringParser* StringParser::getSingleton() {
    if (!singleton_) {
        singleton_ = new StringParser();
    }
    return singleton_;
}

GumpMenu* StringParser::fromString(const UnicodeString& commands, const std::vector<UnicodeString>& strings) {
    return getSingleton()->innerFromString(commands, strings);
}

StringParser::StringParser() {
    functionTable_["nomove"] = boost::bind(&StringParser::setNoMove, this, _1, _2, _3);
    functionTable_["noclose"] = boost::bind(&StringParser::setNoClose, this, _1, _2, _3);
    // TODO: resizable, disposable

    functionTable_["page"] = boost::bind(&StringParser::parsePage, this, _1, _2, _3);
    functionTable_["resizepic"] = boost::bind(&StringParser::parseResizepic, this, _1, _2, _3);
    functionTable_["gumppictiled"] = boost::bind(&StringParser::parseGumpPicTiled, this, _1, _2, _3);
    functionTable_["croppedtext"] = boost::bind(&StringParser::parseCroppedText, this, _1, _2, _3);
    functionTable_["text"] = boost::bind(&StringParser::parseText, this, _1, _2, _3);
    functionTable_["gumppic"] = boost::bind(&StringParser::parseGumpPic, this, _1, _2, _3);
    functionTable_["tilepic"] = boost::bind(&StringParser::parseTilePic, this, _1, _2, _3);
    functionTable_["tilepichue"] = boost::bind(&StringParser::parseTilePicHue, this, _1, _2, _3);
    functionTable_["button"] = boost::bind(&StringParser::parseButton, this, _1, _2, _3);
    functionTable_["checkbox"] = boost::bind(&StringParser::parseCheckbox, this, _1, _2, _3);
    functionTable_["textentry"] = boost::bind(&StringParser::parseTextEntry, this, _1, _2, _3);
    functionTable_["htmlgump"] = boost::bind(&StringParser::parseHtmlGump, this, _1, _2, _3);
    functionTable_["xmfhtmlgump"] = boost::bind(&StringParser::parseXmfHtmlGump, this, _1, _2, _3);
    functionTable_["xmfhtmlgumpcolor"] = boost::bind(&StringParser::parseXmfHtmlGumpColor, this, _1, _2, _3);
    functionTable_["xmfhtmltok"] = boost::bind(&StringParser::parseXmfHtmlTok, this, _1, _2, _3);
    functionTable_["checkertrans"] = boost::bind(&StringParser::parseCheckerTrans, this, _1, _2, _3);
    functionTable_["radio"] = boost::bind(&StringParser::parseRadio, this, _1, _2, _3);
    functionTable_["group"] = boost::bind(&StringParser::parseGroup, this, _1, _2, _3);
}

GumpMenu* StringParser::innerFromString(const UnicodeString& commands, const std::vector<UnicodeString>& strings) {
    CL_Rect bounds(0, 0, 100, 100); // temporary foobar value

    CL_GUITopLevelDescription desc(bounds, false);
    desc.set_decorations(false);

    GumpMenu* ret = new GumpMenu(desc);
    ret->setDraggable(true);
    ret->setClosable(true);
    // TODO: defaults for resizable, disposable

    ret->set_id_name("nobackground");

    UErrorCode status = U_ZERO_ERROR;
    bool parseSuccess = true;

    static RegexMatcher matcher("\\{\\s*(\\w+)\\s*([^}]*)\\}", 0, status);
    matcher.reset(commands);

    while (matcher.find()) {
       UnicodeString curCmd = matcher.group(1, status);
       UnicodeString curParams = matcher.group(2, status);

        //LOG_INFO << "Regex match: " << curCmd << " :: " << curParams << std::endl;
        std::map<UnicodeString, StringParseFunction>::iterator function = functionTable_.find(curCmd);

        if (function != functionTable_.end()) {
            parseSuccess = (function->second)(curParams, strings, ret);
            if (!parseSuccess) {
                LOG_INFO << "Unable to parse gump: " << std::endl;
                LOG_INFO << commands << std::endl;
                LOG_INFO << "Additional lines: " << strings.size() << std::endl;
                for (unsigned int i = 0; i < strings.size(); ++i) {
                    LOG_INFO << "\t" << i << ": " << strings[i] << std::endl;
                }
                break;
            }
        } else {
            LOG_WARN << "Unknown gump string command " << curCmd << std::endl;
        }
    }

    ret->fitSizeToChildren();
    ret->setupResizeHandler();
    ret->activateFirstPage();

    //LOG_INFO << "Gump data: " << std::endl;
    //LOG_INFO << commands << std::endl;
    //LOG_INFO << "Additional lines: " << strings.size() << std::endl;
    //for (unsigned int i = 0; i < strings.size(); ++i) {
        //LOG_INFO << "\t" << i << ": " << strings[i] << std::endl;
    //}

    return ret;
}

bool StringParser::parsePage(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    // { page {0} }
    int pageId = StringConverter::toInt(params);
    menu->addPage(pageId);
    return true;
}

bool StringParser::parseResizepic(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    // { resizepic {0} {1} {2} {3} {4} }
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*", 0, status);
    matcher.reset(params);
    if (matcher.find() && matcher.groupCount() == 5) {
        int x = StringConverter::toInt(matcher.group(1, status));
        int y = StringConverter::toInt(matcher.group(2, status));
        int gumpId = StringConverter::toInt(matcher.group(3, status));
        int width = StringConverter::toInt(matcher.group(4, status));
        int height = StringConverter::toInt(matcher.group(5, status));

        components::Background* bg = new components::Background(menu);
        CL_Rectf bounds(x, y, CL_Sizef(width, height));
        bg->set_geometry(bounds);
        bg->setBaseId(gumpId);
        menu->addToCurrentPage(bg);

        return true;
    } else {
        LOG_ERROR << "Unable to parse resizepic, params " << params << std::endl;
        return false;
    }
}

bool StringParser::parseGumpPicTiled(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*", 0, status);
    matcher.reset(params);
    if (matcher.find() && matcher.groupCount() == 5) {
        int x = StringConverter::toInt(matcher.group(1, status));
        int y = StringConverter::toInt(matcher.group(2, status));
        int width = StringConverter::toInt(matcher.group(3, status));
        int height = StringConverter::toInt(matcher.group(4, status));
        int gumpId = StringConverter::toInt(matcher.group(5, status));

        boost::shared_ptr<ui::Texture> tex = data::Manager::getTexture(data::TextureSource::GUMPART, gumpId);
        if (!tex) {
            LOG_WARN << "Unable to display gumppictiled with gump id " << gumpId << std::endl;
            return true;
        }

        components::Image* img = new components::Image(menu);
        CL_Rectf bounds(x, y, CL_Sizef(width, height));
        img->set_geometry(bounds);
        img->setTiled(true);
        img->setTexture(tex);
        menu->addToCurrentPage(img);

        return true;
    } else {
        LOG_ERROR << "Unable to parse gumppictiled, params " << params << std::endl;
        return false;
    }
}

bool StringParser::parseCroppedText(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*", 0, status);
    matcher.reset(params);
    if (matcher.find() && matcher.groupCount() == 6) {
        int x = StringConverter::toInt(matcher.group(1, status));
        int y = StringConverter::toInt(matcher.group(2, status));
        unsigned int width = StringConverter::toInt(matcher.group(3, status));
        unsigned int height = StringConverter::toInt(matcher.group(4, status));
        int hue = StringConverter::toInt(matcher.group(5, status));
        int textId = StringConverter::toInt(matcher.group(6, status));

        boost::shared_ptr<ui::Texture> tex = ui::Manager::getFontEngine()->getUniFontTexture(1, strings[textId], 99999, hue, false, 0);
        if (width > tex->getWidth()) {
            width = tex->getWidth();
        }
        if (height > tex->getHeight()) {
            height = tex->getHeight();
        }

        components::Image* img = new components::Image(menu);
        CL_Rectf bounds(x, y, CL_Sizef(width, height));
        img->set_geometry(bounds);
        img->setTiled(true);
        img->setTexture(tex);
        menu->addToCurrentPage(img);

        return true;
    } else {
        LOG_ERROR << "Unable to parse croppedtext, params " << params << std::endl;
        return false;
    }
}

bool StringParser::parseText(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*", 0, status);
    matcher.reset(params);
    if (matcher.find() && matcher.groupCount() == 4) {
        int x = StringConverter::toInt(matcher.group(1, status));
        int y = StringConverter::toInt(matcher.group(2, status));
        int hue = StringConverter::toInt(matcher.group(3, status));
        int textId = StringConverter::toInt(matcher.group(4, status));

        boost::shared_ptr<ui::Texture> tex = ui::Manager::getFontEngine()->getUniFontTexture(1, strings[textId], 99999, hue, false, 0);

        components::Image* img = new components::Image(menu);
        CL_Rectf bounds(x, y, CL_Sizef(tex->getWidth(), tex->getHeight()));
        img->set_geometry(bounds);
        img->setTiled(true);
        img->setTexture(tex);
        menu->addToCurrentPage(img);

        return true;
    } else {
        LOG_ERROR << "Unable to parse text, params " << params << std::endl;
        return false;
    }
}

bool StringParser::parseGumpPic(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*", 0, status);
    static RegexMatcher matcherHue("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*hue=(\\w+)\\s*", 0, status);
    matcher.reset(params);
    matcherHue.reset(params);

    int x, y, gumpId;
    int hue = 0;
    if (matcherHue.find() && matcherHue.groupCount() == 4) {
        x = StringConverter::toInt(matcherHue.group(1, status));
        y = StringConverter::toInt(matcherHue.group(2, status));
        gumpId = StringConverter::toInt(matcherHue.group(3, status));
        hue = StringConverter::toInt(matcherHue.group(4, status));
    } else if (matcher.find() && matcher.groupCount() == 3) {
        x = StringConverter::toInt(matcher.group(1, status));
        y = StringConverter::toInt(matcher.group(2, status));
        gumpId = StringConverter::toInt(matcher.group(3, status));
    } else {
        LOG_ERROR << "Unable to parse gumppic, params " << params << std::endl;
        return false;
    }

    boost::shared_ptr<ui::Texture> tex = data::Manager::getTexture(data::TextureSource::GUMPART, gumpId);
    if (!tex) {
        LOG_WARN << "Unable to display gumppic with gump id " << gumpId << std::endl;
        return true;
    }

    components::Image* img = new components::Image(menu);
    CL_Rectf bounds(x, y, CL_Sizef(tex->getWidth(), tex->getHeight()));
    img->set_geometry(bounds);
    img->setTexture(tex);
    img->setHue(hue);
    img->setAutoResize(true);
    menu->addToCurrentPage(img);

    return true;
}

bool StringParser::parseTilePic(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*", 0, status);
    matcher.reset(params);

    if (matcher.find() && matcher.groupCount() == 3) {
        int x = StringConverter::toInt(matcher.group(1, status));
        int y = StringConverter::toInt(matcher.group(2, status));
        int artId = StringConverter::toInt(matcher.group(3, status));

        boost::shared_ptr<ui::Texture> tex = data::Manager::getTexture(data::TextureSource::STATICART, artId);
        if (!tex) {
            LOG_WARN << "Unable to display tilepic with art id " << artId << std::endl;
            return true;
        }
        components::Image* img = new components::Image(menu);
        CL_Rectf bounds(x, y, CL_Sizef(tex->getWidth(), tex->getHeight()));
        img->set_geometry(bounds);
        img->setTexture(tex);
        img->setAutoResize(true);
        menu->addToCurrentPage(img);

        return true;
    } else {
        LOG_ERROR << "Unable to parse tilepic, params " << params << std::endl;
        return false;
    }
}

bool StringParser::parseTilePicHue(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*", 0, status);
    matcher.reset(params);

    if (matcher.find() && matcher.groupCount() == 4) {
        int x = StringConverter::toInt(matcher.group(1, status));
        int y = StringConverter::toInt(matcher.group(2, status));
        int artId = StringConverter::toInt(matcher.group(3, status));
        int hue = StringConverter::toInt(matcher.group(4, status));

        boost::shared_ptr<ui::Texture> tex = data::Manager::getTexture(data::TextureSource::STATICART, artId);
        if (!tex) {
            LOG_WARN << "Unable to display tilepichue with art id " << artId << std::endl;
            return true;
        }

        components::Image* img = new components::Image(menu);
        CL_Rectf bounds(x, y, CL_Sizef(tex->getWidth(), tex->getHeight()));
        img->set_geometry(bounds);
        img->setTexture(tex);
        img->setHue(hue);
        img->setAutoResize(true);
        menu->addToCurrentPage(img);

        return true;
    } else {
        LOG_ERROR << "Unable to parse tilepichue, params " << params << std::endl;
        return false;
    }
}

bool StringParser::parseButton(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*", 0, status);
    matcher.reset(params);

    if (matcher.find() && matcher.groupCount() == 7) {
        int x = StringConverter::toInt(matcher.group(1, status));
        int y = StringConverter::toInt(matcher.group(2, status));
        int upId = StringConverter::toInt(matcher.group(3, status));
        int downId = StringConverter::toInt(matcher.group(4, status));
        int type = StringConverter::toInt(matcher.group(5, status));
        int pageId = StringConverter::toInt(matcher.group(6, status));
        int buttonId = StringConverter::toInt(matcher.group(7, status));

        boost::shared_ptr<ui::Texture> upTex = data::Manager::getTexture(data::TextureSource::GUMPART, upId);
        boost::shared_ptr<ui::Texture> downTex = data::Manager::getTexture(data::TextureSource::GUMPART, downId);

        if (!upTex) {
            LOG_ERROR << "Unable to add button with up id " << upId << std::endl;
            return true;
        }

        if (!downTex) {
            LOG_WARN << "Unable to set button down id " << downId << std::endl;
            return true;
        }

        components::UoButton* but = new components::UoButton(menu);
        CL_Rectf bounds(x, y, CL_Sizef(upTex->getWidth(), upTex->getHeight()));
        but->set_geometry(bounds);

        but->addTexture(components::UoButton::TEX_INDEX_UP, upTex);
        but->addTexture(components::UoButton::TEX_INDEX_MOUSEOVER, upTex);
        but->addTexture(components::UoButton::TEX_INDEX_DOWN, downTex);
        but->updateTexture();

        if (type == 0) {
            but->setPageButton(pageId);
        } else {
            but->setServerButton(buttonId);
        }

        but->setAutoResize(true);

        menu->addToCurrentPage(but);

        return true;
    } else {
        LOG_ERROR << "Unable to parse button, params " << params << std::endl;
        return false;
    }
}

bool StringParser::parseCheckbox(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*", 0, status);
    matcher.reset(params);

    if (matcher.find() && matcher.groupCount() == 6) {
        int x = StringConverter::toInt(matcher.group(1, status));
        int y = StringConverter::toInt(matcher.group(2, status));
        int uncheckedId = StringConverter::toInt(matcher.group(3, status));
        int checkedId = StringConverter::toInt(matcher.group(4, status));
        int checked = StringConverter::toInt(matcher.group(5, status));
        int switchId = StringConverter::toInt(matcher.group(6, status));

        boost::shared_ptr<ui::Texture> uncheckedTex = data::Manager::getTexture(data::TextureSource::GUMPART, uncheckedId);
        boost::shared_ptr<ui::Texture> checkedTex = data::Manager::getTexture(data::TextureSource::GUMPART, checkedId);

        if (!uncheckedTex) {
            LOG_WARN << "Unable to display checkbox with unchecked id " << uncheckedId << std::endl;
            return true;
        }

        if (!checkedTex) {
            LOG_WARN << "Unable to display checkbox with checked id " << checkedId << std::endl;
            return true;
        }

        components::UoCheckbox* cb = new components::UoCheckbox(menu);
        CL_Rectf bounds(x, y, CL_Sizef(checkedTex->getWidth(), checkedTex->getHeight()));
        cb->set_geometry(bounds);

        cb->addTexture(components::UoCheckbox::TEX_INDEX_UNCHECKED, uncheckedTex);
        cb->addTexture(components::UoCheckbox::TEX_INDEX_UNCHECKED_MOUSEOVER, uncheckedTex);
        cb->addTexture(components::UoCheckbox::TEX_INDEX_CHECKED, checkedTex);
        cb->addTexture(components::UoCheckbox::TEX_INDEX_CHECKED_MOUSEOVER, checkedTex);
        cb->updateTexture();
        cb->setAutoResize(true);

        cb->setSwitchId(switchId);

        if (checked) {
            cb->setChecked(true);
        }

        menu->addToCurrentPage(cb);

        return true;
    } else {
        LOG_ERROR << "Unable to parse checkbox, params " << params << std::endl;
        return false;
    }
}

bool StringParser::setNoMove(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    menu->setDraggable(false);
    return true;
}

bool StringParser::setNoClose(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    menu->setClosable(false);
    return true;
}

bool StringParser::parseTextEntry(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*", 0, status);
    matcher.reset(params);

    if (matcher.find() && matcher.groupCount() == 7) {
        int x = StringConverter::toInt(matcher.group(1, status));
        int y = StringConverter::toInt(matcher.group(2, status));
        int width = StringConverter::toInt(matcher.group(3, status));
        int height = StringConverter::toInt(matcher.group(4, status));
        int hue = StringConverter::toInt(matcher.group(5, status));
        int entryId = StringConverter::toInt(matcher.group(6, status));
        int textIdx = StringConverter::toInt(matcher.group(7, status));

        components::TextEntry* entry = new components::TextEntry(menu);
        CL_Rectf bounds(x, y, CL_Sizef(width, height));
        entry->set_geometry(bounds);

        entry->setEntryId(entryId);
        entry->setText(strings[textIdx]);

        entry->setTextHue(hue);

        CL_FontDescription defaultFontDesc;
        defaultFontDesc.set_typeface_name("unifont1");
        entry->setFont(defaultFontDesc);

        menu->addToCurrentPage(entry);

        return true;
    } else {
        LOG_ERROR << "Unable to parse textentry, params " << params << std::endl;
        return false;
    }
}

bool StringParser::parseHtmlGump(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*", 0, status);
    matcher.reset(params);
    if (matcher.find() && matcher.groupCount() == 7) {
        int x = StringConverter::toInt(matcher.group(1, status));
        int y = StringConverter::toInt(matcher.group(2, status));
        int width = StringConverter::toInt(matcher.group(3, status));
        int height = StringConverter::toInt(matcher.group(4, status));
        int stringId = StringConverter::toInt(matcher.group(5, status));
        int background = StringConverter::toInt(matcher.group(6, status));
        int scroll = StringConverter::toInt(matcher.group(7, status));

        CL_Rectf bounds(x, y, CL_Sizef(width, height));
        if (background == 1) {
            components::Background* bg = new components::Background(menu);
            bg->set_geometry(bounds);
            bg->setBaseId(3000);
            menu->addToCurrentPage(bg);
        }

        if (scroll == 1) {
            LOG_INFO << "Scrollbars for html gumps are not implemented yet" << std::endl;
        }

        components::Label* label = new components::Label(menu);
        label->set_geometry(bounds);
        label->setHtmlText(strings[stringId]);
        menu->addToCurrentPage(label);

        return true;
    } else {
        LOG_ERROR << "Unable to parse htmlgump, params " << params << std::endl;
        return false;
    }
}

bool StringParser::parseXmfHtmlGump(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*", 0, status);
    matcher.reset(params);
    if (matcher.find() && matcher.groupCount() == 7) {
        int x = StringConverter::toInt(matcher.group(1, status));
        int y = StringConverter::toInt(matcher.group(2, status));
        int width = StringConverter::toInt(matcher.group(3, status));
        int height = StringConverter::toInt(matcher.group(4, status));
        int clilocId = StringConverter::toInt(matcher.group(5, status));
        int background = StringConverter::toInt(matcher.group(6, status));
        int scroll = StringConverter::toInt(matcher.group(7, status));

        CL_Rectf bounds(x, y, CL_Sizef(width, height));
        if (background == 1) {
            components::Background* bg = new components::Background(menu);
            bg->set_geometry(bounds);
            bg->setBaseId(3000);
            menu->addToCurrentPage(bg);
        }

        if (scroll == 1) {
            LOG_INFO << "Scrollbars for html gumps are not implemented yet" << std::endl;
        }

        UnicodeString text = data::Manager::getClilocLoader()->get(clilocId);

        components::Label* label = new components::Label(menu);
        label->set_geometry(bounds);
        label->setHtmlText(text);
        menu->addToCurrentPage(label);

        return true;
    } else {
        LOG_ERROR << "Unable to parse xmfhtmlgump, params " << params << std::endl;
        return false;
    }
}

bool StringParser::parseXmfHtmlGumpColor(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*", 0, status);
    matcher.reset(params);
    if (matcher.find() && matcher.groupCount() == 8) {
        int x = StringConverter::toInt(matcher.group(1, status));
        int y = StringConverter::toInt(matcher.group(2, status));
        int width = StringConverter::toInt(matcher.group(3, status));
        int height = StringConverter::toInt(matcher.group(4, status));
        int clilocId = StringConverter::toInt(matcher.group(5, status));
        int background = StringConverter::toInt(matcher.group(6, status));
        int scroll = StringConverter::toInt(matcher.group(7, status));
        int color = StringConverter::toInt(matcher.group(8, status));

        CL_Rectf bounds(x, y, CL_Sizef(width, height));
        if (background == 1) {
            components::Background* bg = new components::Background(menu);
            bg->set_geometry(bounds);
            bg->setBaseId(3000);
            menu->addToCurrentPage(bg);
        }

        if (scroll == 1) {
            LOG_INFO << "Scrollbars for html gumps are not implemented yet" << std::endl;
        }

        // hue is sent as 16 bit rgb. 5 bit each. same as format in hues.mul.
        uint16_t color16 = color & 0xFFFF;
        int r = data::Util::getColorR(color16);
        int g = data::Util::getColorG(color16);
        int b = data::Util::getColorB(color16);
        CL_Colorf colorDef(r, g, b);

        UnicodeString text = data::Manager::getClilocLoader()->get(clilocId);

        components::Label* label = new components::Label(menu);
        label->set_geometry(bounds);
        label->setHtmlText(text, colorDef);
        menu->addToCurrentPage(label);

        return true;
    } else {
        LOG_ERROR << "Unable to parse xmfhtmlgumpcolor, params " << params << std::endl;
        return false;
    }
}

bool StringParser::parseXmfHtmlTok(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*@([^@]*)@\\s*", 0, status);
    matcher.reset(params);
    if (matcher.find() && matcher.groupCount() == 9) {
        int x = StringConverter::toInt(matcher.group(1, status));
        int y = StringConverter::toInt(matcher.group(2, status));
        int width = StringConverter::toInt(matcher.group(3, status));
        int height = StringConverter::toInt(matcher.group(4, status));
        int background = StringConverter::toInt(matcher.group(5, status));
        int scroll = StringConverter::toInt(matcher.group(6, status));
        int color = StringConverter::toInt(matcher.group(7, status));
        int clilocId = StringConverter::toInt(matcher.group(8, status));
        UnicodeString args = matcher.group(9, status);

        CL_Rectf bounds(x, y, CL_Sizef(width, height));
        if (background == 1) {
            components::Background* bg = new components::Background(menu);
            bg->set_geometry(bounds);
            bg->setBaseId(3000);
            menu->addToCurrentPage(bg);
        }

        if (scroll == 1) {
            LOG_INFO << "Scrollbars for html gumps are not implemented yet" << std::endl;
        }

        // hue is sent as 16 bit rgb. 5 bit each. same as format in hues.mul.
        uint16_t color16 = color & 0xFFFF;
        int r = data::Util::getColorR(color16);
        int g = data::Util::getColorG(color16);
        int b = data::Util::getColorB(color16);
        CL_Colorf colorDef(r, g, b);

        UnicodeString text = data::Manager::getClilocLoader()->get(clilocId, args);

        components::Label* label = new components::Label(menu);
        label->set_geometry(bounds);
        label->setHtmlText(text, colorDef);
        menu->addToCurrentPage(label);

        return true;
    } else {
        LOG_ERROR << "Unable to parse xmfhtmlgumpcolor, params " << params << std::endl;
        return false;
    }
}

bool StringParser::parseCheckerTrans(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*", 0, status);
    matcher.reset(params);
    if (matcher.find() && matcher.groupCount() == 4) {
        int x = StringConverter::toInt(matcher.group(1, status));
        int y = StringConverter::toInt(matcher.group(2, status));
        int width = StringConverter::toInt(matcher.group(3, status));
        int height = StringConverter::toInt(matcher.group(4, status));

        CL_Rectf checkerRect(x, y, CL_Sizef(width, height));

        std::vector<CL_GUIComponent*> children = menu->get_child_components();
        std::vector<CL_GUIComponent*>::const_iterator iter = children.begin();
        std::vector<CL_GUIComponent*>::const_iterator end = children.end();
        for (; iter != end; ++iter) {
            if ((*iter)->get_geometry().is_overlapped(checkerRect)) {
                components::Image* img = dynamic_cast<components::Image*>(*iter);
                if (img) {
                    img->setAlpha(0.2f);
                } else {
                    components::Background* bg = dynamic_cast<components::Background*>(*iter);
                    if (bg) {
                        bg->setAlpha(0.2f);
                    }
                }
            }
        }

        return true;
    } else {
        LOG_ERROR << "Unable to parse checkertrans, params " << params << std::endl;
        return false;
    }
}

bool StringParser::parseRadio(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*", 0, status);
    matcher.reset(params);

    if (matcher.find() && matcher.groupCount() == 6) {
        int x = StringConverter::toInt(matcher.group(1, status));
        int y = StringConverter::toInt(matcher.group(2, status));
        int uncheckedId = StringConverter::toInt(matcher.group(3, status));
        int checkedId = StringConverter::toInt(matcher.group(4, status));
        int checked = StringConverter::toInt(matcher.group(5, status));
        int switchId = StringConverter::toInt(matcher.group(6, status));

        boost::shared_ptr<ui::Texture> uncheckedTex = data::Manager::getTexture(data::TextureSource::GUMPART, uncheckedId);
        boost::shared_ptr<ui::Texture> checkedTex = data::Manager::getTexture(data::TextureSource::GUMPART, checkedId);

        if (!uncheckedTex) {
            LOG_WARN << "Unable to display radio with unchecked id " << uncheckedId << std::endl;
            return true;
        }

        if (!checkedTex) {
            LOG_WARN << "Unable to display radio with checked id " << checkedId << std::endl;
            return true;
        }

        components::UoRadioButton* cb = new components::UoRadioButton(menu);
        CL_Rectf bounds(x, y, CL_Sizef(checkedTex->getWidth(), checkedTex->getHeight()));
        cb->set_geometry(bounds);

        cb->addTexture(components::UoCheckbox::TEX_INDEX_UNCHECKED, uncheckedTex);
        cb->addTexture(components::UoCheckbox::TEX_INDEX_UNCHECKED_MOUSEOVER, uncheckedTex);
        cb->addTexture(components::UoCheckbox::TEX_INDEX_CHECKED, checkedTex);
        cb->addTexture(components::UoCheckbox::TEX_INDEX_CHECKED_MOUSEOVER, checkedTex);
        cb->updateTexture();
        cb->setAutoResize(true);

        cb->setSwitchId(switchId);
        cb->setRadioGroupId(menu->getCurrentRadioGroup());

        if (checked) {
            cb->setChecked(true);
        }

        menu->addToCurrentPage(cb);

        return true;
    } else {
        LOG_ERROR << "Unable to parse radio, params " << params << std::endl;
        return false;
    }
}

bool StringParser::parseGroup(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const {
    UErrorCode status = U_ZERO_ERROR;
    static RegexMatcher matcher("\\s*(\\w+)\\s*", 0, status);
    matcher.reset(params);

    if (matcher.find() && matcher.groupCount() == 1) {
        int group = StringConverter::toInt(matcher.group(1, status));

        menu->setCurrentRadioGroup(group);

        return true;
    } else {
        LOG_ERROR << "Unable to parse group, params " << params << std::endl;
        return false;
    }
}

}
}
