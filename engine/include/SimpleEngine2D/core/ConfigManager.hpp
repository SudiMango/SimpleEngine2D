#pragma once

#include <glm/glm.hpp>

namespace simpleengine2d::core {

struct _windowconfig {
    char *title = (char*)"SimpleEngine2D";
    int width = 1920;
    int height = 1088;
    bool fullscreen = false;
};

struct _renderconfig {
    glm::ivec4 backgroundColor = glm::ivec4(255);
};

struct Config {
    _windowconfig window;
    _renderconfig render;
};


class ConfigManager {

public:
    static ConfigManager &getInstance() {
        static ConfigManager instance;
        return instance;
    }

    Config &getConfig() {
        return config;
    }

private:
    ConfigManager() = default;
    ~ConfigManager() = default;

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    Config config;

};

}