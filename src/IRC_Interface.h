#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#define NOMINMAX

#undef LoadImage
#undef DrawText
#undef DrawTextEx
#undef PlaySound

class IRC_Interface {

public:
    virtual void Start(bool secure, const char* url) = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void End() = 0;
};