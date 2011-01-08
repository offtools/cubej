#include <string>
#include <iostream>

#include "engine.h"
#include "cube.h"

#include "cubej.h"
#include "protocol.h"
#include "remoteclient.h"

//CHANNEL 4: Client to Control
//CHANNEL 5: Control to Client

void cleanup()
{
    recorder::stop();
    cleanupserver();
    SDL_ShowCursor(1);
    SDL_WM_GrabInput(SDL_GRAB_OFF);
    SDL_SetGamma(1, 1, 1);
    freeocta(worldroot);
    extern void clear_command(); clear_command();
    extern void clear_console(); clear_console();
    extern void clear_mdls();    clear_mdls();
    extern void clear_sound();   clear_sound();
    SDL_Quit();
}

void quit()                     // normal exit
{
    writeservercfg();
    abortconnect();
    disconnect();
    localdisconnect();
    writecfg();
    cleanup();
    exit(EXIT_SUCCESS);
}

bool doquit = false;

using namespace CubeJRemote;

int main (void) {

    std::cout << "[DEBUG] CubeJRemote - remote App" << std::endl;

    if(enet_initialize () < 0) {
        std::cout << "[DEBUG] CubeJRemote - could not init enet" << std::endl;
        return -1;
    }
    CubeJProtocol::Init();
    RemoteClient& remoteclient = GetRemoteClient();
    remoteclient.connectToServer("localhost", 26785, 25000);

    usleep(1000000);


    while (!doquit) {
        usleep(100000);
        remoteclient.update();
    }

    return 0;
}
