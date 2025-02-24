#include "entrypoint.h"
#include "raylib.h"
#include "knob.h"

void raylib_start(void){
    InitWindow(960,540,"Irc");
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        ClearBackground(RED);
        BeginDrawing();
        //TODO: Do stuff
        EndDrawing();
    }

    CloseWindow();
}