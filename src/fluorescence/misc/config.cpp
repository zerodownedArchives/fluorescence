
#include "config.hpp"

#include <iostream>
#include <fstream>

#include <unicode/regex.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp>

#include "log.hpp"


namespace fluo {

Config::Config() {
    // file paths
    variablesMap_["/fluo/files/mul-directory@path"].setPath("./", true);
    variablesMap_["/fluo/files/art@filename-idx"].setPath("artidx.mul", true);
    variablesMap_["/fluo/files/art@filename-mul"].setPath("art.mul", true);
    variablesMap_["/fluo/files/gumpart@filename-idx"].setPath("gumpidx.mul", true);
    variablesMap_["/fluo/files/gumpart@filename-mul"].setPath("gumpart.mul", true);
    variablesMap_["/fluo/files/hues@filename"].setPath("hues.mul", true);
    variablesMap_["/fluo/files/tiledata@filename"].setPath("tiledata.mul", true);
    variablesMap_["/fluo/files/texmaps@filename-idx"].setPath("texidx.mul", true);
    variablesMap_["/fluo/files/texmaps@filename-mul"].setPath("texmaps.mul", true);
    variablesMap_["/fluo/files/animdata@filename"].setPath("animdata.mul", true);


    // maps
    variablesMap_["/fluo/files/map0@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map0@filename"].setPath("map0.mul", true);
    variablesMap_["/fluo/files/map0/difs@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map0/difs@filename-offsets"].setPath("mapdifl0.mul", true);
    variablesMap_["/fluo/files/map0/difs@filename-mul"].setPath("mapdif0.mul", true);
    variablesMap_["/fluo/files/map0@width"].setInt(768, true);
    variablesMap_["/fluo/files/map0@height"].setInt(512, true);

    variablesMap_["/fluo/files/map0/statics@filename-idx"].setPath("staidx0.mul", true);
    variablesMap_["/fluo/files/map0/statics@filename-mul"].setPath("statics0.mul", true);
    variablesMap_["/fluo/files/map0/statics/difs@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map0/statics/difs@filename-offsets"].setPath("stadifl0.mul", true);
    variablesMap_["/fluo/files/map0/statics/difs@filename-idx"].setPath("stadifi0.mul", true);
    variablesMap_["/fluo/files/map0/statics/difs@filename-mul"].setPath("stadif0.mul", true);


    variablesMap_["/fluo/files/map1@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map1@filename"].setPath("map0.mul", true);
    variablesMap_["/fluo/files/map1/difs@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map1/difs@filename-offsets"].setPath("mapdifl1.mul", true);
    variablesMap_["/fluo/files/map1/difs@filename-mul"].setPath("mapdif1.mul", true);
    variablesMap_["/fluo/files/map1@width"].setInt(768, true);
    variablesMap_["/fluo/files/map1@height"].setInt(512, true);

    variablesMap_["/fluo/files/map1/statics@filename-idx"].setPath("staidx0.mul", true);
    variablesMap_["/fluo/files/map1/statics@filename-mul"].setPath("statics0.mul", true);
    variablesMap_["/fluo/files/map1/statics/difs@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map1/statics/difs@filename-offsets"].setPath("stadifl1.mul", true);
    variablesMap_["/fluo/files/map1/statics/difs@filename-idx"].setPath("stadifi1.mul", true);
    variablesMap_["/fluo/files/map1/statics/difs@filename-mul"].setPath("stadif1.mul", true);


    variablesMap_["/fluo/files/map2@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map2@filename"].setPath("map2.mul", true);
    variablesMap_["/fluo/files/map2/difs@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map2/difs@filename-offsets"].setPath("mapdifl2.mul", true);
    variablesMap_["/fluo/files/map2/difs@filename-mul"].setPath("mapdif2.mul", true);
    variablesMap_["/fluo/files/map2@width"].setInt(288, true);
    variablesMap_["/fluo/files/map2@height"].setInt(200, true);

    variablesMap_["/fluo/files/map2/statics@filename-idx"].setPath("staidx2.mul", true);
    variablesMap_["/fluo/files/map2/statics@filename-mul"].setPath("statics2.mul", true);
    variablesMap_["/fluo/files/map2/statics/difs@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map2/statics/difs@filename-offsets"].setPath("stadifl2.mul", true);
    variablesMap_["/fluo/files/map2/statics/difs@filename-idx"].setPath("stadifi2.mul", true);
    variablesMap_["/fluo/files/map2/statics/difs@filename-mul"].setPath("stadif2.mul", true);


    variablesMap_["/fluo/files/map3@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map3@filename"].setPath("map3.mul", true);
    variablesMap_["/fluo/files/map3/difs@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map3/difs@filename-offsets"].setPath("mapdifl3.mul", true);
    variablesMap_["/fluo/files/map3/difs@filename-mul"].setPath("mapdif3.mul", true);
    variablesMap_["/fluo/files/map3@width"].setInt(320, true);
    variablesMap_["/fluo/files/map3@height"].setInt(256, true);

    variablesMap_["/fluo/files/map3/statics@filename-idx"].setPath("staidx3.mul", true);
    variablesMap_["/fluo/files/map3/statics@filename-mul"].setPath("statics3.mul", true);
    variablesMap_["/fluo/files/map3/statics/difs@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map3/statics/difs@filename-offsets"].setPath("stadifl3.mul", true);
    variablesMap_["/fluo/files/map3/statics/difs@filename-idx"].setPath("stadifi3.mul", true);
    variablesMap_["/fluo/files/map3/statics/difs@filename-mul"].setPath("stadif3.mul", true);


    variablesMap_["/fluo/files/map4@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map4@filename"].setPath("map4.mul", true);
    variablesMap_["/fluo/files/map4/difs@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map4/difs@filename-offsets"].setPath("mapdifl4.mul", true);
    variablesMap_["/fluo/files/map4/difs@filename-mul"].setPath("mapdif4.mul", true);
    variablesMap_["/fluo/files/map4@width"].setInt(181, true);
    variablesMap_["/fluo/files/map4@height"].setInt(181, true);

    variablesMap_["/fluo/files/map4/statics@filename-idx"].setPath("staidx4.mul", true);
    variablesMap_["/fluo/files/map4/statics@filename-mul"].setPath("statics4.mul", true);
    variablesMap_["/fluo/files/map4/statics/difs@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/map4/statics/difs@filename-offsets"].setPath("stadifl4.mul", true);
    variablesMap_["/fluo/files/map4/statics/difs@filename-idx"].setPath("stadifi4.mul", true);
    variablesMap_["/fluo/files/map4/statics/difs@filename-mul"].setPath("stadif4.mul", true);


    // anims
    variablesMap_["/fluo/files/anim0@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/anim0@filename-idx"].setPath("anim.idx", true);
    variablesMap_["/fluo/files/anim0@filename-mul"].setPath("anim.mul", true);
    variablesMap_["/fluo/files/anim0@highdetail"].setInt(200, true);
    variablesMap_["/fluo/files/anim0@lowdetail"].setInt(200, true);

    variablesMap_["/fluo/files/anim1@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/anim1@filename-idx"].setPath("anim1.idx", true);
    variablesMap_["/fluo/files/anim1@filename-mul"].setPath("anim1.mul", true);
    variablesMap_["/fluo/files/anim1@highdetail"].setInt(200, true);
    variablesMap_["/fluo/files/anim1@lowdetail"].setInt(200, true);

    variablesMap_["/fluo/files/anim2@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/anim2@filename-idx"].setPath("anim2.idx", true);
    variablesMap_["/fluo/files/anim2@filename-mul"].setPath("anim2.mul", true);
    variablesMap_["/fluo/files/anim2@highdetail"].setInt(200, true);
    variablesMap_["/fluo/files/anim2@lowdetail"].setInt(200, true);

    variablesMap_["/fluo/files/anim3@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/anim3@filename-idx"].setPath("anim3.idx", true);
    variablesMap_["/fluo/files/anim3@filename-mul"].setPath("anim3.mul", true);
    variablesMap_["/fluo/files/anim3@highdetail"].setInt(200, true);
    variablesMap_["/fluo/files/anim3@lowdetail"].setInt(200, true);

    variablesMap_["/fluo/files/anim4@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/anim4@filename-idx"].setPath("anim4.idx", true);
    variablesMap_["/fluo/files/anim4@filename-mul"].setPath("anim4.mul", true);
    variablesMap_["/fluo/files/anim4@highdetail"].setInt(200, true);
    variablesMap_["/fluo/files/anim4@lowdetail"].setInt(200, true);

    variablesMap_["/fluo/files/anim5@enabled"].setBool(true, true);
    variablesMap_["/fluo/files/anim5@filename-idx"].setPath("anim5.idx", true);
    variablesMap_["/fluo/files/anim5@filename-mul"].setPath("anim5.mul", true);
    variablesMap_["/fluo/files/anim5@highdetail"].setInt(200, true);
    variablesMap_["/fluo/files/anim5@lowdetail"].setInt(200, true);


    variablesMap_["/fluo/files/mobtypes@filename"].setPath("mobtypes.txt", true);


    // ui stuff
    variablesMap_["/fluo/ui/theme@name"].setPath("default", true);
    variablesMap_["/fluo/ui/cursor@normal-artid-start"].setInt(0x206A, true);
    variablesMap_["/fluo/ui/cursor@warmode-artid-start"].setInt(0x2053, true);

    // layer priorities for animations
    variablesMap_["/fluo/ui/layer-priorities@north"].setString      ("21, 22,  3,  2,  1, 20, 11,  6,  7, 14, 15, 17,  5,  8,  9, 18, 13, 19, 10, 23, 24, 16, 12,  4", true);
    variablesMap_["/fluo/ui/layer-priorities@northeast"].setString  ("21, 23,  3,  2,  1, 20, 11,  6,  7, 14, 15, 17,  5,  8,  9, 18, 13, 19, 10, 22, 24, 16, 12,  4", true);
    variablesMap_["/fluo/ui/layer-priorities@east"].setString       ("21, 23,  3,  2,  1, 20, 11,  6,  7, 14, 15, 17,  5,  8,  9, 18, 13, 19, 10, 22, 24, 16, 12,  4", true);
    variablesMap_["/fluo/ui/layer-priorities@southeast"].setString  ("22, 23,  4,  3,  2, 21, 12,  7,  8, 15, 16, 18,  6,  9, 10, 19, 14, 20, 11,  1, 24, 17, 13,  5", true);
    variablesMap_["/fluo/ui/layer-priorities@south"].setString      ("21, 23,  3,  2,  1, 20, 11,  6,  7, 14, 15, 17,  5,  8,  9, 18, 13, 19, 10, 22, 24, 16, 12,  4", true);
    variablesMap_["/fluo/ui/layer-priorities@southwest"].setString  ("21, 23,  3,  2,  1, 20, 11,  6,  7, 14, 15, 17,  5,  8,  9, 18, 13, 19, 10, 22, 24, 16, 12,  4", true);
    variablesMap_["/fluo/ui/layer-priorities@west"].setString       ("21, 22,  3,  2,  1, 20, 11,  6,  7, 14, 15, 17,  5,  8,  9, 18, 13, 19, 10, 23, 24, 16, 12,  4", true);
    variablesMap_["/fluo/ui/layer-priorities@northwest"].setString  ("21, 22,  3,  2,  1, 20, 11,  6,  7, 14, 15, 17,  5,  8,  9, 18, 13, 19, 10, 23, 24, 16, 12,  4", true);


    // input stuff
    variablesMap_["/fluo/input/mouse@doubleclick-timeout-ms"].setInt(300, true);
    variablesMap_["/fluo/input/mouse@drag-start-distance"].setInt(40, true); // pixel difference to start dragging


    // shard stuff
    variablesMap_["/fluo/shard/account@save-password"].setBool(false, true);
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
    char* argv[argc];
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
