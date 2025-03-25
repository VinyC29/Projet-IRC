#pragma once
#include <cstring>
#include <string>

static bool IsNullOrEmpty(char[] stringToCheck){
    std::string stringCheck = stringToCheck;

    if (strlen(stringCheck) == 0 || strspn(stringCheck, " \t\n\r\f\v") == strlen(stringCheck))
    {
        return true;
    }

    return false;
}