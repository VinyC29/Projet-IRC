#include "entrypoint.h"
#include "raylib.h"
#include "knob.h"
#include "IRC_Interface.h"
#include "server/server.h"
#include "client/client.h"

IRC_Interface* app = NULL;

void raylib_start(void){
    InitWindow(720,720,"Irc");
    float w = GetMonitorWidth(GetCurrentMonitor()) * 0.75f;  
    float h = GetMonitorHeight(GetCurrentMonitor()) * 0.5f;
    SetWindowSize(w,h);
    SetTargetFPS(60);

    
    Color background = RED;
    #ifdef SERVER
    app = new Server();
    #elif defined(CLIENT)
    background = BLUE;
    app = new Client(w,h);
    #endif


    app->Start(false, "127.0.0.1");

    while(!WindowShouldClose()){
        ClearBackground(background);
        BeginDrawing();
        //TODO: Do stuff

        app->Update();
        app->Draw();

        EndDrawing();
    }

    app->End();

    CloseWindow();
}