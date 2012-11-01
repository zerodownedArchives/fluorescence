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

#include <ui/python/pystring.hpp>
#include <ui/python/modules.hpp>

namespace fluo {
namespace ui {
namespace python {

namespace bpy = boost::python;

ScriptLoader::ScriptLoader() : initialized_(false) {
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

    PyImport_AppendInittab("gumps", &initgumps);
    PyImport_AppendInittab("data", &initdata);
    PyImport_AppendInittab("client", &initclient);
    Py_Initialize();

    // extend python path to the gumps directory
    PyObject* sysPath = PySys_GetObject((char*)"path");
    PyList_Insert(sysPath, 0, PyString_FromString("data/gumps/"));

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
    boost::filesystem::path path = config.getShardPath() / "data/gumps/";
    PyList_Insert(sysPath, 0, PyString_FromString(path.string().c_str()));

    setThemePath(ui::Manager::getSingleton()->getThemePath());
}

void ScriptLoader::unload() {
    if (!initialized_) {
        return;
    }

    pythonModules_.clear();

    Py_Finalize();

    initialized_ = false;
}

void ScriptLoader::reload() {
    unload();
    init();
}

void ScriptLoader::openGump(const UnicodeString& name) {
    boost::python::dict args;
    openGump(name, args);
}

void ScriptLoader::openGump(const UnicodeString& name, boost::python::dict& args) {
    try {
        std::map<UnicodeString, bpy::object>::iterator iter = pythonModules_.find(name);
        bpy::object module;
        if (iter == pythonModules_.end()) {
            module = bpy::import(StringConverter::toUtf8String(name).c_str());
            pythonModules_[name] = module;
        } else {
            module = iter->second;
        }

        if (module.attr("create")) {
            module.attr("create")(args);
        } else {
            LOG_WARN << "No function create in gump module " << name << std::endl;
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

}
}
}

