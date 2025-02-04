#include "entrypoint.h"
#include "raylib.h"

#define KNOB_IMPLEMENTATION
#include "knob.h"

void raylib_start(void){
    InitWindow(960,540,"Irc");
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        ClearBackground(RAYWHITE);
        BeginDrawing();
        //TODO: Do stuff
        EndDrawing();
    }

    CloseWindow();
}