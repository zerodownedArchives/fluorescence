
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
    functionTable_["tlabel"] = boost::bind(&GumpFactory::parseTLabel, this, _1, _2);
    functionTable_["ttextedit"] = boost::bind(&GumpFactory::parseTTextEdit, this, _1, _2);

    functionTable_["image"] = boost::bind(&GumpFactory::parseImage, this, _1, _2);
    functionTable_["label"] = boost::bind(&GumpFactory::parseLabel, this, _1, _2);
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
    std::string title = rootNode.attribute("title").value();

    CL_GUITopLevelDescription desc(bounds, false);

    if (title.length() > 0) {
        desc.set_decorations(true);
        desc.set_title(title);
    } else {
        desc.set_decorations(false);
    }

    GumpMenu* ret = new GumpMenu(desc);

    parseChildren(rootNode, ret);

    return ret;
}

bool GumpFactory::parseChildren(pugi::xml_node& rootNode, CL_GUIComponent* parent) {
    pugi::xml_node_iterator iter = rootNode.begin();
    pugi::xml_node_iterator iterEnd = rootNode.end();

    bool ret = true;

    for (; iter != iterEnd && ret; ++iter) {
        std::map<std::string, boost::function<bool (pugi::xml_node&, CL_GUIComponent*)> >::iterator function = functionTable_.find(iter->name());

        if (function != functionTable_.end()) {
            ret = (function->second)(*iter, parent);
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


bool GumpFactory::parseTButton(pugi::xml_node& node, CL_GUIComponent* parent) {
    CL_Rect bounds = getBoundsFromNode(node);
    std::string text = node.attribute("text").value();

    CL_PushButton* button = new CL_PushButton(parent);
    button->set_geometry(bounds);
    button->set_text(text);

    return true;
}

bool GumpFactory::parseTCheckBox(pugi::xml_node& node, CL_GUIComponent* parent) {
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

bool GumpFactory::parseTRadioButton(pugi::xml_node& node, CL_GUIComponent* parent) {
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

bool GumpFactory::parseTLineEdit(pugi::xml_node& node, CL_GUIComponent* parent) {
    CL_Rect bounds = getBoundsFromNode(node);
    std::string text = node.attribute("text").value();
    int numeric = node.attribute("numeric").as_int();
    int password = node.attribute("password").as_int();
    unsigned int maxlength = node.attribute("maxlength").as_uint();

    CL_LineEdit* edit = new CL_LineEdit(parent);
    edit->set_text(text);
    edit->set_geometry(bounds);
    edit->set_select_all_on_focus_gain(false);

    if (password) {
        edit->set_password_mode(true);
    }

    if (numeric) {
        edit->set_numeric_mode(true);
    }

    if (maxlength) {
        edit->set_max_length(maxlength);
    }

    return true;
}

bool GumpFactory::parseTComboBox(pugi::xml_node& node, CL_GUIComponent* parent) {
    CL_Rect bounds = getBoundsFromNode(node);

    CL_ComboBox* box = new CL_ComboBox(parent);
    box->set_geometry(bounds);

    CL_PopupMenu menu;

    pugi::xml_node_iterator iter = node.begin();
    pugi::xml_node_iterator iterEnd = node.end();

    bool ret = true;
    unsigned int selected = 0;

    for (unsigned int index = 0; iter != iterEnd && ret; ++iter, ++index) {
        if (strcmp(iter->name(), "option") != 0) {
            LOGARG_WARN(LOGTYPE_UI, "Something different than option in combobox: \"%s\"", iter->name());
            ret = false;
        } else {
            std::string text = iter->attribute("text").value();
            int isSelected = iter->attribute("selected").as_int();

            menu.insert_item(text);
            if (isSelected) {
                selected = index;
            }
        }
    }

    box->set_popup_menu(menu);
    box->set_selected_item(selected);

    return ret;
}

bool GumpFactory::parseTGroupBox(pugi::xml_node& node, CL_GUIComponent* parent) {
    CL_Rect bounds = getBoundsFromNode(node);

    CL_GroupBox* box = new CL_GroupBox(parent);
    box->set_geometry(bounds);

    parseChildren(node, box);

    return true;
}

bool GumpFactory::parseTSpin(pugi::xml_node& node, CL_GUIComponent* parent) {
    CL_Rect bounds = getBoundsFromNode(node);
    std::string type = node.attribute("type").value();

    CL_Spin* spin = new CL_Spin(parent);

    if (type == "int") {
        int min = node.attribute("min").as_int();
        int max = node.attribute("max").as_int();
        unsigned int stepsize = node.attribute("stepsize").as_uint();
        int value = node.attribute("value").as_int();

        spin->set_floating_point_mode(false);
        spin->set_ranges(min, max);
        spin->set_step_size(stepsize);
        spin->set_value(value);
    } else if (type == "float") {
        float min = node.attribute("min").as_float();
        float max = node.attribute("max").as_float();
        float stepsize = node.attribute("stepsize").as_float();
        float value = node.attribute("value").as_float();

        spin->set_floating_point_mode(true);
        spin->set_ranges_float(min, max);
        spin->set_step_size_float(stepsize);
        spin->set_value_float(value);
    } else {
        LOGARG_WARN(LOGTYPE_UI, "Unknown spin type: \"%s\"", type.c_str());
        return false;
    }

    spin->set_geometry(bounds);

    return true;
}

bool GumpFactory::parseTTabs(pugi::xml_node& node, CL_GUIComponent* parent) {
    CL_Rect bounds = getBoundsFromNode(node);

    CL_Tab* tabs = new CL_Tab(parent);
    tabs->set_geometry(bounds);

    pugi::xml_node_iterator iter = node.begin();
    pugi::xml_node_iterator iterEnd = node.end();

    bool ret = true;

    for (; iter != iterEnd && ret; ++iter) {
        if (strcmp(iter->name(), "ttabpage") != 0) {
            LOGARG_WARN(LOGTYPE_UI, "Something different than ttabpage in ttabs: \"%s\"", iter->name());
            ret = false;
        } else {
            std::string tabTitle = iter->attribute("text").value();
            CL_TabPage* newpage = tabs->add_page(tabTitle);

            parseChildren(*iter, newpage);
        }
    }

    return ret;
}

bool GumpFactory::parseTSlider(pugi::xml_node& node, CL_GUIComponent* parent) {
    CL_Rect bounds = getBoundsFromNode(node);
    std::string type = node.attribute("type").value();
    int min = node.attribute("min").as_int();
    int max = node.attribute("max").as_int();
    unsigned int pagestep = node.attribute("pagestep").as_uint();
    int value = node.attribute("value").as_int();


    CL_Slider* slider = new CL_Slider(parent);

    if (type == "vertical") {
        slider->set_vertical(true);
    } else if (type == "horizontal") {
        slider->set_horizontal(true);
    } else {
        LOGARG_WARN(LOGTYPE_UI, "Unknown slider type: \"%s\"", type.c_str());
        return false;
    }

    slider->set_min(min);
    slider->set_max(max);
    slider->set_page_step(pagestep);
    slider->set_position(value);
    slider->set_lock_to_ticks(false);
    slider->set_geometry(bounds);

    return true;
}

bool GumpFactory::parseTLabel(pugi::xml_node& node, CL_GUIComponent* parent) {
    return true;
}

bool GumpFactory::parseTTextEdit(pugi::xml_node& node, CL_GUIComponent* parent) {
    return true;
}


bool GumpFactory::parseLabel(pugi::xml_node& node, CL_GUIComponent* parent) {
    return true;
}

bool GumpFactory::parseImage(pugi::xml_node& node, CL_GUIComponent* parent) {
    return true;
}

}
}
