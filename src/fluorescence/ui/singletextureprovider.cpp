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



#include "singletextureprovider.hpp"

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/gumpartloader.hpp>

namespace fluo {
namespace ui {

SingleTextureProvider::SingleTextureProvider(unsigned int textureSource, unsigned int id) {
    switch (textureSource) {
    case FROM_ART_MUL:
        texture_ = data::Manager::getArtLoader()->getItemTexture(id);
        break;
    case FROM_GUMPART_MUL:
        texture_ = data::Manager::getGumpArtLoader()->getTexture(id);
        break;
    }
}

boost::shared_ptr<ui::Texture> SingleTextureProvider::getTexture() const {
    return texture_;
}

bool SingleTextureProvider::update(unsigned int elapsedMillis) {
    // do nothing
    return false;
}

}
}
