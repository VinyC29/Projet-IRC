#define KNOB_IMPLEMENTATION
#include "knob.h"

#include "function_to_test.h"

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
}