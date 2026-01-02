#pragma once

#include "SimpleEngine2D/components/Audio.hpp"

namespace simpleengine2d::events {

struct PlayAudio {
    components::AudioComponent audio;
};


}