#pragma once

#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/core/ConfigManager.hpp"

namespace simpleengine2d::core {

class Scene {

public:
    virtual void setup() = 0;
    virtual void clean() {};

protected:
    EntityManager &em = EntityManager::getInstance();
    Config &config = ConfigManager::getInstance().getConfig();

};

}