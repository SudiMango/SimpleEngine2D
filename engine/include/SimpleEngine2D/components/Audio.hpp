#pragma once

namespace simpleengine2d::components {

struct AudioComponent {
    bool isMusic = false;
    char *filePath;
    int numLoops = 0; // 0 = no loops, -1 = infinite loops
    int volume = 128; // 0 - 128
};

}