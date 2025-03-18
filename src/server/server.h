#pragma once
#include "IRC_Interface.h";

class Server : public IRC_Interface {
private:
    
public:
    void Start(bool secure, const char* url) override;
    void Update() override;
    void Draw() override;
    void End() override;
};
