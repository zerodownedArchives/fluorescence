
#include "gumpfactory.hpp"

#include <ClanLib/Display/Image/pixel_buffer.h>

#include <boost/bind.hpp>

#include <misc/logger.hpp>
#include <client.hpp>
#include <data/artloader.hpp>
#include <data/gumpartloader.hpp>
#include <data/manager.hpp>

#include "manager.hpp"
#include "texture.hpp"
#include "components/basebutton.hpp"
#include "components/pagebutton.hpp"
#include "components/serverbutton.hpp"
#include "components/localbutton.hpp"
#include "components/scrollarea.hpp"
#include "components/lineedit.hpp"

namespace uome {
namespace ui {

GumpFactory::RepeatKeyword::RepeatKeyword(const UnicodeString& node, const UnicodeString& attribute, const UnicodeString& search) :
            nodeText_(node), attributeText_(attribute), searchText_(search) {
}

bool GumpFactory::RepeatKeyword::operator<(const RepeatKeyword& rhs) const {
    if (nodeText_ == rhs.nodeText_) {
        if (attributeText_ == rhs.attributeText_) {
            if (searchText_ == rhs.searchText_) {
                return true;
            } else {
                return searchText_ < rhs.searchText_;
            }
        } else {
            return attributeText_ < rhs.attributeText_;
        }
    } else {
        return nodeText_ < rhs.nodeText_;
    }
}


GumpFactory* GumpFactory::singleton_ = NULL;

GumpFactory* GumpFactory::getSingleton() {
    if (!singleton_) {
        singleton_ = new GumpFactory();
    }

    return singleton_;
}

GumpFactory::GumpFactory() {
    functionTable_["tbutton"] = boost::bind(&GumpFactory::parseTButton, this, _1, _2, _3);
    functionTable_["tcheckbox"] = boost::bind(&GumpFactory::parseTCheckBox, this, _1, _2, _3);
    functionTable_["tradiobutton"] = boost::bind(&GumpFactory::parseTRadioButton, this, _1, _2, _3);
    functionTable_["tlineedit"] = boost::bind(&GumpFactory::parseTLineEdit, this, _1, _2, _3);
    functionTable_["tcombobox"] = boost::bind(&GumpFactory::parseTComboBox, this, _1, _2, _3);
    functionTable_["tgroupbox"] = boost::bind(&GumpFactory::parseTGroupBox, this, _1, _2, _3);
    functionTable_["tspin"] = boost::bind(&GumpFactory::parseTSpin, this, _1, _2, _3);
    functionTable_["ttabs"] = boost::bind(&GumpFactory::parseTTabs, this, _1, _2, _3);
    functionTable_["tslider"] = boost::bind(&GumpFactory::parseTSlider, this, _1, _2, _3);
    functionTable_["tlabel"] = boost::bind(&GumpFactory::parseTLabel, this, _1, _2, _3);
    functionTable_["ttextedit"] = boost::bind(&GumpFactory::parseTTextEdit, this, _1, _2, _3);
    functionTable_["tscrollarea"] = boost::bind(&GumpFactory::parseTScrollArea, this, _1, _2, _3);
    functionTable_["repeat"] = boost::bind(&GumpFactory::parseRepeat, this, _1, _2, _3);

    functionTable_["page"] = boost::bind(&GumpFactory::parsePage, this, _1, _2, _3);

    functionTable_["image"] = boost::bind(&GumpFactory::parseImage, this, _1, _2, _3);
}

void GumpFactory::addRepeatContext(const UnicodeString& name, const RepeatContext& context) {
    getSingleton()->repeatContexts_[name] = context;
}

void GumpFactory::removeRepeatContext(const UnicodeString& name) {
    getSingleton()->repeatContexts_.erase(name);
}

GumpMenu* GumpFactory::fromXmlFile(const UnicodeString& name, GumpMenu* menu) {
    // can be called before a shard is set
    boost::filesystem::path path;
    std::string utf8fileName = StringConverter::toUtf8String(name) + ".xml";
    if (Client::getSingleton()->getConfig().exists("/uome/shard@name")) {
        path = "shards" / Client::getSingleton()->getConfig()["/uome/shard@name"].asPath() / "gumps" / utf8fileName;
    }

    if (!boost::filesystem::exists(path)) {
        path = "gumps";
        path = path / utf8fileName;

        if (!boost::filesystem::exists(path)) {
            LOGARG_CRITICAL(LOGTYPE_UI, "Unable to gump xml %s: file not found", utf8fileName.c_str());
            return menu;
        }
    }

    LOGARG_DEBUG(LOGTYPE_UI, "Parsing xml gump file: %s", path.string().c_str());

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.string().c_str());

    if (result) {
        GumpMenu* ret = getSingleton()->fromXml(doc, menu);

        // save transformed document (debug)
        // std::cout << "Saving result: " << doc.save_file("out.xml") << std::endl;

        if (ret) {
            ret->setName(name);
        }

        return ret;
    } else {
        LOGARG_ERROR(LOGTYPE_UI, "Error parsing file at offset %i: %s", result.offset, result.description());
        return menu;
    }
}

GumpMenu* GumpFactory::fromXmlString(const UnicodeString& str, GumpMenu* menu) {
    pugi::xml_document doc;
    std::string utf8String = StringConverter::toUtf8String(str);
    pugi::xml_parse_result result = doc.load_buffer(utf8String.c_str(), utf8String.length());

    if (result) {
        GumpMenu* ret = getSingleton()->fromXml(doc, menu);

        return ret;
    } else {
        LOGARG_ERROR(LOGTYPE_UI, "Error parsing string at offset %i: %s", result.offset, result.description());
        return menu;
    }
}


GumpMenu* GumpFactory::fromXml(pugi::xml_document& doc, GumpMenu* menu) {

    pugi::xml_node rootNode = doc.child("gump");

    GumpMenu* ret = menu;

    if (!ret) {
        CL_Rect bounds = getBoundsFromNode(rootNode, ui::Manager::getSingleton()->getMainWindow()->get_viewport());
        bool closable = rootNode.attribute("closable").as_bool();
        bool draggable = rootNode.attribute("draggable").as_bool();

        CL_GUITopLevelDescription desc(bounds, false);
        desc.set_decorations(false);

        ret = new GumpMenu(desc);
        ret->setClosable(closable);
        ret->setDraggable(draggable);
    }

    parseChildren(rootNode, ret, ret);

    ret->activateFirstPage();

    return ret;
}

bool GumpFactory::parseChildren(pugi::xml_node& rootNode, CL_GUIComponent* parent, GumpMenu* top) {
    pugi::xml_node_iterator iter = rootNode.begin();
    pugi::xml_node_iterator iterEnd = rootNode.end();

    bool ret = true;

    for (; iter != iterEnd && ret; ++iter) {
        std::map<UnicodeString, boost::function<bool (pugi::xml_node&, CL_GUIComponent*, GumpMenu*)> >::iterator function = functionTable_.find(iter->name());

        if (function != functionTable_.end()) {
            ret = (function->second)(*iter, parent, top);
        } else {
            LOGARG_WARN(LOGTYPE_UI, "Unknown gump tag \"%s\"", iter->name());
        }
    }

    return ret;
}

CL_Rect GumpFactory::getBoundsFromNode(pugi::xml_node& node, const CL_GUIComponent* parent) {
    return getBoundsFromNode(node, parent->get_geometry());
}

CL_Rect GumpFactory::getBoundsFromNode(pugi::xml_node& node, const CL_Rect& parentGeometry) {
    unsigned int width = node.attribute("width").as_uint();
    unsigned int height = node.attribute("height").as_uint();

    int locX;
    int locY;
    static std::string centerStr("center");

    if (centerStr == node.attribute("x").value()) {
        locX = (parentGeometry.get_width() - width) / 2;
    } else {
        locX = node.attribute("x").as_int();
    }

    if (centerStr == node.attribute("y").value()) {
        locY = (parentGeometry.get_height() - height) / 2;
    } else {
        locY = node.attribute("y").as_int();
    }

    CL_Rect bounds(locX, locY, CL_Size(width, height));

    return bounds;
}

bool GumpFactory::parseId(pugi::xml_node& node, CL_GUIComponent* component) {
    std::string cssid = node.attribute("id").value();
    if (cssid.length() > 0) {
        component->set_id_name(cssid);
    }

    return true;
}

bool GumpFactory::parseTButton(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, parent);
    UnicodeString text = StringConverter::fromUtf8(node.attribute("text").value());
    unsigned int buttonId = node.attribute("buttonid").as_uint();
    unsigned int pageId = node.attribute("page").as_uint();
    UnicodeString action = StringConverter::fromUtf8(node.attribute("action").value());
    UnicodeString param = StringConverter::fromUtf8(node.attribute("param").value());

    components::BaseButton* button;
    if (action.length() > 0) {
        button = new components::LocalButton(parent, action, param);
    } else if (!node.attribute("buttonid").empty()) {
        button = new components::ServerButton(parent, buttonId);
    } else if (!node.attribute("page").empty()) {
        button = new components::PageButton(parent, pageId);
    } else {
        LOG_WARN(LOGTYPE_UI, "Button without action, id or page");
        return false;
    }

    parseId(node, button);
    button->set_geometry(bounds);
    button->setText(text);

    top->addToCurrentPage(button);
    return true;
}

bool GumpFactory::parseTCheckBox(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, parent);
    std::string text = node.attribute("text").value();
    int checked = node.attribute("checked").as_int();

    CL_CheckBox* cb = new CL_CheckBox(parent);
    parseId(node, cb);
    cb->set_geometry(bounds);
    cb->set_text(text);

    if (checked) {
        cb->set_checked(true);
    }

    top->addToCurrentPage(cb);
    return true;
}

bool GumpFactory::parseTRadioButton(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, parent);
    std::string text = node.attribute("text").value();
    std::string group = node.attribute("group").value();
    int selected = node.attribute("selected").as_int();

    if (group.length() == 0) {
        LOG_ERROR(LOGTYPE_UI, "Adding tradiobutton without group");
        return false;
    }

    CL_RadioButton* button = new CL_RadioButton(parent);
    parseId(node, button);
    button->set_geometry(bounds);
    button->set_text(text);
    button->set_group_name(group);

    if (selected) {
        button->set_selected(true);
    }

    top->addToCurrentPage(button);
    return true;
}

bool GumpFactory::parseTLineEdit(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, parent);
    UnicodeString text = StringConverter::fromUtf8(node.attribute("text").value());
    int numeric = node.attribute("numeric").as_int();
    int password = node.attribute("password").as_int();
    unsigned int maxlength = node.attribute("maxlength").as_uint();

    components::LineEdit* edit = new components::LineEdit(parent);
    parseId(node, edit);
    edit->setText(text);
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

    top->addToCurrentPage(edit);
    return true;
}

bool GumpFactory::parseTComboBox(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, parent);

    CL_ComboBox* box = new CL_ComboBox(parent);
    parseId(node, box);
    box->set_geometry(bounds);

    CL_PopupMenu menu;

    pugi::xml_node_iterator iter = node.begin();
    pugi::xml_node_iterator iterEnd = node.end();

    unsigned int selected = 0;

    for (unsigned int index = 0; iter != iterEnd; ++iter, ++index) {
        if (strcmp(iter->name(), "option") != 0) {
            LOGARG_WARN(LOGTYPE_UI, "Something different than option in combobox: \"%s\"", iter->name());
            return false;
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

    top->addToCurrentPage(box);
    return true;
}

bool GumpFactory::parseTGroupBox(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, parent);

    CL_GroupBox* box = new CL_GroupBox(parent);
    parseId(node, box);
    box->set_geometry(bounds);

    parseChildren(node, box, top);

    top->addToCurrentPage(box);
    return true;
}

bool GumpFactory::parseTSpin(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, parent);
    std::string type = node.attribute("type").value();

    CL_Spin* spin = new CL_Spin(parent);
    parseId(node, spin);

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

    top->addToCurrentPage(spin);
    return true;
}

bool GumpFactory::parseTTabs(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, parent);

    CL_Tab* tabs = new CL_Tab(parent);
    parseId(node, tabs);
    tabs->set_geometry(bounds);

    pugi::xml_node_iterator iter = node.begin();
    pugi::xml_node_iterator iterEnd = node.end();

    for (; iter != iterEnd; ++iter) {
        if (strcmp(iter->name(), "ttabpage") != 0) {
            LOGARG_WARN(LOGTYPE_UI, "Something different than ttabpage in ttabs: \"%s\"", iter->name());
            return false;
        } else {
            std::string tabTitle = iter->attribute("text").value();
            CL_TabPage* newpage = tabs->add_page(tabTitle);
            parseId(*iter, newpage);

            parseChildren(*iter, newpage, top);
        }
    }

    top->addToCurrentPage(tabs);
    return true;
}

bool GumpFactory::parseTSlider(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, parent);
    std::string type = node.attribute("type").value();
    int min = node.attribute("min").as_int();
    int max = node.attribute("max").as_int();
    unsigned int pagestep = node.attribute("pagestep").as_uint();
    int value = node.attribute("value").as_int();


    CL_Slider* slider = new CL_Slider(parent);
    parseId(node, slider);

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

    top->addToCurrentPage(slider);
    return true;
}

bool GumpFactory::parseTLabel(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, parent);
    std::string align = node.attribute("align").value();
    std::string text = node.attribute("text").value();

    CL_Label* label = new CL_Label(parent);
    parseId(node, label);

    if (align.length() == 0 || align == "left") {
        label->set_alignment(CL_Label::align_left);
    } else if (align == "right") {
        label->set_alignment(CL_Label::align_right);
    } else if (align == "center") {
        label->set_alignment(CL_Label::align_center);
    } else if (align == "justify") {
        label->set_alignment(CL_Label::align_justify);
    } else {
        LOGARG_WARN(LOGTYPE_UI, "Unknown label align: \"%s\"", align.c_str());
        return false;
    }

    label->set_text(text);
    label->set_geometry(bounds);

    top->addToCurrentPage(label);
    return true;
}

bool GumpFactory::parseTTextEdit(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    // does not seem to work currently (clanlib problem)

    //CL_Rect bounds = getBoundsFromNode(node);
    //std::string text = node.attribute("text").value();

    //CL_GUIComponentDescription desc;
    //std::string cssid = node.attribute("cssid").value();
    //desc.set_type_name("TextEdit");
    //if (cssid.length() > 0) {
        //desc.set_id_name(cssid);
    //}
    //CL_TextEdit* edit = new CL_TextEdit(desc, parent);
    //edit->set_geometry(bounds);

    //return true;

    LOG_WARN(LOGTYPE_UI, "TextEdit is currently not supported, sorry!");
    return false;
}

bool GumpFactory::parseImage(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    std::string path = node.attribute("path").value();
    unsigned int gumpId = node.attribute("gumpid").as_uint();
    unsigned int artId = node.attribute("artid").as_uint();

    int locX = node.attribute("x").as_int();
    int locY = node.attribute("y").as_int();
    unsigned int width = node.attribute("width").as_uint();
    unsigned int height = node.attribute("height").as_uint();

    CL_ImageView* img = new CL_ImageView(parent);
    parseId(node, img);

    // path takes precedence over gumpid, gumpid over artid
    if (path.length() > 0) {
        try {
            CL_PixelBuffer buf(path);
            img->set_image(buf);

            if (width == 0) {
                width = buf.get_width();
            }

            if (height == 0) {
                height = buf.get_height();
            }
        } catch (const CL_Exception& ex) {
            LOGARG_ERROR(LOGTYPE_UI, "Unable to load image from path %s: %s", path.c_str(), ex.what());
            return false;
        }
    } else if (gumpId) {
        boost::shared_ptr<ui::Texture> texture = data::Manager::getGumpArtLoader()->getTexture(gumpId);
        // TODO: find something better than busy waiting here
        while(!texture->isReadComplete()) {
            usleep(1);
        }

        img->set_image(*(texture->getPixelBuffer()));

        if (width == 0) {
            width = texture->getPixelBuffer()->get_width();
        }

        if (height == 0) {
            height = texture->getPixelBuffer()->get_height();
        }
    } else if (artId) {
        boost::shared_ptr<ui::Texture> texture = data::Manager::getArtLoader()->getItemTexture(artId);
        // TODO: find something better than busy waiting here
        while(!texture->isReadComplete()) {
            usleep(1);
        }

        img->set_image(*(texture->getPixelBuffer()));

        if (width == 0) {
            width = texture->getPixelBuffer()->get_width();
        }

        if (height == 0) {
            height = texture->getPixelBuffer()->get_height();
        }
    } else {
        LOG_WARN(LOGTYPE_UI, "Image component without image information! Please supply path, gumpid or artid");
        return false;
    }


    CL_Rect bounds(locX, locY, CL_Size(width, height));
    img->set_geometry(bounds);
    img->set_scale_to_fit();

    top->addToCurrentPage(img);
    return true;
}

bool GumpFactory::parsePage(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    unsigned int number = node.attribute("number").as_uint();

    if (top->getActivePageId() != 0) {
        // check that we add pages only at the top level hierarchy
        // adding a page inside another page
        LOGARG_ERROR(LOGTYPE_UI, "Adding page %i inside of page %i", top->getActivePageId(), number);
        return false;
    }

    top->addPage(number);

    bool ret = parseChildren(node, parent, top);

    top->activatePage(0);

    return ret;
}

bool GumpFactory::parseTScrollArea(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, parent);
    std::string hVisibilityStr = node.attribute("hvisible").value();
    std::string vVisibilityStr = node.attribute("vvisible").value();

    static std::string visibilityAlways("always");
    static std::string visibilityNever("never");
    static std::string visibilityOnDemand("ondemand");

    unsigned int hVisibility = components::ScrollArea::VISIBLE_ON_DEMAND;
    if (hVisibilityStr.length() > 0) {
        if (hVisibilityStr == visibilityAlways) {
            hVisibility = components::ScrollArea::VISIBLE_ALWAYS;
        } else if (hVisibilityStr == visibilityNever) {
            hVisibility = components::ScrollArea::VISIBLE_NEVER;
        } else if (hVisibilityStr == visibilityOnDemand) {
            hVisibility = components::ScrollArea::VISIBLE_ON_DEMAND;
        } else {
            LOGARG_ERROR(LOGTYPE_UI, "Unknown scrollbar hvisibility: %s. Possible values: always/never/ondemand", hVisibilityStr.c_str());
            return false;
        }
    }

    unsigned int vVisibility = components::ScrollArea::VISIBLE_ON_DEMAND;
    if (vVisibilityStr.length() > 0) {
        if (vVisibilityStr == visibilityAlways) {
            vVisibility = components::ScrollArea::VISIBLE_ALWAYS;
        } else if (vVisibilityStr == visibilityNever) {
            vVisibility = components::ScrollArea::VISIBLE_NEVER;
        } else if (vVisibilityStr == visibilityOnDemand) {
            vVisibility = components::ScrollArea::VISIBLE_ON_DEMAND;
        } else {
            LOGARG_ERROR(LOGTYPE_UI, "Unknown scrollbar vvisibility: %s. Possible values: always/never/ondemand", vVisibilityStr.c_str());
            return false;
        }
    }


    components::ScrollArea* area = new components::ScrollArea(parent);
    parseId(node, area);
    area->set_geometry(bounds);

    unsigned int hLineStep = node.attribute("hstep").as_uint();
    unsigned int hPageStep = node.attribute("hpage").as_uint();
    unsigned int vLineStep = node.attribute("vstep").as_uint();
    unsigned int vPageStep = node.attribute("vpage").as_uint();
    unsigned int marginLeft = node.attribute("marginleft").as_uint();
    unsigned int marginBottom = node.attribute("marginbottom").as_uint();

    top->addToCurrentPage(area);

    parseChildren(node, area->getClientArea(), top);

    area->updateScrollbars(vVisibility, hVisibility, vPageStep, hPageStep, vLineStep, hLineStep, marginLeft, marginBottom);

    return true;
}

bool GumpFactory::parseRepeat(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    UnicodeString name(node.attribute("name").value());

    if (repeatContexts_.count(name) == 0) {
        // TODO: include log msg again
        //LOGARG_ERROR(LOGTYPE_UI, "Trying to access unknown repeat context %s", name.c_str());
        return false;
    }

    const RepeatContext& context = repeatContexts_[name];

    int xIncrease = node.attribute("xincrease").as_int();
    int yIncrease = node.attribute("yincrease").as_int();
    unsigned int xLimit = node.attribute("xlimit").as_uint();
    unsigned int yLimit = node.attribute("ylimit").as_uint();

    unsigned int xIndex = 0;
    unsigned int yIndex = 0;

    pugi::xml_node::iterator nodeIter;
    pugi::xml_node::iterator nodeEnd = node.end();

    pugi::xml_node::attribute_iterator attrIter;
    pugi::xml_node::attribute_iterator attrEnd;

    std::map<RepeatKeyword, std::vector<UnicodeString> >::const_iterator contextIter;
    std::map<RepeatKeyword, std::vector<UnicodeString> >::const_iterator contextEnd = context.keywordReplacments_.end();

    for (unsigned int index = 0; index < context.repeatCount_; ++index) {

        for (nodeIter = node.begin(); nodeIter != nodeEnd; ++nodeIter) {
            pugi::xml_node newNode = node.parent().insert_child_after(nodeIter->type(), node);
            newNode.set_name(nodeIter->name());

            attrIter = nodeIter->attributes_begin();
            attrEnd = nodeIter->attributes_end();

            for (; attrIter != attrEnd; ++attrIter) {
                bool contextHit = false;

                for (contextIter = context.keywordReplacments_.begin(); contextIter != contextEnd; ++contextIter) {
                    if (contextIter->first.nodeText_ == nodeIter->name() &&
                            contextIter->first.attributeText_ == attrIter->name() &&
                            contextIter->first.searchText_ == attrIter->value()) {
                        contextHit = true;

                        pugi::xml_attribute newAttr = newNode.append_attribute(attrIter->name());
                        newAttr.set_value(StringConverter::toUtf8String(contextIter->second[index]).c_str());
                    }
                }

                if (!contextHit) {
                    pugi::xml_attribute newAttr = newNode.append_attribute(attrIter->name());

                    // increase and y values, if found
                    static std::string attrNameX("x");
                    static std::string attrNameY("y");
                    static std::string attrValueCenter("center");
                    if (attrNameX == newAttr.name() && attrValueCenter != attrIter->value()) {
                        int baseX = attrIter->as_int();
                        int curXIncrease = xIncrease * xIndex;
                        int curX = baseX + curXIncrease;
                        newAttr.set_value(curX);

                        if (++xIndex >= xLimit && xLimit != 0) {
                            xIndex = 0;
                        }
                    } else if (attrNameY == newAttr.name() && attrValueCenter != attrIter->value()) {
                        int baseY = attrIter->as_int();
                        int curYIncrease = yIncrease * yIndex;
                        int curY = baseY + curYIncrease;
                        newAttr.set_value(curY);

                        if (++yIndex >= yLimit && yLimit != 0) {
                            yIndex = 0;
                        }
                    } else {
                        newAttr.set_value(attrIter->value());
                    }
                }
            }
        }
    }

    return true;
}

}
}
