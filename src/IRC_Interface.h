#pragma once

class IRC_Interface {

public:
    virtual void Start(const char* url) = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void End() = 0;
};