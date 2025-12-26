#pragma once

#include <cstdint>
#include <vector>

namespace simpleengine2d::core {

class System {

public:
    virtual ~System() = default;

    virtual void init() = 0;
    virtual void update(float dt) = 0;
    virtual void fixedUpdate(float f_dt) {};
    virtual void clean() = 0;

};

}