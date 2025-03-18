#pragma once

class ConnectIRC {
private:

public:
    static SOCKET CreateSocket();
    static void Connect(const bool secure, const char* address);
};
