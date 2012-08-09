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


#ifndef FLUO_UI_XMLLOADER_HPP
#define FLUO_UI_XMLLOADER_HPP

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/function.hpp>

#include <map>
#include <vector>

#include <misc/pugixml/pugixml.hpp>

#include <data/manager.hpp>
#include "gumpmenu.hpp"
#include "components/multitextureimage.hpp"

namespace fluo {
namespace ui {

namespace components {
class Button;
class Label;
class ScrollBar;
class ScrollArea;
}

class XmlLoader {
public:
    struct RepeatContext {
        unsigned int repeatCount_;
        std::map<UnicodeString, std::vector<UnicodeString> > keywordReplacements_;
    };

    static void addRepeatContext(const UnicodeString& name, const RepeatContext& context);
    static void removeRepeatContext(const UnicodeString& name);
    static void clearRepeatContexts();

    /**
     * \brief Create a gump menu according to the description in the xml file
     * Gumps are delete by the gui system automatically, so there is no need to delete or free this pointer
     * \param menu if the second parameter is NULL, a new gump is created, otherwise, the components are inserted in menu. \
            Top level attributes are not parsed then
     */
    static GumpMenu* fromXmlFile(const UnicodeString& name);

    /**
     * \brief Create a gump menu according to the description in the string
     * Gumps are delete by the gui system automatically, so there is no need to delete or free this pointer
     * \param menu if the second parameter is NULL, a new gump is created, otherwise, the components are inserted in menu. \
            Top level attributes are not parsed then
     */
    static GumpMenu* fromXmlString(const UnicodeString& str);


    static bool readTemplateFile(const boost::filesystem::path& themePath);

private:
    static XmlLoader* singleton_;
    static XmlLoader* getSingleton();

    XmlLoader();
    XmlLoader(const XmlLoader& copy) {};
    XmlLoader& operator=(const XmlLoader& copy) { return *this; };

    GumpMenu* fromXml(pugi::xml_document& doc);


    // template stuff
    pugi::xml_document templateDocument_;
    void setTemplates(); // templateDocument_ is set before this function is called
    std::map<UnicodeString, pugi::xml_node> templateMap_;
    std::map<UnicodeString, pugi::xml_node> defaultTemplateMap_;
    pugi::xml_node getTemplate(const UnicodeString& templateName);
    pugi::xml_attribute getAttribute(const char* name, pugi::xml_node& node, pugi::xml_node& defaultNode);
    pugi::xml_node getNode(const char* name, pugi::xml_node& node, pugi::xml_node& defaultNode);

    // helper functions
    GumpComponent* parseChildren(pugi::xml_node& rootNode, CL_GUIComponent* parent, GumpMenu* top);
    CL_Rect getBoundsFromNode(pugi::xml_node& node, pugi::xml_node& defaultNode);
    bool parseId(pugi::xml_node& node, CL_GUIComponent* component);
    boost::shared_ptr<ui::Texture> parseTexture(pugi::xml_node& node, pugi::xml_node& defaultNode);
    bool parseMultiTextureImage(pugi::xml_node& node, pugi::xml_node& defaultNode, components::MultiTextureImage* button, unsigned int index);
    bool parseButtonText(pugi::xml_node& node, pugi::xml_node& defaultNode, components::Button* button, unsigned int index);
    bool parseLabelHelper(components::Label* label, pugi::xml_node& node, pugi::xml_node& defaultNode);
    bool parseScrollBar(components::ScrollBar* bar, components::ScrollArea* area, bool vertical, pugi::xml_node node, pugi::xml_node defaultNode);
    bool parseScrollBarTextures(boost::shared_ptr<ui::Texture>* textures, CL_Colorf* colors, CL_Vec3f* hueInfos, pugi::xml_node& node, pugi::xml_node& defaultNode);


    // components
    GumpComponent* parseImage(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parseButton(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parseBackground(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parsePage(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parseCheckbox(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parseRadioButton(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parseLabel(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parseSysLogLabel(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parsePropertyLabel(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parseClickLabel(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parseLineEdit(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parseScrollArea(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parseWarModeButton(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parseWorldView(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parsePaperdoll(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parseContainer(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    GumpComponent* parseHtmlLabel(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);


    // repeat-related stuff
    bool parseRepeat(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top);
    void insertRepeatNodes(pugi::xml_node::iterator begin, pugi::xml_node::iterator end, pugi::xml_node dst,
            const RepeatContext& context, unsigned int index,
            int xIncrease, int yIncrease, unsigned int xLimit, unsigned int yLimit);
    void replaceRepeatKeywords(pugi::xml_node& node, const RepeatContext& context, unsigned int index,
            int xIncrease, int yIncrease, unsigned int xLimit, unsigned int yLimit);
    void checkRepeatIf(pugi::xml_node& node, unsigned int index, unsigned int xLimit, unsigned int yLimit);


    typedef boost::function<GumpComponent* (pugi::xml_node&, pugi::xml_node&, CL_GUIComponent*, GumpMenu*)> XmlParseFunction;
    std::map<UnicodeString, XmlParseFunction> functionTable_;
    std::map<UnicodeString, RepeatContext> repeatContexts_;

};

}
}


#endif
