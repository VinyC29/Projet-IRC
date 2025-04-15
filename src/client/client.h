#pragma once
#include "IRC_Interface.h"

class Client : public IRC_Interface {
private:
    float m_Width;
    float m_Height;
public:
    void Start(bool secure, const char* url) override;
    Client(float width,float height);
    void Update() override;
    void Draw() override;
    void End() override;
};
