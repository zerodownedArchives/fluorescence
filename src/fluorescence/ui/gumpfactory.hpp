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


#ifndef FLUO_UI_GUMPFACTORY_HPP
#define FLUO_UI_GUMPFACTORY_CPP

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/function.hpp>

#include <map>
#include <vector>

#include <misc/pugixml/pugixml.hpp>

#include "gumpmenu.hpp"

namespace fluo {
namespace ui {

class GumpFactory {
public:
    struct RepeatKeyword {
        RepeatKeyword(const UnicodeString& node, const UnicodeString& attribute, const UnicodeString& search);

        bool operator<(const RepeatKeyword& rhs) const;

        UnicodeString nodeText_;
        UnicodeString attributeText_;
        UnicodeString searchText_;
    };

    struct RepeatContext {
        unsigned int repeatCount_;
        std::map<RepeatKeyword, std::vector<UnicodeString> > keywordReplacments_;
    };

    static void addRepeatContext(const UnicodeString& name, const RepeatContext& context);
    static void removeRepeatContext(const UnicodeString& name);

    /**
     * \brief Create a gump menu according to the description in the xml file
     * Gumps are delete by the gui system automatically, so there is no need to delete or free this pointer
     * \param menu if the second parameter is NULL, a new gump is created, otherwise, the components are inserted in menu. \
            Top level attributes are not parsed then
     */
    static GumpMenu* fromXmlFile(const UnicodeString& name, GumpMenu* menu = NULL);

    /**
     * \brief Create a gump menu according to the description in the string
     * Gumps are delete by the gui system automatically, so there is no need to delete or free this pointer
     * \param menu if the second parameter is NULL, a new gump is created, otherwise, the components are inserted in menu. \
            Top level attributes are not parsed then
     */
    static GumpMenu* fromXmlString(const UnicodeString& str, GumpMenu* menu = NULL);

private:
    static GumpFactory* singleton_;
    static GumpFactory* getSingleton();

    GumpFactory();
    GumpFactory(const GumpFactory& factory) { }
    GumpFactory& operator=(const GumpFactory& factory) { return *this; }

    GumpMenu* fromXml(pugi::xml_document& doc, GumpMenu* menu);

    CL_Rect getBoundsFromNode(pugi::xml_node& node, const CL_GUIComponent* parent);
    CL_Rect getBoundsFromNode(pugi::xml_node& node, const CL_Rect& parentGeometry);
    bool parseId(pugi::xml_node& node, CL_GUIComponent* component);

    // themed ui components
    bool parseChildren(pugi::xml_node& rootNode, CL_GUIComponent* parent, GumpMenu* top);
    bool parseTButton(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);
    bool parseTCheckBox(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);
    bool parseTRadioButton(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);
    bool parseTLineEdit(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);
    bool parseTComboBox(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);
    bool parseTGroupBox(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);
    bool parseTSpin(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);
    bool parseTTabs(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);
    bool parseTSlider(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);
    bool parseTLabel(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);
    bool parseTTextEdit(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);
    bool parseTScrollArea(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);

    bool parseRepeat(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);
    bool parsePropertyLabel(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);

    // supports both themed and uo images
    bool parseImage(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);

    // uo components
    bool parsePage(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);

    bool parseWorldView(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);
    bool parsePaperdoll(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);
    bool parseContainer(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);
    // TODO


    std::map<UnicodeString, boost::function<bool (pugi::xml_node&, CL_GUIComponent*, GumpMenu*)> > functionTable_;
    std::map<UnicodeString, RepeatContext> repeatContexts_;

    bool gameViewFindHelper(pugi::xml_node& node) const;
};

}
}


#endif
