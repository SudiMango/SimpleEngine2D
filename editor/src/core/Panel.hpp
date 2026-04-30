#pragma once

namespace simpleengine2d::editor {

class Panel {

public:
    virtual void init() {};
    virtual void update(float dt) {};
    virtual void clean() {};

};

}