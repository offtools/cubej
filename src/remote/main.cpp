#include <string>
#include <iostream>

#include "config.h"
#include "juce_amalgamated.h"
#include "ui_mainwindow.h"

//CHANNEL 4: Client to Control
//CHANNEL 5: Control to Client

//void cleanup()
//{
//    recorder::stop();
//    cleanupserver();
//    SDL_ShowCursor(1);
//    SDL_WM_GrabInput(SDL_GRAB_OFF);
//    SDL_SetGamma(1, 1, 1);
//    freeocta(worldroot);
//    extern void clear_command(); clear_command();
//    extern void clear_console(); clear_console();
//    extern void clear_mdls();    clear_mdls();
//    extern void clear_sound();   clear_sound();
//    SDL_Quit();
//}

//void quit()                     // normal exit
//{
//    writeservercfg();
//    abortconnect();
//    disconnect();
//    localdisconnect();
//    writecfg();
//    cleanup();
//    exit(EXIT_SUCCESS);
//}

class RemoteApp  : public JUCEApplication
{
public:
    //==============================================================================
    RemoteApp() : mainwindow(0) {}

    ~RemoteApp() {}

    //==============================================================================
    void initialise (const String& commandLine)
    {
//        if(!CubeJRemote::Init()) {
//            Logger::outputDebugString (T("Could not initialize Enet"));
//            systemRequestedQuit();
//        }

        mainwindow = new MainWindow();
        mainwindow->centreWithSize (1024, 800);
        mainwindow->setVisible (true);
    }

    void shutdown()
    {
        // Do your application's shutdown code here..
        std::cout << "RemoteApp::shutdown" << std::endl;
        delete mainwindow;
        mainwindow = 0;
    }

    void systemRequestedQuit()
    {
        std::cout << "RemoteApp::systemRequestedQuit" << std::endl;
        shutdown();
        JUCEApplication::quit();
    }

    const String getApplicationName()
    {
        return AppInfo::projectName;
    }

    const String getApplicationVersion()
    {
        return AppInfo::versionString;
    }

    bool moreThanOneInstanceAllowed()
    {
        return true;
    }

    void anotherInstanceStarted (const String& commandLine)
    {

    }

private:
      MainWindow* mainwindow;
};

START_JUCE_APPLICATION(RemoteApp)

