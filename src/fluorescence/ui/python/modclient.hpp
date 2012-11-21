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


#ifndef FLUO_UI_PYTHON_MODCLIENT_HPP
#define FLUO_UI_PYTHON_MODCLIENT_HPP

#include <boost/python.hpp>

#include "pyclient.hpp"

namespace fluo {
namespace ui {
namespace python {

namespace bpy = boost::python;

BOOST_PYTHON_MODULE(client) {
    bpy::def("setShard", &PyClient::setShard);
    bpy::def("createShard", &PyClient::createShard);
    bpy::def("connect", &PyClient::connect);
    bpy::def("disconnect", &PyClient::disconnect);
    bpy::def("selectServer", &PyClient::selectServer);
    bpy::def("selectCharacter", &PyClient::selectCharacter);
    bpy::def("getConfig", &PyClient::getConfig);
    bpy::def("shutdown", &PyClient::shutdown);
    bpy::def("messagebox", &PyClient::messagebox);
    bpy::def("openGump", &PyClient::openGump);
    bpy::def("handleTextInput", &PyClient::handleTextInput);
    bpy::def("sendHelpRequest", &PyClient::sendHelpRequest);
    bpy::def("useSkill", &PyClient::useSkill);

    // functions sending a packet to the server in response to a special gump
    bpy::def("objectPickerResponse", &PyClient::objectPickerResponse);
}

}
}
}

#endif

