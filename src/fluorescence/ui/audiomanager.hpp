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


#ifndef FLUO_UI_AUDIOMANAGER_HPP
#define FLUO_UI_AUDIOMANAGER_HPP

#include <map>
#include <fmodex/fmod.hpp>

#include <misc/config.hpp>

namespace fluo {
namespace ui {

class AudioManager {
    
private:
    struct MusicConfig {
        UnicodeString name_;
        bool loop_;
    };
    
public:
    AudioManager(Config& config);
    ~AudioManager();
    
    // starts playing the corresponding mp3 file
    void playMusic(unsigned int musicId);
    
    // calls the data::Sound loader to load the sound. The loader will also asynchronously call playSoundFromMul
    void playSound(unsigned int soundId);

    void playSoundFromMul(const char* buf, unsigned int length);
    void stopMusic();

private:
    FMOD::System* fmodSystem_;
    FMOD::Sound* backgroundMusic_;
    
    void initMusicConfig();
    std::map<unsigned int, MusicConfig> musicConfig_;
    
    bool musicOff_;
    bool soundOff_;
    float musicVolume_;
    float soundVolume_;
    
    void playSound(FMOD::Sound* sound);
};

}
}

#endif

