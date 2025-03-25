#include "verificator.h"
#include <cstring>

bool IsStringNullOrEmpty(char* stringToCheck){

    if (strlen(stringToCheck) == 0 || strspn(stringToCheck, " \t\n\r\f\v") == strlen(stringToCheck))
    {
        return true;
    }

    return false;
}