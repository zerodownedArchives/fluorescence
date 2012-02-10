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


#ifndef FLUO_UI_STRINGPARSER_HPP
#define FLUO_UI_STRINGPARSER_HPP

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <misc/string.hpp>

namespace fluo {
namespace ui {
    
class GumpMenu;

class StringParser {
public:
    static GumpMenu* fromString(const UnicodeString& commands, const std::vector<UnicodeString>& strings);
    
private:
    static StringParser* singleton_;
    static StringParser* getSingleton();
    
    StringParser();
    StringParser(const StringParser& copy) { };
    StringParser& operator=(const StringParser& copy) { };
    
    GumpMenu* innerFromString(const UnicodeString& commands, const std::vector<UnicodeString>& strings);
    
    typedef boost::function<bool (const UnicodeString&, const std::vector<UnicodeString>& strings, GumpMenu*)> StringParseFunction;
    std::map<UnicodeString, StringParseFunction> functionTable_;
    
    bool setNoMove(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    bool setNoClose(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    
    bool parsePage(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    bool parseResizepic(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    bool parseGumpPicTiled(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    bool parseCroppedText(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    bool parseText(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    bool parseGumpPic(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    bool parseTilePic(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    bool parseTilePicHue(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    bool parseButton(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    bool parseCheckbox(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    bool parseTextEntry(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    bool parseHtmlGump(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    bool parseXmfHtmlGump(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    bool parseXmfHtmlGumpColor(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
    bool parseXmfHtmlTok(const UnicodeString& params, const std::vector<UnicodeString>& strings, GumpMenu* menu) const;
};

}
}

#endif

