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

#include "scriptloader.hpp"

#include <misc/log.hpp>
#include <ui/manager.hpp>
#include <ui/gumpmenu.hpp>

#include "pystring.hpp"
#include "modclient.hpp"
#include "moddata.hpp"
#include "modui.hpp"
#include "modworld.hpp"

namespace fluo {
namespace ui {
namespace python {

namespace bpy = boost::python;

ScriptLoader::ScriptLoader() : initialized_(false), shouldReload_(false) {
}

ScriptLoader::~ScriptLoader() {
    unload();
}

void ScriptLoader::init() {
    if (initialized_) {
        return;
    }

    // create object to register converters with boost::python
    static PythonUnicodeStringConverter strconv;

    PyImport_AppendInittab("ui", &initui);
    PyImport_AppendInittab("data", &initdata);
    PyImport_AppendInittab("client", &initclient);
    PyImport_AppendInittab("world", &initworld);
    Py_Initialize();

    // extend python path to the gumps directory
    PyObject* sysPath = PySys_GetObject((char*)"path");
    PyList_Insert(sysPath, 0, PyString_FromString("gumps/"));

    initialized_ = true;
}

void ScriptLoader::setThemePath(const boost::filesystem::path& themePath) {
    // extend python path to the theme directory
    PyObject* sysPath = PySys_GetObject((char*)"path");
    PyList_Insert(sysPath, 0, PyString_FromString(themePath.string().c_str()));
}

void ScriptLoader::setShardConfig(Config& config) {
    // there might be some overrides in the shards gump directory, so we need to reload
    // python. otherwise, things might be "stuck" in the bytecode cache.
    reload();

    // extend python path to the shard gumps directory
    PyObject* sysPath = PySys_GetObject((char*)"path");
    boost::filesystem::path path = config.getShardPath() / "gumps/";
    PyList_Insert(sysPath, 0, PyString_FromString(path.string().c_str()));

    setThemePath(ui::Manager::getSingleton()->getThemePath());
}

void ScriptLoader::unload() {
    if (!initialized_) {
        return;
    }

    pythonModules_.clear();

    /*
     * This is a bit tricky. As boost::python cannot handle repeated calls to Py_Finalize and Py_Initialize,
     * we have to manually unload all python modules inside the client directory.
     * To do this, iterate over sys.modules and del all modules whose path starts with the current working directory
     */
    try {
        bpy::object main = bpy::import("__main__");
        bpy::object mainDict = main.attr("__dict__");
        bpy::exec(  "import sys,os\n"
                    "cwd = os.getcwd()\n"
                    "for name, mod in sys.modules.items():\n"
                    "    if hasattr(mod, \"__file__\") and os.path.abspath(mod.__file__).startswith(cwd):\n"
                    "        del sys.modules[name]\n", mainDict, mainDict);
    } catch (bpy::error_already_set const&) {
        logError();
    }
}

void ScriptLoader::reload() {
    unload();
    init();
}

void ScriptLoader::openGump(const UnicodeString& name) {
    bpy::dict args;
    openGump(name, args);
}

bpy::object ScriptLoader::loadModule(const UnicodeString& name) {
    bpy::object module;
    try {
        std::map<UnicodeString, bpy::object>::iterator iter = pythonModules_.find(name);
        if (iter == pythonModules_.end()) {
            module = bpy::import(StringConverter::toUtf8String(name).c_str());
            pythonModules_[name] = module;
        } else {
            module = iter->second;
        }
    } catch (bpy::error_already_set const&) {
        logError();
    }

    return module;
}

void ScriptLoader::openGump(const UnicodeString& name, bpy::dict& args) {
    bpy::object module = loadModule(name);
    if (!module) {
        LOG_ERROR << "Python gump " << name << " not found" << std::endl;
        return;
    }

    try {
        if (module.attr("create")) {
            module.attr("create")(args);
        } else {
            LOG_ERROR << "No function create in gump module " << name << std::endl;
        }

    } catch (bpy::error_already_set const&) {
        logError();
    }
}

void ScriptLoader::logError() const {
    LOG_ERROR << "Python error: " << std::endl;

#ifdef DEBUG
    PyErr_Print();
#else

    PyObject *errType = NULL, *errValue = NULL, *errTraceback = NULL;
    PyErr_Fetch(&errType, &errValue, &errTraceback);

    char* errValueStr = PyString_AsString(errValue);
    char* errTypeStr = PyString_AsString(PyObject_Str(errType));
    LOG_ERROR << errTypeStr << ": " << errValueStr <<std::endl;
#endif
}

void ScriptLoader::requestReload() {
    // reload is delayed to avoid interfering with python code executed at the very moment
    shouldReload_ = true;
}

void ScriptLoader::saveGumps(std::list<std::pair<UnicodeString, bpy::object> >& savedObjects) {
    // iterate through all gumps and call their save function, if they are python gumps and have one
    // store gumps and their module name to load later

    ui::Manager* uiMan = ui::Manager::getSingleton();

    std::list<GumpMenu*>::iterator saveIter = uiMan->gumpsBegin();
    std::list<GumpMenu*>::iterator saveEnd = uiMan->gumpsEnd();
    for (; saveIter != saveEnd; ++saveIter) {
        GumpMenu* curMenu = *saveIter;
        const UnicodeString& name = curMenu->getName();
        if (name.length() <= 0) {
            // not a python module
            continue;
        }

        std::map<UnicodeString, bpy::object>::iterator gumpModule = pythonModules_.find(curMenu->getName());
        if (gumpModule == pythonModules_.end()) {
            LOG_WARN << "Unable to find python module for gump " << name << ". Will not be saved." << std::endl;
        } else {
            // call the gump's save function
            try {
                if (gumpModule->second.attr("save")) {
                    bpy::object savedDict = gumpModule->second.attr("save")(bpy::ptr(curMenu));
                    savedObjects.push_back(std::make_pair(name, savedDict));
                } else {
                    LOG_WARN << "No function save in gump module " << name << std::endl;
                }

            } catch (bpy::error_already_set const&) {
                logError();
            }
        }
    }
}

void ScriptLoader::loadGumps(std::list<std::pair<UnicodeString, bpy::object> >& savedObjects) {
    // load gumps from saved objects list
    std::list<std::pair<UnicodeString, bpy::object> >::iterator loadIter = savedObjects.begin();
    std::list<std::pair<UnicodeString, bpy::object> >::iterator loadEnd = savedObjects.end();
    for (; loadIter != loadEnd; ++loadIter) {
        bpy::object module = loadModule(loadIter->first);
        try {
            if (module.attr("load")) {
                module.attr("load")(loadIter->second);
            } else {
                LOG_ERROR << "No function load in gump module " << loadIter->first << std::endl;
            }
        } catch (bpy::error_already_set const&) {
            logError();
        }
    }
}

bool ScriptLoader::step(unsigned int elapsedMillis) {
    bool ret = shouldReload_;
    if (shouldReload_) {
        LOG_DEBUG << "Reloading python..." << std::endl;


        boost::filesystem::path p("gumpsave.py");

        ui::Manager* uiMan = ui::Manager::getSingleton();

        //std::list<std::pair<UnicodeString, bpy::object> > savedObjects;
        //saveGumps(savedObjects);
        saveToFile(p);

        std::list<GumpMenu*>::iterator saveIter = uiMan->gumpsBegin();
        std::list<GumpMenu*>::iterator saveEnd = uiMan->gumpsEnd();
        for (; saveIter != saveEnd; ++saveIter) {
            GumpMenu* curMenu = *saveIter;
            if (curMenu->getName().length() > 0) {
                uiMan->closeGumpMenu(curMenu);
            }
        }

        // close all python gumps now
        uiMan->processGumpCloseList();

        reload();

        //loadGumps(savedObjects);
        loadFromFile(p);

        shouldReload_ = false;
        LOG_DEBUG << "Python reload complete" << std::endl;
    }

    return ret;
}

void ScriptLoader::saveToFile(const boost::filesystem::path& path) {
    std::list<std::pair<UnicodeString, bpy::object> > savedObjects;
    saveGumps(savedObjects);

    // the gumps are stored as python literals
    // to make printing this list easy (and foolproof), the saveObjects list is converted to a python list
    // and then printed with native python code, just using print
    bpy::list saveList;

    std::list<std::pair<UnicodeString, bpy::object> >::iterator iter = savedObjects.begin();
    std::list<std::pair<UnicodeString, bpy::object> >::iterator end = savedObjects.end();
    for (; iter != end; ++iter) {
        saveList.append(bpy::make_tuple(iter->first, iter->second));
    }

    try {
        bpy::object main = bpy::import("__main__");
        bpy::object mainDict = main.attr("__dict__");
        mainDict["path"] = bpy::str(path.string());
        mainDict["saveList"] = saveList;
        bpy::exec(  "with open(path, 'w')as f:\n"
                    "    print >>f, saveList\n",
                    mainDict, mainDict);
    } catch (bpy::error_already_set const&) {
        logError();
    }
}

void ScriptLoader::loadFromFile(const boost::filesystem::path& path) {
    if (!boost::filesystem::exists(path)) {
        LOG_INFO << "No saved UO desktop found" << std::endl;
        return;
    }

    // load the python list from the save file, again using native python code
    // then, convert it to the c++ structure and call loadGumps()
    bpy::list saveList;
    std::list<std::pair<UnicodeString, bpy::object> > savedObjects;

    try {
        // try to extract the previously stored list
        bpy::object main = bpy::import("__main__");
        bpy::object mainDict = main.attr("__dict__");
        mainDict["path"] = bpy::str(path.string());
        bpy::exec(  "import ast\n"
                    "with open(path, 'r')as f:\n"
                    "    saveList = ast.literal_eval(f.read())\n",
                    mainDict, mainDict);

        saveList = bpy::extract<bpy::list>(mainDict["saveList"]);

        bpy::ssize_t n = bpy::len(saveList);
        for(bpy::ssize_t i = 0; i < n; ++i) {
            bpy::tuple elem = bpy::extract<bpy::tuple>(saveList[i]);
            UnicodeString name = bpy::extract<UnicodeString>(elem[0]);
            bpy::object attribs = elem[1];
            savedObjects.push_back(std::make_pair(name, attribs));
        }

        loadGumps(savedObjects);
    } catch (bpy::error_already_set const&) {
        logError();
    }
}

}
}
}

