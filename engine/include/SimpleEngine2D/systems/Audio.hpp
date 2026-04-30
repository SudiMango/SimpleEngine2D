#pragma once

#include <iostream>
#include <map>
#include <SDL_mixer.h>
#include <string>
#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EventBus.hpp"
#include "SimpleEngine2D/events/PlayAudio.hpp"
#include "SimpleEngine2D/events/ControlAudio.hpp"

namespace simpleengine2d::systems {

class Audio : public core::System {

public:

    void init() override;
    void update(float dt) override {};
    void clean() override;

private:
    std::map<std::string, Mix_Chunk*> soundCache;
    std::map<std::string, Mix_Music*> musicCache;

    Mix_Chunk* getOrCreateChunk(std::string path) {
        if (path.empty()) return nullptr;
        if (soundCache.find(path) == soundCache.end()) {
            soundCache[path] = Mix_LoadWAV(path.c_str());
        }
        return soundCache[path];
    }

    Mix_Music* getOrCreateMusic(std::string path) {
        if (path.empty()) return nullptr;
        if (musicCache.find(path) == musicCache.end()) {
            musicCache[path] = Mix_LoadMUS(path.c_str());
        }
        return musicCache[path];
    }

};

}