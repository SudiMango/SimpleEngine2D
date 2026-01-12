#include "SimpleEngine2D/systems/Audio.hpp"

namespace simpleengine2d::systems {

void Audio::init() {
    core::EventBus::getInstance().subscribe<events::PlayAudio>(this, [this](void *evt) {
        events::PlayAudio *_evt = (events::PlayAudio*)evt;

        if (_evt->audio.isMusic) {
            Mix_Music *music = getOrCreateMusic(_evt->audio.filePath);
            if (music) {
                Mix_VolumeMusic(_evt->audio.volume);
                Mix_PlayMusic(music, _evt->audio.numLoops);
            }
        } else {
            Mix_Chunk *sound = getOrCreateChunk(_evt->audio.filePath);
            if (sound) {
                Mix_VolumeChunk(sound, _evt->audio.volume);
                Mix_PlayChannel(-1, sound, _evt->audio.numLoops);
            }
        }
    });

    core::EventBus::getInstance().subscribe<events::ControlAudio>(this, [this](void *evt) {
        events::ControlAudio *_evt = (events::ControlAudio*)evt;

        if (_evt->isMusic) {
            if (_evt->action == events::AudioAction::STOP) {
                Mix_HaltMusic();
            } else if (_evt->action == events::AudioAction::PAUSE) {
                Mix_PauseMusic();
            } else if (_evt->action == events::AudioAction::RESUME) {
                Mix_ResumeMusic();
            }
        } else {
            if (_evt->action == events::AudioAction::STOP) {
                Mix_HaltChannel(-1);
            } else if (_evt->action == events::AudioAction::PAUSE) {
                Mix_Pause(-1);
            } else if (_evt->action == events::AudioAction::RESUME) {
                Mix_Resume(-1);
            }
        }
    });
}

void Audio::clean() {
    core::EventBus::getInstance().unsubscribe(this);

    for (auto const& [path, chunk] : soundCache) {
        Mix_FreeChunk(chunk);
    }
    soundCache.clear();

    for (auto const& [path, music] : musicCache) {
        Mix_FreeMusic(music);
    }
    musicCache.clear();
}

}