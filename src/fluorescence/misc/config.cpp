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



#include "config.hpp"

#include <iostream>
#include <fstream>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp>

#include "log.hpp"


namespace fluo {

Config::Config() {
    // file paths
    variablesMap_["/fluo/files/mul-directory@path"].setPath("./", true);
    variablesMap_["/fluo/files/cliloc@language"].setString("enu", true);

    // maps
    variablesMap_["/fluo/files/map0@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map0@difs-enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map0@width"].setInt(768, true);
    variablesMap_["/fluo/files/map0@height"].setInt(512, true);

    variablesMap_["/fluo/files/map1@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map1@difs-enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map1@width"].setInt(768, true);
    variablesMap_["/fluo/files/map1@height"].setInt(512, true);

    variablesMap_["/fluo/files/map2@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map2@difs-enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map2@width"].setInt(288, true);
    variablesMap_["/fluo/files/map2@height"].setInt(200, true);

    variablesMap_["/fluo/files/map3@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map3@difs-enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map3@width"].setInt(320, true);
    variablesMap_["/fluo/files/map3@height"].setInt(256, true);

    variablesMap_["/fluo/files/map4@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map4@difs-enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map4@width"].setInt(181, true);
    variablesMap_["/fluo/files/map4@height"].setInt(181, true);

    variablesMap_["/fluo/files/map5@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map5@difs-enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map5@width"].setInt(320, true);
    variablesMap_["/fluo/files/map5@height"].setInt(256, true);



    // anims
    variablesMap_["/fluo/files/anim@highdetail"].setInt(200, true);
    variablesMap_["/fluo/files/anim@lowdetail"].setInt(200, true);

    variablesMap_["/fluo/files/anim2@highdetail"].setInt(200, true);
    variablesMap_["/fluo/files/anim2@lowdetail"].setInt(200, true);

    variablesMap_["/fluo/files/anim3@highdetail"].setInt(420, true);
    variablesMap_["/fluo/files/anim3@lowdetail"].setInt(70, true);

    variablesMap_["/fluo/files/anim4@highdetail"].setInt(200, true);
    variablesMap_["/fluo/files/anim4@lowdetail"].setInt(200, true);

    variablesMap_["/fluo/files/anim5@highdetail"].setInt(200, true);
    variablesMap_["/fluo/files/anim5@lowdetail"].setInt(200, true);


    // ui stuff
    variablesMap_["/fluo/ui/theme@name"].setPath("default", true);
    variablesMap_["/fluo/ui/cursor@normal-artid-start"].setInt(0x206A, true);
    variablesMap_["/fluo/ui/cursor@warmode-artid-start"].setInt(0x2053, true);
    variablesMap_["/fluo/ui/gameview/font@name"].setString("Arial", true);
    variablesMap_["/fluo/ui/gameview/font@size"].setInt(12, true);


    // layer priorities for animations and paperdoll
    variablesMap_["/fluo/ui/layer-priorities@north"].setString      ("21, 22,  3,  2,  1, 20, 11,  6,  7, 14, 15, 17,  5,  8,  9, 18, 13, 19, 10, 23, 24, 16, 12,  4", true);
    variablesMap_["/fluo/ui/layer-priorities@northeast"].setString  ("21, 23,  3,  2,  1, 20, 11,  6,  7, 14, 15, 17,  5,  8,  9, 18, 13, 19, 10, 22, 24, 16, 12,  4", true);
    variablesMap_["/fluo/ui/layer-priorities@east"].setString       ("21, 23,  3,  2,  1, 20, 11,  6,  7, 14, 15, 17,  5,  8,  9, 18, 13, 19, 10, 22, 24, 16, 12,  4", true);
    variablesMap_["/fluo/ui/layer-priorities@southeast"].setString  ("22, 23,  4,  3,  2, 21, 12,  7,  8, 15, 16, 18,  6,  9, 10, 19, 14, 20, 11,  1, 24, 17, 13,  5", true);
    variablesMap_["/fluo/ui/layer-priorities@south"].setString      ("21, 23,  3,  2,  1, 20, 11,  6,  7, 14, 15, 17,  5,  8,  9, 18, 13, 19, 10, 22, 24, 16, 12,  4", true);
    variablesMap_["/fluo/ui/layer-priorities@southwest"].setString  ("21, 23,  3,  2,  1, 20, 11,  6,  7, 14, 15, 17,  5,  8,  9, 18, 13, 19, 10, 22, 24, 16, 12,  4", true);
    variablesMap_["/fluo/ui/layer-priorities@west"].setString       ("21, 22,  3,  2,  1, 20, 11,  6,  7, 14, 15, 17,  5,  8,  9, 18, 13, 19, 10, 23, 24, 16, 12,  4", true);
    variablesMap_["/fluo/ui/layer-priorities@northwest"].setString  ("21, 22,  3,  2,  1, 20, 11,  6,  7, 14, 15, 17,  5,  8,  9, 18, 13, 19, 10, 23, 24, 16, 12,  4", true);
    variablesMap_["/fluo/ui/layer-priorities@paperdoll"].setString  ("22, 23,  4,  3,  2, 21, 13,  9, 10, 15, 16, 17,  7, 11, 12, 19,  8, 20,  6,  1, 24, 17, 14,  5", true);


    // input stuff
    variablesMap_["/fluo/input/mouse@doubleclick-timeout-ms"].setInt(300, true);
    variablesMap_["/fluo/input/mouse@drag-start-distance"].setInt(40, true); // pixel difference to start dragging


    // shard stuff
    variablesMap_["/fluo/shard/account@name"].setString("", true);
    variablesMap_["/fluo/shard/account@password"].setString("", true);
    variablesMap_["/fluo/shard/account@save-password"].setBool(false, true);
    variablesMap_["/fluo/shard/address@host"].setString("", true);
    variablesMap_["/fluo/shard/address@port"].setInt(2593, true);
    variablesMap_["/fluo/shard/client@version-id"].setString("fluorescence 0.1", true);
}

bool Config::parseCommandLine(const std::vector<CL_String8>& args) {
    namespace po = boost::program_options;

    po::options_description consoleDesc;
    consoleDesc.add_options()
    ("help,h,?", "Receive this message")
    ("shard", po::value<std::string>(), "The shard you want to connect to. If empty, shard selection dialog is shown. Shard name can also be given without --shard prefix")
    ;

    // transform vector to default argc, argv
    int argc = args.size();
#ifdef WIN32
    char** argv = new char*[argc];
#else
    char* argv[argc];
#endif
    for (int i = 0; i < argc; ++i) {
        argv[i] = const_cast<char*>(args[i].c_str());
    }

    po::variables_map consoleOptions;

    try {
        // parse console
        po::positional_options_description p;
        p.add("shard", 1);
        po::store(po::command_line_parser(argc, argv).options(consoleDesc).positional(p).run(), consoleOptions);

        if (consoleOptions.count("help")) {
            std::cout << consoleDesc << std::endl;
            return false;
        }

        po::notify(consoleOptions);

        if (consoleOptions.count("shard") > 0) {
            UnicodeString shard = StringConverter::fromUtf8(consoleOptions["shard"].as<std::string>());
            variablesMap_["/fluo/shard@name"].setString(shard);
        }
    } catch (const std::exception& ex) {
        LOG_EMERGENCY << "Error parsing command line: " << ex.what() << std::endl;
        return false;
    }

#ifdef WIN32
    free(argv);
#endif

    return true;
}

bool Config::parseShardConfig() {
    boost::filesystem::path path = "shards" / variablesMap_["/fluo/shard@name"].asPath() / "config.xml";
    if (!boost::filesystem::exists(path)) {
        LOG_EMERGENCY << "Unable to open config file " << path << std::endl;
        return false;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.string().c_str());

    if (!result) {
        LOG_EMERGENCY << "Unable to open config file " << path << std::endl;
        return false;
    }

    pugi::xml_node rootNode = doc.child("fluo");

    if (!rootNode) {
        LOG_EMERGENCY << "Unable to find root node in config file" << std::endl;
        return false;
    }

    return parseNode(rootNode);
}

bool Config::parseNode(const pugi::xml_node& node) {
    UnicodeString currentPath = StringConverter::fromUtf8(node.path('/'));
    currentPath.append("@");

    pugi::xml_node::attribute_iterator attrIter = node.attributes_begin();
    pugi::xml_node::attribute_iterator attrEnd = node.attributes_end();

    for (; attrIter != attrEnd; ++attrIter) {
        UnicodeString configKey = currentPath;
        configKey.append(attrIter->name());
        if (!variablesMap_[configKey].parse(attrIter->value())) {
            return false;
        }
    }

    pugi::xml_node::iterator nodeIter = node.begin();
    pugi::xml_node::iterator nodeEnd = node.end();

    for (; nodeIter != nodeEnd; ++nodeIter) {
        if (nodeIter->type() == pugi::node_element) {
            if (!parseNode(*nodeIter)) {
                return false;
            }
        }
    }

    return true;
}

Variable& Config::operator[](const UnicodeString& name) {
    return get(name);
}

Variable& Config::get(const UnicodeString& name) {
    return variablesMap_[name];
}

bool Config::exists(const UnicodeString& name) const {
    return variablesMap_.count(name) > 0;
}

void Config::dumpMap() const {
    LOG_DEBUG << "\n\nFull config dump:" << std::endl;
    std::map<UnicodeString, Variable>::const_iterator iter = variablesMap_.begin();
    std::map<UnicodeString, Variable>::const_iterator end = variablesMap_.end();

    for (; iter != end; ++iter) {
        LOG_DEBUG << "Config value type=" << iter->second.valueType() << " isDefault=" << iter->second.isDefault() << "  " <<
                iter->first << " => " << iter->second.asString() << std::endl;
    }
}

bool Config::save(const boost::filesystem::path& path, bool includeDefaultValues) {
    pugi::xml_document rootNode;
    rootNode.set_name("fluo");

    std::map<UnicodeString, Variable>::const_iterator iter = variablesMap_.begin();
    std::map<UnicodeString, Variable>::const_iterator end = variablesMap_.end();

    for (; iter != end; ++iter) {
        if (includeDefaultValues || !iter->second.isDefault()) {
            unsigned int indexOfAt = iter->first.indexOf('@');
            UnicodeString nodePath(iter->first, 0, indexOfAt);

            std::string utf8NodePath = StringConverter::toUtf8String(nodePath);
            pugi::xml_node existingNode = rootNode.first_element_by_path(utf8NodePath.c_str(), '/');

            if (!existingNode) {
                existingNode = buildXmlNode(rootNode, nodePath);
            }

            UnicodeString attrName(iter->first, indexOfAt + 1);
            UnicodeString attrValue = iter->second.asString();

            pugi::xml_attribute attr = existingNode.append_attribute(StringConverter::toUtf8String(attrName).c_str());
            attr.set_value(StringConverter::toUtf8String(attrValue).c_str());
        }
    }

    // take care of some special nodes
    if (!variablesMap_["/fluo/shard/account@save-password"].asBool()) {
        pugi::xml_node accountNode = rootNode.first_element_by_path("/fluo/shard/account");
        if (accountNode) {
            accountNode.remove_attribute("password");
        }
    }

    pugi::xml_node shardNode = rootNode.first_element_by_path("/fluo/shard");
    if (shardNode) {
        shardNode.remove_attribute("name");
    }

    return rootNode.save_file(path.string().c_str(), "    ");
}

pugi::xml_node Config::buildXmlNode(pugi::xml_node& rootNode, const UnicodeString& path) const {
    unsigned int indexOfSlash = path.lastIndexOf('/');
    UnicodeString parentPath(path, 0, indexOfSlash);
    UnicodeString selfName(path, indexOfSlash + 1);

    //LOGARG_DEBUG(LOGTYPE_MAIN, "buildxmlnode parentPath: %s selfName: %s", StringConverter::toUtf8String(parentPath).c_str(), StringConverter::toUtf8String(selfName).c_str());

    pugi::xml_node parentNode;
    if (indexOfSlash == 0) {
        parentNode = rootNode;
    }

    if (!parentNode) {
        std::string utf8ParentPath = StringConverter::toUtf8String(parentPath);
        parentNode = rootNode.first_element_by_path(utf8ParentPath.c_str(), '/');
    }

    if (!parentNode) {
        parentNode = buildXmlNode(rootNode, parentPath);
    }

    pugi::xml_node ret = parentNode.append_child(pugi::node_element);
    ret.set_name(StringConverter::toUtf8String(selfName).c_str());

    return ret;
}

}
