#pragma once
#include "verificator.h"
#include <cstring>
#include <string>

static bool IsStringNullOrEmpty(char* stringToCheck){

    if (strlen(stringToCheck) == 0 || strspn(stringToCheck, " \t\n\r\f\v") == strlen(stringToCheck))
    {
        return true;
    }

    return false;
}