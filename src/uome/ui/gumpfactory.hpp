#ifndef UOME_UI_GUMPFACTORY_HPP
#define UOME_UI_GUMPFACTORY_CPP

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/function.hpp>

#include <map>

#include <misc/pugixml.hpp>

#include "gumpmenu.hpp"

namespace uome {
namespace ui {

class GumpFactory {
public:
    /**
     * \brief Create a gump menu according to the description in the xml file
     * Gumps are delete by the gui system automatically, so there is no need to delete or free this pointer
     * \param menu if the second parameter is NULL, a new gump is created, otherwise, the components are inserted in menu. \
            Top level attributes are not parsed then
     */
    static GumpMenu* fromXmlFile(const std::string& name, GumpMenu* menu = NULL);

    /**
     * \brief Create a gump menu according to the description in the string
     * Gumps are delete by the gui system automatically, so there is no need to delete or free this pointer
     * \param menu if the second parameter is NULL, a new gump is created, otherwise, the components are inserted in menu. \
            Top level attributes are not parsed then
     */
    static GumpMenu* fromXmlString(const std::string& str, GumpMenu* menu = NULL);

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

    // supports both themed and uo images
    bool parseImage(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);

    // uo components
    bool parsePage(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top);

    // TODO

    std::map<std::string, boost::function<bool (pugi::xml_node&, CL_GUIComponent*, GumpMenu*)> > functionTable_;
};

}
}


#endif
