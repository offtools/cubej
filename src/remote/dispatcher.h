#ifndef REMOTE_CUBEJDISPATCHER_H
#define REMOTE_CUBEJDISPATCHER_H

#include "dispatcher.h"
#include "remoteclient.h"
#include "juce_amalgamated.h"
#include "ui_maintoolbar.h"
#include "ui_listener.h"
#include "ui_connectcomponent.h"

class NetworkDispatcher;

//class ConnectServerButtonListener : public ButtonListenerAdapter<MainToolbar> {
//    public:
//        ConnectServerButtonListener(MainToolbar& toolbar, NetworkDispatcher& net) : ButtonListenerAdapter<MainToolbar>(toolbar), nethandle(net) {}
//        void buttonClicked(Button* button);
//    private:
//        NetworkDispatcher& nethandle;
//};

class NetworkDispatcher :   public CubeJRemote::RemoteInterface,
                            public Timer,
                            public AppMessageListener<ConnectComponent>
{
    public:
        NetworkDispatcher();
        ~NetworkDispatcher();

        void connectWithServer(const char* sname, int port);

        //Callback for MSG_SND_SRVINFO
        void CallbackSrvInfo(int sender, int channel, packetbuf& p);
        void handleData (ConnectComponent* data);
    private:
        void timerCallback();
};

#endif // REMOTE_CUBEJDISPATCHER_H
