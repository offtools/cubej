#ifndef REMOTE_CUBEJDISPATCHER_H
#define REMOTE_CUBEJDISPATCHER_H

#include "dispatcher.h"
#include "remoteclient.h"
#include "juce_amalgamated.h"
#include "ui_listener.h"
#include "ui_maintoolbar.h"
#include "ui_connectcomponent.h"
#include "ui_scenecomponent.h"
#include "ui_console.h"

class NetworkDispatcher :   public CubeJRemote::RemoteInterface,
                            public Timer,
                            public AppMessageListener<MainToolbar>,
                            public AppMessageCommandListener<ConnectComponent>,
                            public AppMessageCommandListener<SceneComponent>,
                            public AppMessageCommandListener<Console>
{
    public:
        NetworkDispatcher();
        ~NetworkDispatcher();

        void connectWithServer(const char* sname, int port);

        //Network Message Callbacks


        //Command ID's
        enum AppCommandID {
            CONNECT_CLIENT = 0,
            DISCONNECT_CLIENT,
            REQ_CLIENTLIST,
            MSG_REQ_CHANGESCENE,
            MSG_FWD_RCIN
        } ;

        //Application Message Callback
        void handleData (MainToolbar* data);
        void handleData (int id, ConnectComponent* data);
        void handleData (int id, SceneComponent* data);
        void handleData (int id, Console* data);

    private:
        void timerCallback();
};

#endif // REMOTE_CUBEJDISPATCHER_H
