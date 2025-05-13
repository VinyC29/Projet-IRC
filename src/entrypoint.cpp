#include "entrypoint.h"
#include "raylib.h"
#include "knob.h"
#include "IRC_Interface.h"
#include "server/server.h"
#include "client/client.h"
#include "rlImGui.h"

IRC_Interface* app = NULL;

void raylib_start(void){
    InitWindow(720,720,"Irc");
    float w = GetMonitorWidth(GetCurrentMonitor()) * 0.75f;  
    float h = GetMonitorHeight(GetCurrentMonitor()) * 0.5f;
    SetWindowSize(w,h);
    SetTargetFPS(60);

    
    Color background = RED;
    #ifdef SERVER
    app = new Server(w,h);
    #elif defined(CLIENT)
    background = BLUE;
    app = new Client(w,h);
    #endif


    app->Start(false, "127.0.0.1");
    
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

	rlImGuiSetup(true);
    
    while(!WindowShouldClose()){
        ClearBackground(background);
        BeginDrawing();
        //TODO: Do stuff

        app->Update();
        rlImGuiBegin();
        app->Draw();
        rlImGuiEnd();
        EndDrawing();
    }

    app->End();
    rlImGuiShutdown();
    CloseWindow();
}