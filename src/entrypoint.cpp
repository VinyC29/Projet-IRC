#include "entrypoint.h"
#include "raylib.h"
#include "knob.h"
#include "IRC_Interface.h"
#include "server/server.h"
#include "client/client.h"

IRC_Interface* app = NULL;

void raylib_start(void){
    Color background = RED;
    #ifdef SERVER
    app = new Server();
    #elif defined(CLIENT)
    background = BLUE;
    app = new Client();
    #endif

    InitWindow(960,540,"Irc");
    SetTargetFPS(60);

    app->Start("127.0.0.1");

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