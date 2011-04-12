
#include "gumpfactory.hpp"

#include <boost/bind.hpp>

#include <misc/logger.hpp>
#include <client.hpp>


namespace uome {
namespace ui {


GumpFactory* GumpFactory::singleton_ = NULL;

GumpFactory* GumpFactory::getSingleton() {
    if (!singleton_) {
        singleton_ = new GumpFactory();
    }

    return singleton_;
}

GumpFactory::GumpFactory() {
    functionTable_["tbutton"] = boost::bind(&GumpFactory::parseTButton, this, _1, _2);
    functionTable_["tcheckbox"] = boost::bind(&GumpFactory::parseTCheckBox, this, _1, _2);
    functionTable_["tradiobutton"] = boost::bind(&GumpFactory::parseTRadioButton, this, _1, _2);
    functionTable_["tlineedit"] = boost::bind(&GumpFactory::parseTLineEdit, this, _1, _2);
    functionTable_["tcombobox"] = boost::bind(&GumpFactory::parseTComboBox, this, _1, _2);
    functionTable_["tgroupbox"] = boost::bind(&GumpFactory::parseTGroupBox, this, _1, _2);
    functionTable_["tspin"] = boost::bind(&GumpFactory::parseTSpin, this, _1, _2);
    functionTable_["ttabs"] = boost::bind(&GumpFactory::parseTTabs, this, _1, _2);
    functionTable_["tslider"] = boost::bind(&GumpFactory::parseTSlider, this, _1, _2);
}

GumpMenu* GumpFactory::fromXmlFile(const std::string& name) {
    if (name.find("..") != std::string::npos) {
        LOGARG_WARN(LOGTYPE_UI, "Directory traversal attempt, trying to read gump file %s", name.c_str());
        return NULL;
    }

    std::string gumpsDirectory = Client::getSingleton()->getConfig()["ui.gumps-directory"].as<std::string>();
    boost::filesystem::path fileName = name + ".xml";

    boost::filesystem::path path = gumpsDirectory / fileName;

    LOGARG_DEBUG(LOGTYPE_UI, "Parsing gumps file: %s", path.string().c_str());

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.string().c_str());

    if (result) {
        return getSingleton()->fromXml(doc);
    } else {
        return NULL;
    }
}

GumpMenu* GumpFactory::fromXmlString(const std::string& str) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(str.c_str(), str.length());

    if (result) {
        return getSingleton()->fromXml(doc);
    } else {
        return NULL;
    }
}


GumpMenu* GumpFactory::fromXml(pugi::xml_document& doc) {

    pugi::xml_node rootNode = doc.child("gump");

    CL_Rect bounds = getBoundsFromNode(rootNode);
    CL_GUITopLevelDescription desc(bounds, false);
    desc.set_decorations(false);

    GumpMenu* ret = new GumpMenu(desc);

    pugi::xml_node_iterator iter = rootNode.begin();
    pugi::xml_node_iterator iterEnd = rootNode.end();

    for (; iter != iterEnd; ++iter) {
        std::map<std::string, boost::function<bool (pugi::xml_node&, GumpMenu*)> >::iterator function = functionTable_.find(iter->name());

        if (function != functionTable_.end()) {
            (function->second)(*iter, ret);
        } else {
            LOGARG_WARN(LOGTYPE_UI, "Unknown gump tag \"%s\"", iter->name());
        }
    }

    return ret;
}

CL_Rect GumpFactory::getBoundsFromNode(pugi::xml_node& node) {
    int locX = node.attribute("x").as_int();
    int locY = node.attribute("y").as_int();
    unsigned int width = node.attribute("width").as_uint();
    unsigned int height = node.attribute("height").as_uint();

    CL_Rect bounds(locX, locY, CL_Size(width, height));

    return bounds;
}


bool GumpFactory::parseTButton(pugi::xml_node& node, GumpMenu* parent) {
    CL_Rect bounds = getBoundsFromNode(node);
    std::string text = node.attribute("text").value();

    CL_PushButton* button = new CL_PushButton(parent);
    button->set_geometry(bounds);
    button->set_text(text);

    return true;
}

bool GumpFactory::parseTCheckBox(pugi::xml_node& node, GumpMenu* parent) {
    CL_Rect bounds = getBoundsFromNode(node);
    std::string text = node.attribute("text").value();
    int checked = node.attribute("checked").as_int();

    CL_CheckBox* cb = new CL_CheckBox(parent);
    cb->set_geometry(bounds);
    cb->set_text(text);

    if (checked) {
        cb->set_checked(true);
    }

    return true;
}

bool GumpFactory::parseTRadioButton(pugi::xml_node& node, GumpMenu* parent) {
    CL_Rect bounds = getBoundsFromNode(node);
    std::string text = node.attribute("text").value();
    std::string group = node.attribute("group").value();
    int selected = node.attribute("selected").as_int();

    CL_RadioButton* button = new CL_RadioButton(parent);
    button->set_geometry(bounds);
    button->set_text(text);
    button->set_group_name(group);

    if (selected) {
        button->set_selected(true);
    }

    return true;
}

bool GumpFactory::parseTLineEdit(pugi::xml_node& node, GumpMenu* parent) {
    return true;
}

bool GumpFactory::parseTComboBox(pugi::xml_node& node, GumpMenu* parent) {
    return true;
}

bool GumpFactory::parseTGroupBox(pugi::xml_node& node, GumpMenu* parent) {
    return true;
}

bool GumpFactory::parseTSpin(pugi::xml_node& node, GumpMenu* parent) {
    return true;
}

bool GumpFactory::parseTTabs(pugi::xml_node& node, GumpMenu* parent) {
    return true;
}

bool GumpFactory::parseTSlider(pugi::xml_node& node, GumpMenu* parent) {
    return true;
}


}
}
