#pragma once
#include "IRC_Interface.h"

class Client : public IRC_Interface {
private:
    float m_Width;
    float m_Height;
public:
    Client(float width,float height);
    void Start(bool secure, const char* url) override;
    void Update() override;
    void Draw() override;
    void End() override;
};
