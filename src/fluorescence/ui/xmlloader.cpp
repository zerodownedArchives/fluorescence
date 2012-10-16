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



#include "xmlloader.hpp"

#include <ClanLib/Display/Image/pixel_buffer.h>

#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>

#include <platform.hpp>
#include <misc/log.hpp>
#include <client.hpp>
#include <data/manager.hpp>
#include <data/clilocloader.hpp>

#include "manager.hpp"
#include "texture.hpp"
#include "components/button.hpp"
#include "components/scrollarea.hpp"
#include "components/scrollbar.hpp"
#include "components/lineedit.hpp"
#include "components/label.hpp"
#include "components/clicklabel.hpp"
#include "components/worldview.hpp"
#include "components/propertylabel.hpp"
#include "components/paperdollview.hpp"
#include "components/containerview.hpp"
#include "components/image.hpp"
#include "components/background.hpp"
#include "components/checkbox.hpp"
#include "components/sysloglabel.hpp"
#include "components/warmodebutton.hpp"
#include "components/radiobutton.hpp"
#include "components/alpharegion.hpp"

namespace fluo {
namespace ui {

XmlLoader* XmlLoader::singleton_ = NULL;

XmlLoader* XmlLoader::getSingleton() {
    if (!singleton_) {
        singleton_ = new XmlLoader();
    }

    return singleton_;
}

XmlLoader::XmlLoader() {
    functionTable_["image"] = boost::bind(&XmlLoader::parseImage, this, _1, _2, _3, _4);
    functionTable_["button"] = boost::bind(&XmlLoader::parseButton, this, _1, _2, _3, _4);
    functionTable_["background"] = boost::bind(&XmlLoader::parseBackground, this, _1, _2, _3, _4);
    functionTable_["checkbox"] = boost::bind(&XmlLoader::parseCheckbox, this, _1, _2, _3, _4);
    functionTable_["radiobutton"] = boost::bind(&XmlLoader::parseRadioButton, this, _1, _2, _3, _4);
    functionTable_["label"] = boost::bind(&XmlLoader::parseLabel, this, _1, _2, _3, _4);
    functionTable_["propertylabel"] = boost::bind(&XmlLoader::parsePropertyLabel, this, _1, _2, _3, _4);
    functionTable_["sysloglabel"] = boost::bind(&XmlLoader::parseSysLogLabel, this, _1, _2, _3, _4);
    functionTable_["clicklabel"] = boost::bind(&XmlLoader::parseClickLabel, this, _1, _2, _3, _4);
    functionTable_["lineedit"] = boost::bind(&XmlLoader::parseLineEdit, this, _1, _2, _3, _4);
    functionTable_["scrollarea"] = boost::bind(&XmlLoader::parseScrollArea, this, _1, _2, _3, _4);
    functionTable_["htmllabel"] = boost::bind(&XmlLoader::parseHtmlLabel, this, _1, _2, _3, _4);
    functionTable_["alpharegion"] = boost::bind(&XmlLoader::parseAlphaRegion, this, _1, _2, _3, _4);

    functionTable_["worldview"] = boost::bind(&XmlLoader::parseWorldView, this, _1, _2, _3, _4);
    functionTable_["paperdoll"] = boost::bind(&XmlLoader::parsePaperdoll, this, _1, _2, _3, _4);
    functionTable_["container"] = boost::bind(&XmlLoader::parseContainer, this, _1, _2, _3, _4);
    functionTable_["warmodebutton"] = boost::bind(&XmlLoader::parseWarModeButton, this, _1, _2, _3, _4);
}


void XmlLoader::addRepeatContext(const UnicodeString& name, const RepeatContext& context) {
    getSingleton()->repeatContexts_[name] = context;
}

void XmlLoader::removeRepeatContext(const UnicodeString& name) {
    getSingleton()->repeatContexts_.erase(name);
}

void XmlLoader::clearRepeatContexts() {
    getSingleton()->repeatContexts_.clear();
}


bool XmlLoader::readTemplateFile(const boost::filesystem::path& themePath) {
    boost::filesystem::path path = themePath / "templates.xml";

    if (!boost::filesystem::exists(path)) {
        LOG_ERROR << "Unable to load templates.xml for theme " << themePath << ", file not found" << std::endl;
        return false;
    }

    LOG_DEBUG << "Parsing templates.xml for theme " << themePath << std::endl;

    pugi::xml_parse_result result = getSingleton()->templateDocument_.load_file(path.string().c_str());

    if (result) {
        getSingleton()->setTemplates();

        return true;
    } else {
        LOG_ERROR << "Error parsing templates.xml file at offset " << result.offset << ": " << result.description() << std::endl;
        return false;
    }
}

void XmlLoader::setTemplates() {
    templateMap_.clear();
    defaultTemplateMap_.clear();

    pugi::xml_node rootNode = templateDocument_.child("templates");

    pugi::xml_node_iterator iter = rootNode.begin();
    pugi::xml_node_iterator iterEnd = rootNode.end();

    for (; iter != iterEnd; ++iter) {
        pugi::xml_node componentNode = iter->first_child();

        if (functionTable_.find(componentNode.name()) != functionTable_.end()) {
            // register template by name, if it has one
            if (iter->attribute("name")) {
                templateMap_[iter->attribute("name").value()] = componentNode;
            }

            // if default, register by component name
            if (iter->attribute("default").as_bool()) {
                defaultTemplateMap_[componentNode.name()] = componentNode;
            }
        } else {
            LOG_WARN << "Unknown gump tag " << componentNode.name() << " in templates file" << std::endl;
        }
    }
}

pugi::xml_node XmlLoader::getTemplate(const UnicodeString& templateName) {
    std::map<UnicodeString, pugi::xml_node>::iterator iter = templateMap_.find(templateName);
    if (iter != templateMap_.end()) {
        return iter->second;
    } else {
        LOG_WARN << "Trying to access unknown gump component template " << templateName << std::endl;
        static pugi::xml_node ret;
        return ret;
    }
}

pugi::xml_attribute XmlLoader::getAttribute(const char* name, pugi::xml_node& node, pugi::xml_node& defaultNode) {
    pugi::xml_attribute ret = node.attribute(name);
    if (!ret && defaultNode) {
        ret = defaultNode.attribute(name);
    }
    return ret;
}

pugi::xml_node XmlLoader::getNode(const char* name, pugi::xml_node& node, pugi::xml_node& defaultNode) {
    pugi::xml_node ret = node.child(name);
    if (!ret && defaultNode) {
        ret = defaultNode.child(name);
    }
    return ret;
}


GumpMenu* XmlLoader::fromXmlFile(const UnicodeString& name) {
    boost::filesystem::path path = "gumps";
    std::string utf8FileName = StringConverter::toUtf8String(name) + ".xml";
    path = path / utf8FileName;

    path = data::Manager::getShardFilePath(path);

    if (!boost::filesystem::exists(path)) {
        LOG_ERROR << "Unable to load gump xml, file not found: " << utf8FileName << std::endl;
        return nullptr;
    }

    LOG_DEBUG << "Parsing xml gump file: " << path << std::endl;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.string().c_str());

    if (result) {
        GumpMenu* ret = getSingleton()->fromXml(doc);

        // save transformed document (debug)
        //LOG_DEBUG << "Saving result: " << doc.save_file("out.xml") << std::endl;
        //LOG_DEBUG << "Transformed xml:" << std::endl;
        //doc.print(std::cout);

        if (ret) {
            ret->setName(name);
        }

        return ret;
    } else {
        LOG_ERROR << "Error parsing gump xml file at offset " << result.offset << ": " << result.description() << std::endl;
        return nullptr;
    }
}

GumpMenu* XmlLoader::fromXmlString(const UnicodeString& str) {
    pugi::xml_document doc;
    std::string utf8String = StringConverter::toUtf8String(str);
    pugi::xml_parse_result result = doc.load_buffer(utf8String.c_str(), utf8String.length());

    if (result) {
        GumpMenu* ret = getSingleton()->fromXml(doc);

        return ret;
    } else {
        LOG_ERROR << "Error parsing gump xml string at offset " << result.offset << ": " << result.description() << std::endl;
        return nullptr;
    }
}

GumpMenu* XmlLoader::fromXml(pugi::xml_document& doc) {
    pugi::xml_node rootNode = doc.child("gump");

    pugi::xml_node dummy;
    CL_Rect bounds = getBoundsFromNode(rootNode, dummy);
    bounds.set_width(1);
    bounds.set_height(1);

    bool closable = rootNode.attribute("closable").as_bool();
    bool draggable = rootNode.attribute("draggable").as_bool();
    UnicodeString action = StringConverter::fromUtf8(rootNode.attribute("action").value());
    UnicodeString cancelAction = StringConverter::fromUtf8(rootNode.attribute("cancelaction").value());
    bool inbackground = rootNode.attribute("inbackground").as_bool();

    CL_GUITopLevelDescription desc(bounds, false);
    desc.set_decorations(false);
    desc.set_in_background(inbackground);


    GumpMenu* ret = new GumpMenu(desc);
    ret->setClosable(closable);
    ret->setDraggable(draggable);

    if (action.length() > 0) {
        ret->setAction(action);
    }

    if (cancelAction.length() > 0) {
        ret->setCancelAction(cancelAction);
    }

    parseChildren(rootNode, ret, ret);

    ret->fitSizeToChildren();
    ret->activateFirstPage();

    return ret;
}

GumpComponent* XmlLoader::parseChildren(pugi::xml_node& rootNode, CL_GUIComponent* parent, GumpMenu* top) {
    pugi::xml_node_iterator iter = rootNode.begin();
    pugi::xml_node_iterator iterEnd = rootNode.end();

    bool success = true;
    GumpComponent* lastComponent = nullptr;

    for (; iter != iterEnd && success; ++iter) {
        if (strcmp(iter->name(), "repeat") == 0) {
            success = parseRepeat(*iter, parent, top);
        } else if (strcmp(iter->name(), "page") == 0) {
            success = parsePage(*iter, parent, top);
        } else {
            std::map<UnicodeString, XmlParseFunction>::iterator function = functionTable_.find(iter->name());

            //LOG_DEBUG << "Gump Component: " << iter->name() << std::endl;

            if (function != functionTable_.end()) {
                pugi::xml_node defaultNode;
                if (iter->attribute("template")) {
                    defaultNode = getTemplate(iter->attribute("template").value());
                }

                if (!defaultNode) {
                    defaultNode = defaultTemplateMap_[iter->name()];
                }

                lastComponent = (function->second)(*iter, defaultNode, parent, top);
                success = lastComponent != nullptr;
            } else {
                LOG_WARN << "Unknown gump tag " << iter->name() << std::endl;
            }
        }
    }

    return lastComponent;
}


CL_Rect XmlLoader::getBoundsFromNode(pugi::xml_node& node, pugi::xml_node& defaultNode) {
    unsigned int width = getAttribute("width", node, defaultNode).as_uint();
    unsigned int height = getAttribute("height", node, defaultNode).as_uint();
    int locX = getAttribute("x", node, defaultNode).as_int();
    int locY = getAttribute("y", node, defaultNode).as_int();

    CL_Rect bounds(locX, locY, CL_Size(width, height));
    return bounds;
}

bool XmlLoader::parseId(pugi::xml_node& node, CL_GUIComponent* component) {
    std::string cssid = node.attribute("name").value();
    if (cssid.length() > 0) {
        component->set_id_name(cssid);
    }

    return true;
}

boost::shared_ptr<ui::Texture> XmlLoader::parseTexture(pugi::xml_node& node, pugi::xml_node& defaultNode) {
    UnicodeString imgSource = StringConverter::fromUtf8(getAttribute("source", node, defaultNode).value());
    UnicodeString imgId = StringConverter::fromUtf8(getAttribute("id", node, defaultNode).value());

    boost::shared_ptr<ui::Texture> texture = data::Manager::getTexture(imgSource, imgId);
    texture->setUsage(ui::Texture::USAGE_GUMP);

    return texture;
}

bool XmlLoader::parseMultiTextureImage(pugi::xml_node& node, pugi::xml_node& defaultNode, components::MultiTextureImage* img, unsigned int index) {
    boost::shared_ptr<ui::Texture> texture = parseTexture(node, defaultNode);

    if (!texture) {
        LOG_ERROR << "Unable to parse gump image, source=" << getAttribute("source", node, defaultNode) << " id=" << getAttribute("id", node, defaultNode) << std::endl;
        return false;
    }

    unsigned int hue = getAttribute("hue", node, defaultNode).as_uint();
    std::string rgba = getAttribute("color", node, defaultNode).value();
    float alpha = getAttribute("alpha", node, defaultNode).as_float();

    bool tiled = getAttribute("tiled", node, defaultNode).as_bool();

    img->addTexture(index, texture, hue, rgba, alpha, tiled);

    return true;
}

bool XmlLoader::parseButtonText(pugi::xml_node& node, pugi::xml_node& defaultNode, components::Button* button, unsigned int index) {
    unsigned int hue = getAttribute("font-hue", node, defaultNode).as_uint();
    std::string rgba = getAttribute("font-color", node, defaultNode).value();
    UnicodeString text = getAttribute("text", node, defaultNode).value();
    CL_Colorf color(rgba);

    if (text.length() > 0) {
        button->setText(index, text);
    }

    // if the node has its own color or hue property, don't use the template values
    if (node.attribute("font-color")) {
        button->setFontColor(index, color);
    } else if (node.attribute("font-hue")) {
        button->setFontHue(index, hue);
    } else if (rgba.length() > 0) {
        button->setFontColor(index, color);
    } else {
        button->setFontHue(index, hue);
    }

    return true;
}

bool XmlLoader::parseLabelHelper(components::Label* label, pugi::xml_node& node, pugi::xml_node& defaultNode) {
    std::string align = getAttribute("align", node, defaultNode).value();
    UnicodeString fontName = getAttribute("font", node, defaultNode).value();
    unsigned int fontHeight = getAttribute("font-height", node, defaultNode).as_uint();
    unsigned int hue = getAttribute("hue", node, defaultNode).as_uint();
    std::string rgba = getAttribute("color", node, defaultNode).value();
    CL_Colorf color(rgba);

    parseId(node, label);

    if (align.length() == 0 || align == "left") {
        label->setAlignment(components::Label::Alignment::LEFT);
    } else if (align == "right") {
        label->setAlignment(components::Label::Alignment::RIGHT);
    } else if (align == "center") {
        label->setAlignment(components::Label::Alignment::CENTER);
    } else {
        LOG_WARN << "Unknown label align: " << align << std::endl;
        return false;
    }

    if (label->getAlignment() != components::Label::Alignment::LEFT && label->get_width() == 0) {
        LOG_WARN << "Label with alignment != left, but no width detected. Setting width to 100" << std::endl;
        CL_Rectf geom(label->get_geometry());
        geom.set_width(100);
        label->set_geometry(geom);
    }

    label->setFont(fontName, fontHeight);

    // if the node has its own color or hue property, don't use the template values
    if (node.attribute("color")) {
        label->setColor(color);
    } else if (node.attribute("hue")) {
        label->setHue(hue);
    } else if (rgba.length() > 0) {
        label->setColor(color);
    } else if (getAttribute("hue", node, defaultNode)) { // does it even have a hue attribute?
        label->setHue(hue);
    }

    return true;
}

bool XmlLoader::parseScrollBarTextures(boost::shared_ptr<ui::Texture>* textures, CL_Colorf* colors, CL_Vec3f* hueInfos, pugi::xml_node& node, pugi::xml_node& defaultNode) {
    pugi::xml_node normalNode = node.child("normal");
    pugi::xml_node mouseOverNode = node.child("mouseover");
    pugi::xml_node mouseDownNode = node.child("mousedown");

    pugi::xml_node defaultNormalNode = defaultNode.child("normal");
    pugi::xml_node defaultMouseOverNode = defaultNode.child("mouseover");
    pugi::xml_node defaultMouseDownNode = defaultNode.child("mousedown");

    if ((normalNode || defaultNormalNode) && (mouseOverNode || defaultMouseOverNode) && (mouseDownNode || defaultMouseDownNode)) {
        textures[components::ScrollBar::TEX_INDEX_UP] = parseTexture(normalNode, defaultNormalNode);
        hueInfos[components::ScrollBar::TEX_INDEX_UP][1u] = getAttribute("hue", normalNode, defaultNormalNode).as_uint();
        std::string rgba = getAttribute("color", normalNode, defaultNormalNode).value();
        if (rgba.length() > 0) {
            colors[components::ScrollBar::TEX_INDEX_UP] = CL_Colorf(rgba);
        }

        textures[components::ScrollBar::TEX_INDEX_MOUSEOVER] = parseTexture(mouseOverNode, defaultMouseOverNode);
        hueInfos[components::ScrollBar::TEX_INDEX_MOUSEOVER][1u] = getAttribute("hue", mouseOverNode, defaultMouseOverNode).as_uint();
        rgba = getAttribute("color", mouseOverNode, defaultMouseOverNode).value();
        if (rgba.length() > 0) {
            colors[components::ScrollBar::TEX_INDEX_MOUSEOVER] = CL_Colorf(rgba);
        }

        textures[components::ScrollBar::TEX_INDEX_DOWN] = parseTexture(mouseDownNode, defaultMouseDownNode);
        hueInfos[components::ScrollBar::TEX_INDEX_DOWN][1u] = getAttribute("hue", mouseDownNode, defaultMouseDownNode).as_uint();
        rgba = getAttribute("color", mouseDownNode, defaultMouseDownNode).value();
        if (rgba.length() > 0) {
            colors[components::ScrollBar::TEX_INDEX_DOWN] = CL_Colorf(rgba);
        }
    } else {
        LOG_ERROR << "Scrollbar needs definitions for normal, mouseover and mousedown textures for all components" << std::endl;
        return false;
    }

    if (!textures[components::ScrollBar::TEX_INDEX_UP] || !textures[components::ScrollBar::TEX_INDEX_MOUSEOVER] || !textures[components::ScrollBar::TEX_INDEX_DOWN]) {
        LOG_ERROR << "Invalid scrollbar texture (texture not found)" << std::endl;
        return false;
    }

    return true;
}

bool XmlLoader::parseScrollBar(components::ScrollBar* bar, components::ScrollArea* parent, bool vertical, pugi::xml_node node, pugi::xml_node defaultNode) {
    bool visible = getAttribute("visible", node, defaultNode).as_bool();
    bar->set_visible(visible);

    // don't bother parsing if it is not visible anyway
    if (!visible) {
        return true;
    }

    if (vertical) {
        unsigned int width = getAttribute("width", node, defaultNode).as_uint();
        if (width == 0) {
            LOG_ERROR << "Vertical scrollbar without width is not possible" << std::endl;
            return false;
        }
        // only need to set the final width here, position and height is calculated in scrollarea
        bar->set_geometry(CL_Rectf(0, 0, CL_Sizef(width, 1)));
    } else {
        unsigned int height = getAttribute("height", node, defaultNode).as_uint();
        if (height == 0) {
            LOG_ERROR << "Horizontal scrollbar without height is not possible" << std::endl;
            return false;
        }
        // only need to set the final height here, position and width is calculated in scrollarea
        bar->set_geometry(CL_Rectf(0, 0, CL_Sizef(1, height)));
    }

    pugi::xml_node thumbNode = node.child("thumb");
    pugi::xml_node defaultThumbNode = defaultNode.child("thumb");
    if (thumbNode || defaultThumbNode) {
        if (!parseScrollBarTextures(bar->thumbTextures_, bar->thumbColors_, bar->thumbHueInfos_, thumbNode, defaultThumbNode)) {
            LOG_ERROR << "Error parsing scrollbar thumb" << std::endl;
            return false;
        }
    } else {
        LOG_ERROR << "No thumb node given for scrollbar" << std::endl;
        return false;
    }

    pugi::xml_node incNode = node.child("increment");
    pugi::xml_node defaultIncNode = defaultNode.child("increment");
    if (incNode || defaultIncNode) {
        if (!parseScrollBarTextures(bar->incrementTextures_, bar->incrementColors_, bar->incrementHueInfos_, incNode, defaultIncNode)) {
            LOG_ERROR << "Error parsing scrollbar increment" << std::endl;
            return false;
        }
    } else {
        LOG_ERROR << "No increment node given for scrollbar" << std::endl;
        return false;
    }

    pugi::xml_node decNode = node.child("decrement");
    pugi::xml_node defaultDecNode = defaultNode.child("decrement");
    if (decNode || defaultDecNode) {
        if (!parseScrollBarTextures(bar->decrementTextures_, bar->decrementColors_, bar->decrementHueInfos_, decNode, defaultDecNode)) {
            LOG_ERROR << "Error parsing scrollbar decrement" << std::endl;
            return false;
        }
    } else {
        LOG_ERROR << "No decrement node given for scrollbar" << std::endl;
        return false;
    }

    pugi::xml_node trackNode = node.child("track");
    pugi::xml_node defaultTrackNode = defaultNode.child("track");
    if (trackNode || defaultTrackNode) {
        bar->trackTexture_ = parseTexture(trackNode, defaultTrackNode);
        bar->trackHueInfo_[1u] = getAttribute("hue", trackNode, defaultTrackNode).as_uint();
        std::string rgba = getAttribute("color", trackNode, defaultTrackNode).value();
        if (rgba.length() > 0) {
            bar->trackColor_ = CL_Colorf(rgba);
        }
    } else {
        LOG_ERROR << "No track node given for scrollbar" << std::endl;
        return false;
    }

    //unsigned int lineStep = node.attribute("linestep").as_uint();
    //unsigned int pageStep = node.attribute("pagestep").as_uint();

    //if (lineStep > 0) {
        //bar->set_line_step(lineStep);
    //}
    //if (pageStep > 0) {
        //bar->set_page_step(pageStep);
    //}

    return true;
}


GumpComponent* XmlLoader::parseImage(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);

    unsigned int hue = getAttribute("hue", node, defaultNode).as_uint();
    std::string rgba = getAttribute("color", node, defaultNode).value();
    float alpha = getAttribute("alpha", node, defaultNode).as_float();

    bool tiled = getAttribute("tiled", node, defaultNode).as_bool();

    components::Image* img = new components::Image(parent);
    parseId(node, img);

    boost::shared_ptr<ui::Texture> texture = parseTexture(node, defaultNode);
    if (!texture) {
        LOG_ERROR << "Unable to parse gump image, source=" << getAttribute("source", node, defaultNode) << " id=" << getAttribute("id", node, defaultNode) << std::endl;
        return nullptr;
    }

    if (bounds.get_width() == 0 || bounds.get_height() == 0) {
        if (texture->getWidth() != 1) {
            bounds.set_width(texture->getWidth());
            bounds.set_height(texture->getHeight());
        } else {
            img->setAutoResize(true);
            bounds.set_width(1);
            bounds.set_height(1);
        }
    } else if (tiled) {
        img->setTiled(true);
    }

    img->setTexture(texture);
    img->set_geometry(bounds);

    img->setHue(hue);
    if (rgba.length() > 0) {
        img->setRgba(CL_Colorf(rgba));
    }

    if (alpha) {
        img->setAlpha(alpha);
    }

    top->addToCurrentPage(img);
    return img;
}

GumpComponent* XmlLoader::parseButton(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);
    unsigned int buttonId = getAttribute("buttonid", node, defaultNode).as_uint();
    unsigned int pageId = getAttribute("page", node, defaultNode).as_uint();
    UnicodeString action = StringConverter::fromUtf8(getAttribute("action", node, defaultNode).value());
    UnicodeString param = StringConverter::fromUtf8(getAttribute("param", node, defaultNode).value());
    UnicodeString param2 = StringConverter::fromUtf8(getAttribute("param2", node, defaultNode).value());
    UnicodeString param3 = StringConverter::fromUtf8(getAttribute("param3", node, defaultNode).value());
    UnicodeString param4 = StringConverter::fromUtf8(getAttribute("param4", node, defaultNode).value());
    UnicodeString param5 = StringConverter::fromUtf8(getAttribute("param5", node, defaultNode).value());

    std::string align = getAttribute("font-align", node, defaultNode).value();
    UnicodeString fontName = getAttribute("font", node, defaultNode).value();
    unsigned int fontHeight = getAttribute("font-height", node, defaultNode).as_uint();
    UnicodeString text = getAttribute("text", node, defaultNode).value();

    components::Button* button = new components::Button(parent);
    if (action.length() > 0) {
        button->setLocalButton(action);
        button->setParameter(param, 0);
        button->setParameter(param2, 1);
        button->setParameter(param3, 2);
        button->setParameter(param4, 3);
        button->setParameter(param5, 4);
    } else if (!getAttribute("buttonid", node, defaultNode).empty()) {
        button->setServerButton(buttonId);
    } else if (!getAttribute("page", node, defaultNode).empty()) {
        button->setPageButton(pageId);
    } else {
        LOG_WARN << "Button without action, id or page" << std::endl;
        return nullptr;
    }

    if (align == "left") {
        button->setFontAlignment(components::Label::Alignment::LEFT);
    } else if (align == "right") {
        button->setFontAlignment(components::Label::Alignment::RIGHT);
    } else if (align.length() == 0 || align == "center") {
        button->setFontAlignment(components::Label::Alignment::CENTER);
    } else {
        LOG_WARN << "Unknown button align: " << align << std::endl;
        return nullptr;
    }
    button->setFont(fontName, fontHeight);

    if (text.length() > 0) {
        button->setText(0, text);
        button->setText(1, text);
        button->setText(2, text);
    }

    parseId(node, button);

    pugi::xml_node normalNode = node.child("normal");
    pugi::xml_node mouseOverNode = node.child("mouseover");
    pugi::xml_node mouseDownNode = node.child("mousedown");

    pugi::xml_node defaultNormalNode = defaultNode.child("normal");
    pugi::xml_node defaultMouseOverNode = defaultNode.child("mouseover");
    pugi::xml_node defaultMouseDownNode = defaultNode.child("mousedown");

    if (normalNode || defaultNormalNode) {
        parseMultiTextureImage(normalNode, defaultNormalNode, button, components::Button::TEX_INDEX_UP);
        parseButtonText(normalNode, defaultNormalNode, button, components::Button::TEX_INDEX_UP);
    } else {
        LOG_ERROR << "Normal image for uo button not defined" << std::endl;
        node.print(std::cout);
        return nullptr;
    }

    if (mouseOverNode || defaultMouseOverNode) {
        parseMultiTextureImage(mouseOverNode, defaultMouseOverNode, button, components::Button::TEX_INDEX_MOUSEOVER);
        parseButtonText(mouseOverNode, defaultMouseOverNode, button, components::Button::TEX_INDEX_MOUSEOVER);
    }
    if (mouseDownNode || defaultMouseDownNode) {
        parseMultiTextureImage(mouseDownNode, defaultMouseDownNode, button, components::Button::TEX_INDEX_DOWN);
        parseButtonText(mouseDownNode, defaultMouseDownNode, button, components::Button::TEX_INDEX_DOWN);
    }

    if (bounds.get_width() == 0 || bounds.get_height() == 0) {
        button->setAutoResize(true);
        bounds.set_width(1);
        bounds.set_height(1);
    }

    button->updateTexture();

    button->set_geometry(bounds);

    top->addToCurrentPage(button);

    return button;
}

GumpComponent* XmlLoader::parseBackground(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);

    unsigned int hue = getAttribute("hue", node, defaultNode).as_uint();
    std::string rgba = getAttribute("color", node, defaultNode).value();
    float alpha = getAttribute("alpha", node, defaultNode).as_float();

    unsigned int gumpId = getAttribute("gumpid", node, defaultNode).as_uint();

    if (!gumpId) {
        LOG_ERROR << "Unable to parse background, gumpid not found, not a number or zero" << std::endl;
        return nullptr;
    }

    components::Background* img = new components::Background(parent);
    parseId(node, img);

    img->set_geometry(bounds);

    img->setBaseId(gumpId);

    img->setHue(hue);
    if (rgba.length() > 0) {
        img->setRgba(CL_Colorf(rgba));
    }

    if (alpha) {
        img->setAlpha(alpha);
    }

    top->addToCurrentPage(img);
    return img;
}

GumpComponent* XmlLoader::parseCheckbox(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);
    unsigned int switchId = getAttribute("switchid", node, defaultNode).as_uint();
    bool isChecked = getAttribute("checked", node, defaultNode).as_bool();

    components::Checkbox* cb = new components::Checkbox(parent);
    cb->setSwitchId(switchId);

    parseId(node, cb);

    pugi::xml_node uncheckedNode = node.child("unchecked");
    pugi::xml_node uncheckedMoNode = node.child("unchecked-mouseover");
    pugi::xml_node checkedNode = node.child("checked");
    pugi::xml_node checkedMoNode = node.child("checked-mouseover");

    pugi::xml_node defUncheckedNode = defaultNode.child("unchecked");
    pugi::xml_node defUncheckedMoNode = defaultNode.child("unchecked-mouseover");
    pugi::xml_node defCheckedNode = defaultNode.child("checked");
    pugi::xml_node defCheckedMoNode = defaultNode.child("checked-mouseover");

    if ((checkedNode || defCheckedNode) && (uncheckedNode || defUncheckedNode)) {
        parseMultiTextureImage(uncheckedNode, defUncheckedNode, cb, components::Checkbox::TEX_INDEX_UNCHECKED);
        parseMultiTextureImage(checkedNode, defCheckedNode, cb, components::Checkbox::TEX_INDEX_CHECKED);
    } else {
        LOG_ERROR << "Checkbox needs to have checked and unchecked image node" << std::endl;
        return nullptr;
    }

    if (uncheckedMoNode || defUncheckedMoNode) {
        parseMultiTextureImage(uncheckedMoNode, defUncheckedMoNode, cb, components::Checkbox::TEX_INDEX_UNCHECKED_MOUSEOVER);
    }
    if (checkedMoNode || defCheckedMoNode) {
        parseMultiTextureImage(checkedMoNode, defCheckedMoNode, cb, components::Checkbox::TEX_INDEX_CHECKED_MOUSEOVER);
    }

    if (bounds.get_width() == 0 || bounds.get_height() == 0) {
        cb->setAutoResize(true);
        bounds.set_width(1);
        bounds.set_height(1);
    }

    cb->setChecked(isChecked);

    cb->set_geometry(bounds);

    top->addToCurrentPage(cb);
    return cb;
}

GumpComponent* XmlLoader::parseRadioButton(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);
    unsigned int switchId = getAttribute("switchid", node, defaultNode).as_uint();
    bool isChecked = getAttribute("checked", node, defaultNode).as_bool();
    unsigned int groupId = getAttribute("group", node, defaultNode).as_uint();

    components::RadioButton* rb = new components::RadioButton(parent);
    rb->setSwitchId(switchId);
    rb->setRadioGroupId(groupId);

    parseId(node, rb);

    pugi::xml_node uncheckedNode = node.child("unchecked");
    pugi::xml_node uncheckedMoNode = node.child("unchecked-mouseover");
    pugi::xml_node checkedNode = node.child("checked");
    pugi::xml_node checkedMoNode = node.child("checked-mouseover");

    pugi::xml_node defUncheckedNode = defaultNode.child("unchecked");
    pugi::xml_node defUncheckedMoNode = defaultNode.child("unchecked-mouseover");
    pugi::xml_node defCheckedNode = defaultNode.child("checked");
    pugi::xml_node defCheckedMoNode = defaultNode.child("checked-mouseover");

    if ((checkedNode || defCheckedNode) && (uncheckedNode || defUncheckedNode)) {
        parseMultiTextureImage(uncheckedNode, defUncheckedNode, rb, components::Checkbox::TEX_INDEX_UNCHECKED);
        parseMultiTextureImage(checkedNode, defCheckedNode, rb, components::Checkbox::TEX_INDEX_CHECKED);
    } else {
        LOG_ERROR << "radiobutton needs to have checked and unchecked image node" << std::endl;
        return nullptr;
    }

    if (uncheckedMoNode || defUncheckedMoNode) {
        parseMultiTextureImage(uncheckedMoNode, defUncheckedMoNode, rb, components::Checkbox::TEX_INDEX_UNCHECKED_MOUSEOVER);
    }
    if (checkedMoNode || defCheckedMoNode) {
        parseMultiTextureImage(checkedMoNode, defCheckedMoNode, rb, components::Checkbox::TEX_INDEX_CHECKED_MOUSEOVER);
    }

    if (bounds.get_width() == 0 || bounds.get_height() == 0) {
        rb->setAutoResize(true);
        bounds.set_width(1);
        bounds.set_height(1);
    }

    rb->setChecked(isChecked);

    rb->set_geometry(bounds);

    top->addToCurrentPage(rb);
    return rb;
}

GumpComponent* XmlLoader::parseLabel(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);
    UnicodeString text = getAttribute("text", node, defaultNode).value();

    unsigned int cliloc = getAttribute("cliloc", node, defaultNode).as_uint();
    if (cliloc) {
        UnicodeString args = getAttribute("args", node, defaultNode).value();
        args.findAndReplace("\\t", "\t");
        text = data::Manager::getClilocLoader()->get(cliloc, args);
    }

    components::Label* label = new components::Label(parent);
    label->set_geometry(bounds);
    if (!parseLabelHelper(label, node, defaultNode)) {
        return nullptr;
    }
    label->setText(text);

    top->addToCurrentPage(label);
    return label;
}

GumpComponent* XmlLoader::parsePropertyLabel(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);
    UnicodeString link = StringConverter::fromUtf8(node.attribute("link").value());

    if (link.length() == 0) {
        LOG_WARN << "PropertyLabel without link" << std::endl;
        return nullptr;
    }

    components::PropertyLabel* label = new components::PropertyLabel(parent, link);
    label->set_geometry(bounds);
    if (!parseLabelHelper(label, node, defaultNode)) {
        return nullptr;
    }

    top->addToCurrentPage(label);
    return label;
}

GumpComponent* XmlLoader::parseSysLogLabel(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);

    components::SysLogLabel* label = new components::SysLogLabel(top);
    label->setMaxGeometry(bounds);
    if (!parseLabelHelper(label, node, defaultNode)) {
        return nullptr;
    }

    top->addToCurrentPage(label);
    return label;
}

GumpComponent* XmlLoader::parseClickLabel(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);
    UnicodeString text = getAttribute("text", node, defaultNode).value();

    unsigned int cliloc = getAttribute("cliloc", node, defaultNode).as_uint();
    if (cliloc) {
        UnicodeString args = getAttribute("args", node, defaultNode).value();
        args.findAndReplace("\\t", "\t");
        text = data::Manager::getClilocLoader()->get(cliloc, args);
    }

    unsigned int buttonId = node.attribute("buttonid").as_uint();
    unsigned int pageId = node.attribute("page").as_uint();
    UnicodeString action = StringConverter::fromUtf8(getAttribute("action", node, defaultNode).value());
    UnicodeString param = StringConverter::fromUtf8(getAttribute("param", node, defaultNode).value());
    UnicodeString param2 = StringConverter::fromUtf8(getAttribute("param2", node, defaultNode).value());
    UnicodeString param3 = StringConverter::fromUtf8(getAttribute("param3", node, defaultNode).value());
    UnicodeString param4 = StringConverter::fromUtf8(getAttribute("param4", node, defaultNode).value());
    UnicodeString param5 = StringConverter::fromUtf8(getAttribute("param5", node, defaultNode).value());

    components::ClickLabel* label = new components::ClickLabel(parent);
    label->set_geometry(bounds);
    if (!parseLabelHelper(label, node, defaultNode)) {
        return nullptr;
    }

    if (action.length() > 0) {
        label->setLocalButton(action);
        label->setParameter(param, 0);
        label->setParameter(param2, 1);
        label->setParameter(param3, 2);
        label->setParameter(param4, 3);
        label->setParameter(param5, 4);
    } else if (!node.attribute("buttonid").empty()) {
        label->setServerButton(buttonId);
    } else if (!node.attribute("page").empty()) {
        label->setPageButton(pageId);
    } else {
        LOG_WARN << "ClickLabel without action, id or page" << std::endl;
        return nullptr;
    }

    label->setText(text);

    top->addToCurrentPage(label);
    return label;
}

GumpComponent* XmlLoader::parseHtmlLabel(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);
    pugi::xml_text textNode = node.text();
    UnicodeString text(textNode.get());

    UnicodeString scrollbarTemplate = StringConverter::fromUtf8(getAttribute("scrollarea-template", node, defaultNode).value());
    UnicodeString backgroundTemplate = StringConverter::fromUtf8(getAttribute("background-template", node, defaultNode).value());
    bool useBackground = getAttribute("background", node, defaultNode).as_bool();
    bool useScrollbar = getAttribute("scrollbar", node, defaultNode).as_bool();

    unsigned int cliloc = getAttribute("cliloc", node, defaultNode).as_uint();
    if (cliloc) {
        UnicodeString args = getAttribute("args", node, defaultNode).value();
        args.findAndReplace("\\t", "\t");
        text = data::Manager::getClilocLoader()->get(cliloc, args);
    }

    GumpComponent* background = nullptr;
    if (useBackground) {
        pugi::xml_node dummy;
        pugi::xml_node bgNode = getTemplate(backgroundTemplate);
        if (!bgNode) {
            LOG_ERROR << "htmllabel background requested, but unable to find given template: " << backgroundTemplate << std::endl;
            return nullptr;
        }

        background = parseBackground(dummy, bgNode, parent, top);
        if (!background) {
            LOG_ERROR << "Unable to get htmllabel background from template " << backgroundTemplate << std::endl;
            return nullptr;
        }

        background->set_geometry(bounds);
        top->addToCurrentPage(background);
    }

    components::ScrollArea* scrollarea = nullptr;
    if (useScrollbar) {
        pugi::xml_node dummy;
        pugi::xml_node scrollNode = getTemplate(scrollbarTemplate);
        if (!scrollNode) {
            LOG_ERROR << "htmllabel scrollbar requested, but unable to find given template: " << scrollbarTemplate << std::endl;
            return nullptr;
        }

        scrollarea = dynamic_cast<components::ScrollArea*>(parseScrollArea(dummy, scrollNode, parent, top));
        if (!scrollarea) {
            LOG_ERROR << "Unable to get htmllabel scrollbar from template " << scrollbarTemplate << std::endl;
            return nullptr;
        }
        scrollarea->set_geometry(bounds);
        scrollarea->updateScrollbars(); // to set client area size correctly

        // adjust background size
        if (background) {
            CL_Rectf boundsWoScroll(bounds.left, bounds.top, CL_Sizef(
                    bounds.get_width() - scrollarea->getVerticalScrollBar()->get_width(),
                    bounds.get_height() - scrollarea->getHorizontalScrollBar()->get_height()
            ));
            // background does not span the scrollbars
            background->set_geometry(boundsWoScroll);
        }
    }

    components::Label* label;
    if (scrollarea) {
        label = new components::Label(scrollarea->getClientArea());
        CL_Rectf geom(0, 0, CL_Sizef(scrollarea->getClientArea()->get_width(), 0)); // auto height
        label->set_geometry(geom);
    } else {
        label = new components::Label(parent);
        label->set_geometry(bounds);
    }

    if (!parseLabelHelper(label, node, defaultNode)) {
        return nullptr;
    }

    label->setHtmlText(text);

    if (scrollarea) {
        scrollarea->updateScrollbars();
        scrollarea->setupResizeHandler();

        top->addToCurrentPage(scrollarea);
    } else {
        top->addToCurrentPage(label);
    }

    return label;
}

GumpComponent* XmlLoader::parseLineEdit(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);
    UnicodeString text = StringConverter::fromUtf8(getAttribute("text", node, defaultNode).value());
    int numeric = getAttribute("numeric", node, defaultNode).as_int();
    int password = getAttribute("password", node, defaultNode).as_int();
    unsigned int maxlength = getAttribute("maxlength", node, defaultNode).as_uint();
    unsigned int entryId = getAttribute("entryid", node, defaultNode).as_uint();
    UnicodeString action = StringConverter::fromUtf8(getAttribute("action", node, defaultNode).value());
    UnicodeString fontName = StringConverter::fromUtf8(getAttribute("font", node, defaultNode).value());
    unsigned int fontHeight = getAttribute("font-height", node, defaultNode).as_uint();
    std::string rgba = getAttribute("color", node, defaultNode).value();
    unsigned int hue = getAttribute("hue", node, defaultNode).as_uint();
    CL_Colorf color(rgba);

    components::LineEdit* edit = new components::LineEdit(parent);
    parseId(node, edit);
    edit->setFont(fontName, fontHeight);
    edit->set_geometry(bounds);

    if (password) {
        edit->setPasswordMode(true);
    }
    if (numeric) {
        edit->setNumericMode(true);
    }
    if (maxlength) {
        edit->setMaxLength(maxlength);
    }

    if (action.length() > 0) {
        edit->setAction(action);
    }

    // if the node has its own color or hue property, don't use the template values
    if (node.attribute("color")) {
        edit->setColor(color);
    } else if (node.attribute("hue")) {
        edit->setHue(hue);
    } else if (rgba.length() > 0) {
        edit->setColor(color);
    } else if (getAttribute("hue", node, defaultNode)) {
        edit->setHue(hue);
    }

    edit->setText(text);
    edit->setEntryId(entryId);

    top->addToCurrentPage(edit);
    return edit;
}

GumpComponent* XmlLoader::parseScrollArea(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);

    components::ScrollArea* area = new components::ScrollArea(parent);
    parseId(node, area);
    area->set_geometry(bounds);

    parseScrollBar(area->getVerticalScrollBar(), area, true, node.child("vscroll"), defaultNode.child("vscroll"));
    parseScrollBar(area->getHorizontalScrollBar(), area, false, node.child("hscroll"), defaultNode.child("hscroll"));

    unsigned int marginLeft = getAttribute("marginleft", node, defaultNode).as_uint();
    unsigned int marginTop = getAttribute("margintop", node, defaultNode).as_uint();
    unsigned int marginRight = getAttribute("marginright", node, defaultNode).as_uint();
    unsigned int marginBottom = getAttribute("marginbottom", node, defaultNode).as_uint();

    top->addToCurrentPage(area);

    pugi::xml_node contentNode = node.child("content");
    if (contentNode) {
        parseChildren(contentNode, area->getClientArea(), top);
    } else {
        //LOG_ERROR << "No content node in scrollarea" << std::endl;
    }

    area->setMargins(marginLeft, marginTop, marginRight, marginBottom);
    area->updateScrollbars();
    area->setupResizeHandler();

    return area;
}

GumpComponent* XmlLoader::parseAlphaRegion(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);
    float alpha = getAttribute("alpha", node, defaultNode).as_float();

    components::AlphaRegion* areg = new components::AlphaRegion(parent);
    parseId(node, areg);
    areg->set_geometry(bounds);
    areg->setAlpha(alpha);

    top->addToCurrentPage(areg);
    return areg;
}

GumpComponent* XmlLoader::parseWorldView(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);
    ui::components::WorldView* worldView = new ui::components::WorldView(parent, bounds);
    parseId(node, worldView);
    top->addToCurrentPage(worldView);

    return worldView;
}

GumpComponent* XmlLoader::parsePaperdoll(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);
    ui::components::PaperdollView* pdView = new ui::components::PaperdollView(parent, bounds);
    parseId(node, pdView);
    top->addToCurrentPage(pdView);

    return pdView;
}

GumpComponent* XmlLoader::parseContainer(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);
    ui::components::ContainerView* contView = new ui::components::ContainerView(parent, bounds);
    parseId(node, contView);
    top->addToCurrentPage(contView);

    return contView;
}

GumpComponent* XmlLoader::parseWarModeButton(pugi::xml_node& node, pugi::xml_node& defaultNode, CL_GUIComponent* parent, GumpMenu* top) {
    CL_Rect bounds = getBoundsFromNode(node, defaultNode);

    components::WarModeButton* button = new components::WarModeButton(parent);
    parseId(node, button);

    pugi::xml_node normalNode = node.child("normal");
    pugi::xml_node mouseOverNode = node.child("mouseover");
    pugi::xml_node mouseDownNode = node.child("mousedown");

    pugi::xml_node defaultNormalNode = defaultNode.child("normal");
    pugi::xml_node defaultMouseOverNode = defaultNode.child("mouseover");
    pugi::xml_node defaultMouseDownNode = defaultNode.child("mousedown");

    if (normalNode || defaultNormalNode) {
        parseMultiTextureImage(normalNode, defaultNormalNode, button, components::WarModeButton::TEX_INDEX_UP);
    } else {
        LOG_ERROR << "Normal image for warmode button not defined" << std::endl;
        return nullptr;
    }

    if (mouseOverNode || defaultMouseOverNode) {
        parseMultiTextureImage(mouseOverNode, defaultMouseOverNode, button, components::WarModeButton::TEX_INDEX_MOUSEOVER);
    }
    if (mouseDownNode || defaultMouseDownNode) {
        parseMultiTextureImage(mouseDownNode, defaultMouseDownNode, button, components::WarModeButton::TEX_INDEX_DOWN);
    }


    pugi::xml_node warmodeNormalNode = node.child("warmode-normal");
    pugi::xml_node warmodeMouseOverNode = node.child("warmode-mouseover");
    pugi::xml_node warmodeMouseDownNode = node.child("warmode-mousedown");

    pugi::xml_node warmodeDefaultNormalNode = defaultNode.child("warmode-normal");
    pugi::xml_node warmodeDefaultMouseOverNode = defaultNode.child("warmode-mouseover");
    pugi::xml_node warmodeDefaultMouseDownNode = defaultNode.child("warmode-mousedown");

    if (warmodeNormalNode || warmodeDefaultNormalNode) {
        parseMultiTextureImage(warmodeNormalNode, warmodeDefaultNormalNode, button, components::WarModeButton::WARMODE_TEX_INDEX_UP);
    }
    if (warmodeMouseOverNode || warmodeDefaultMouseOverNode) {
        parseMultiTextureImage(warmodeMouseOverNode, warmodeDefaultMouseOverNode, button, components::WarModeButton::WARMODE_TEX_INDEX_MOUSEOVER);
    }
    if (warmodeMouseDownNode || warmodeDefaultMouseDownNode) {
        parseMultiTextureImage(warmodeMouseDownNode, warmodeDefaultMouseDownNode, button, components::WarModeButton::WARMODE_TEX_INDEX_DOWN);
    }

    if (bounds.get_width() == 0 || bounds.get_height() == 0) {
        button->setAutoResize(true);
        bounds.set_width(1);
        bounds.set_height(1);
    }

    button->updateTexture();

    button->set_geometry(bounds);

    top->addToCurrentPage(button);
    return button;
}



bool XmlLoader::parsePage(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    unsigned int number = node.attribute("number").as_uint();

    if (top->getActivePageId() != 0) {
        // check that we add pages only at the top level hierarchy
        // adding a page inside another page
        LOG_ERROR << "Adding page " << top->getActivePageId() << " inside of page " << number << std::endl;
        return false;
    }

    top->addPage(number);
    parseChildren(node, parent, top);
    top->activatePage(0);

    return true;
}

bool XmlLoader::parseRepeat(pugi::xml_node& node, CL_GUIComponent* parent, GumpMenu* top) {
    UnicodeString name(node.attribute("name").value());

    if (repeatContexts_.count(name) == 0) {
        LOG_ERROR << "Trying to access unknown repeat context " << name << std::endl;
        return false;
    }

    const RepeatContext& context = repeatContexts_[name];

    int xIncrease = node.attribute("xincrease").as_int();
    int yIncrease = node.attribute("yincrease").as_int();
    unsigned int xLimit = node.attribute("xlimit").as_uint();
    unsigned int yLimit = node.attribute("ylimit").as_uint();

    for (unsigned int index = 0; index < context.repeatCount_; ++index) {
        insertRepeatNodes(node.begin(), node.end(), node.parent(), context, index,
                xIncrease, yIncrease, xLimit, yLimit);
    }

    // TODO: what to do here?
    return true;
}

void XmlLoader::insertRepeatNodes(pugi::xml_node::iterator begin, pugi::xml_node::iterator end, pugi::xml_node dst,
            const RepeatContext& context, unsigned int index,
            int xIncrease, int yIncrease, unsigned int xLimit, unsigned int yLimit) {
    for (pugi::xml_node::iterator iter = begin; iter != end; ++iter) {
        pugi::xml_node newNode = dst.insert_copy_after(*iter, dst.last_child());

        replaceRepeatKeywords(newNode, context, index,
                xIncrease, yIncrease, xLimit, yLimit);

        checkRepeatIf(newNode, index, xLimit, yLimit);
    }
}

void XmlLoader::checkRepeatIf(pugi::xml_node& node, unsigned int index, unsigned int xLimit, unsigned int yLimit) {
    unsigned int xIndex = xLimit > 0 ? index % xLimit : index;
    unsigned int yIndex = yLimit > 0 ? index % yLimit : index;

    bool removeNode = false;
    if (strcmp(node.name(), "repeatif") == 0) {
        pugi::xml_node::attribute_iterator attrIter = node.attributes_begin();
        pugi::xml_node::attribute_iterator attrEnd = node.attributes_end();

        for (; attrIter != attrEnd; ++attrIter) {
            if (strcmp(attrIter->name(), "xindex") == 0) {
                if (attrIter->as_uint() != xIndex) {
                    removeNode = true;
                    break;
                }
            } else if (strcmp(attrIter->name(), "yindex") == 0) {
                if (attrIter->as_uint() != yIndex) {
                    removeNode = true;
                    break;
                }
            } else if (strcmp(attrIter->name(), "test") == 0) {
                if (attrIter->as_uint() != 1) {
                    removeNode = true;
                    break;
                }
            }
        }

        if (!removeNode) {
            // move children to parent node
            pugi::xml_node childIter = node.last_child();
            while (childIter) {
                pugi::xml_node newChild = node.parent().insert_copy_after(childIter, node);
                checkRepeatIf(newChild, index, xLimit, yLimit);
                childIter = childIter.previous_sibling();
            }
        }

        // remove repeatif node
        node.parent().remove_child(node);
    } else {
        pugi::xml_node childIter = node.first_child();
        while (childIter) {
            checkRepeatIf(childIter, index, xLimit, yLimit);
            childIter = childIter.next_sibling();
        }
    }
}

void XmlLoader::replaceRepeatKeywords(pugi::xml_node& node, const RepeatContext& context, unsigned int index,
            int xIncrease, int yIncrease, unsigned int xLimit, unsigned int yLimit) {

    static std::string attrNameX("x");
    static std::string attrNameY("y");

    pugi::xml_node::attribute_iterator attrIter = node.attributes_begin();
    pugi::xml_node::attribute_iterator attrEnd = node.attributes_end();

    for (; attrIter != attrEnd; ++attrIter) {
        bool contextHit = false;

        std::map<UnicodeString, std::vector<UnicodeString> >::const_iterator contextIter = context.keywordReplacements_.begin();
        std::map<UnicodeString, std::vector<UnicodeString> >::const_iterator contextEnd = context.keywordReplacements_.end();

        for (; contextIter != contextEnd; ++contextIter) {
            if (contextIter->first == attrIter->value()) {
                contextHit = true;

                attrIter->set_value(StringConverter::toUtf8String(contextIter->second[index]).c_str());
                break;
            }
        }

        if (!contextHit) {
            // increase and y values, if found
            if (attrNameX == attrIter->name()) {
                int baseX = attrIter->as_int();
                unsigned int xIndex = xLimit > 0 ? index % xLimit : index;
                int curXIncrease = xIncrease * xIndex;
                int curX = baseX + curXIncrease;
                attrIter->set_value(curX);
            } else if (attrNameY == attrIter->name()) {
                int baseY = attrIter->as_int();
                unsigned int yIndex = yLimit > 0 ? index % yLimit : index;
                int curYIncrease = yIncrease * yIndex;
                int curY = baseY + curYIncrease;
                attrIter->set_value(curY);
            }
        }
    }

    // also apply keyword replacements to child nodes
    pugi::xml_node childIter = node.first_child();
    while (childIter) {
        replaceRepeatKeywords(childIter, context, index,
                xIncrease, yIncrease, xLimit, yLimit);
        childIter = childIter.next_sibling();
    }
}


GumpComponent* XmlLoader::getServerGumpComponent(const UnicodeString& templateName, GumpMenu* menu) {
    XmlLoader* sing = getSingleton();

    pugi::xml_node dummy;
    pugi::xml_node templateNode = sing->getTemplate(templateName);

    if (!templateNode) {
        LOG_ERROR << "Could not find template " << templateName << " required for server side gumps" << std::endl;
        return nullptr;
    }

    std::map<UnicodeString, XmlParseFunction>::iterator function = sing->functionTable_.find(templateNode.name());

    if (function != sing->functionTable_.end()) {
        return (function->second)(dummy, templateNode, menu, menu);
    } else {
        LOG_ERROR << "Unknown tag " << templateNode.name() << " in server side gump template " << templateName << std::endl;
        return nullptr;
    }
}

GumpComponent* XmlLoader::getServerGumpHtmlLabel(const UnicodeString& templateName, GumpMenu* menu, const CL_Rectf& bounds, bool scrollbar, bool background) {
    // needed to create a new node on the fly
    static pugi::xml_document doc;
    static pugi::xml_node htmlLabelNode;

    if (!htmlLabelNode) {
        htmlLabelNode = doc.append_child("htmllabel");
        htmlLabelNode.append_attribute("scrollbar").set_value(scrollbar);
        htmlLabelNode.append_attribute("background").set_value(background);
        htmlLabelNode.append_attribute("x").set_value(bounds.left);
        htmlLabelNode.append_attribute("y").set_value(bounds.top);
        htmlLabelNode.append_attribute("width").set_value(bounds.get_width());
        htmlLabelNode.append_attribute("height").set_value(bounds.get_height());
    } else {
        htmlLabelNode.attribute("scrollbar").set_value(scrollbar);
        htmlLabelNode.attribute("background").set_value(background);
        htmlLabelNode.attribute("x").set_value(bounds.left);
        htmlLabelNode.attribute("y").set_value(bounds.top);
        htmlLabelNode.attribute("width").set_value(bounds.get_width());
        htmlLabelNode.attribute("height").set_value(bounds.get_height());
    }


    XmlLoader* sing = getSingleton();

    pugi::xml_node templateNode = sing->getTemplate(templateName);

    if (!templateNode) {
        LOG_ERROR << "Could not find template " << templateName << " required for server side gumps" << std::endl;
        return nullptr;
    }

    return sing->parseHtmlLabel(htmlLabelNode, templateNode, menu, menu);
}

}
}
