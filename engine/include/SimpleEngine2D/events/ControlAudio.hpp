#pragma once

namespace simpleengine2d::events {

enum class AudioAction { STOP, PAUSE, RESUME };

struct ControlAudio {
    AudioAction action;
    bool isMusic = false;
};

}