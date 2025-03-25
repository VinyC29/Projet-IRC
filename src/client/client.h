#pragma once
#include "IRC_Interface.h"

class Client : public IRC_Interface {
private:
    
public:
    void Start(const char* url) override;
    void Update() override;
    void Draw() override;
    void End() override;
};
