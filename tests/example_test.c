#define KNOB_IMPLEMENTATION
#include "knob.h"

#include "function_to_test.h"
#include "serverUnitTest.h"

#define TEST(message,expected,got,cmp,showHas) do { \
    if(!(expected cmp got)){ \
        knob_log(KNOB_ERROR,"\033[31m[TEST Failed] %s expected " showHas " got " showHas "\033[0m",message,expected,got); \
    } \
    else{ \
        knob_log(KNOB_INFO,"\033[32m[TEST Success] %s\033[0m",message); \
    } \
} \
while(0) \

MAIN(Validate_functions){
    TEST("This should fail",68,add(34,35),==,"%d");
    TEST("This should succeed",69,add(34,35),==,"%d");
    TEST("Sending server test message | Should receive: | :projectirc.example.com 001 Test :Welcome to the IRC Project\r\n |", 1, knob_cstr_match(":projectirc.example.com 001 Test :Welcome to the IRC Project\r\n", SimulateMessageToServer("NICK Test\r\nUSER Test * * :Test Lol\r\n", "\r\n ")), ==, "%d");
}