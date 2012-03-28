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

#include "audiomanager.hpp"

#include <fmodex/fmod_errors.h>
#include <boost/filesystem/operations.hpp>

#include <data/manager.hpp>
#include <data/soundloader.hpp>
#include <misc/exception.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace ui {
    
AudioManager::AudioManager(Config& config) : fmodSystem_(nullptr), backgroundMusic_(nullptr) {
    FMOD_RESULT result;
    
    LOG_INFO << "Initializing fmod" << std::endl;
    result = FMOD::System_Create(&fmodSystem_);
    if (result != FMOD_OK) {
        LOG_EMERGENCY << "fmod error on FMOD::System_Create: " << FMOD_ErrorString(result) << " (" << result << ")" << std::endl;
        throw Exception("Unable to intialize fmod sound system");
    }
    
    result = fmodSystem_->init(16, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK) {
        LOG_EMERGENCY << "fmod error on FMOD::System::init: " << FMOD_ErrorString(result) << " (" << result << ")" << std::endl;
        throw Exception("Unable to intialize fmod sound system");
    }
    
    musicOff_ = config["/fluo/audio/music@off"].asBool();
    soundOff_ = config["/fluo/audio/sound@off"].asBool();
    musicVolume_ = config["/fluo/audio/music@volume"].asInt() / 100.f;
    soundVolume_ = config["/fluo/audio/sound@volume"].asInt() / 100.f;
}

AudioManager::~AudioManager() {
    FMOD_RESULT result;
    
    if (backgroundMusic_) {
        result = backgroundMusic_->release();
    }
    
    if (fmodSystem_) {
        result = fmodSystem_->close();
        if (result != FMOD_OK) {
            LOG_ERROR << "fmod error on FMOD::System::close: " << FMOD_ErrorString(result) << " (" << result << ")" << std::endl;
        }
        result = fmodSystem_->release();
        if (result != FMOD_OK) {
            LOG_ERROR << "fmod error on FMOD::System::release: " << FMOD_ErrorString(result) << " (" << result << ")" << std::endl;
        }
    }
}

void AudioManager::playMusic(unsigned int musicId) {
    if (musicOff_ || musicVolume_ == 0) {
        return;
    }
    
    data::MusicConfigDef configEntry = data::Manager::getMusicConfigDef(musicId);
    
    if (configEntry.musicId_ != musicId) {
        LOG_WARN << "Unknown music id " << musicId << std::endl;
        return;
    }
    
    LOG_DEBUG << "Playing music id: " << musicId << " filename=" << configEntry.fileName_ << " loop=" << configEntry.loop_ << std::endl;
    
    UnicodeString pathStr = "music/digital/" + configEntry.fileName_;
    boost::filesystem::path path = data::Manager::getFilePathFor(pathStr);
    
    if (!boost::filesystem::exists(path)) {
        LOG_WARN << "Unable to open music file " << path.string() << std::endl;
        return;
    }
    
    stopMusic();
    
    FMOD_RESULT result;
    
    result = fmodSystem_->createSound(path.string().c_str(), FMOD_HARDWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &backgroundMusic_);
    if (result != FMOD_OK) {
        LOG_ERROR << "AudioManager::playMusic: fmod error on FMOD::System::createSound: " << FMOD_ErrorString(result) << " (" << result << ")" << std::endl;
        return;
    }
    
    if (configEntry.loop_) {
        result = backgroundMusic_->setMode(FMOD_LOOP_NORMAL);
        if (result != FMOD_OK) {
            LOG_ERROR << "AudioManager::playMusic: fmod error on FMOD::Sound::setMode(FMOD_LOOP_NORMAL): " << FMOD_ErrorString(result) << " (" << result << ")" << std::endl;
            return;
        }
    }
    
    FMOD::Channel* channel;
    result = fmodSystem_->playSound(FMOD_CHANNEL_FREE, backgroundMusic_, false, &channel);
    if (result != FMOD_OK) {
        LOG_ERROR << "AudioManager::playMusic: fmod error on FMOD::System::playSound: " << FMOD_ErrorString(result) << " (" << result << ")" << std::endl;
        return;
    }
    
    result = channel->setVolume(musicVolume_);
    if (result != FMOD_OK) {
        LOG_ERROR << "AudioManager::playMusic: fmod error on FMOD::Channel::setVolume: " << FMOD_ErrorString(result) << " (" << result << ")" << std::endl;
        return;
    }
}

void AudioManager::stopMusic() {
    FMOD_RESULT result;
    if (backgroundMusic_) {
        result = backgroundMusic_->release();
        if (result != FMOD_OK) {
            LOG_ERROR << "AudioManager::playMusic: fmod error on FMOD::Sound::release: " << FMOD_ErrorString(result) << " (" << result << ")" << std::endl;
        }
    }
}

void AudioManager::playSound(unsigned int soundId) {
    if (soundOff_) {
        return;
    }
    
    data::SoundDef soundDef = data::Manager::getSoundDef(soundId);
    if (soundDef.soundId_ == soundId) {
        if (soundDef.translateId_ == -1) {
            return;
        } else {
            soundId = soundDef.translateId_;
        }
    }
    
    data::Manager::getSoundLoader()->loadAndPlay(soundId);
}

void AudioManager::playSoundFromMul(const char* buf, unsigned int length) {
    FMOD::Sound* sound;
    FMOD_CREATESOUNDEXINFO exinfo;
    FMOD_RESULT       result;
    
    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.length = length;
    result = fmodSystem_->createSound(buf, FMOD_HARDWARE | FMOD_2D | FMOD_OPENMEMORY, &exinfo, &sound);
    
    if (result != FMOD_OK) {
        LOG_ERROR << "AudioManager::createSoundFromMul: fmod error on FMOD::System::createSound: " << FMOD_ErrorString(result) << " (" << result << ")" << std::endl;
        return;
    }
    
    playSound(sound);
}

void AudioManager::playSound(FMOD::Sound* sound) {
    FMOD::Channel* channel;
    FMOD_RESULT       result;
    
    result = fmodSystem_->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
    if (result != FMOD_OK) {
        LOG_ERROR << "AudioManager::playSound: fmod error on FMOD::System::playSound: " << FMOD_ErrorString(result) << " (" << result << ")" << std::endl;
        return;
    }
    
    result = channel->setVolume(soundVolume_);
    if (result != FMOD_OK) {
        LOG_ERROR << "AudioManager::playSound: fmod error on FMOD::Channel::setVolume: " << FMOD_ErrorString(result) << " (" << result << ")" << std::endl;
        return;
    }
}

}
}

