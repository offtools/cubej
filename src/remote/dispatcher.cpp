#include "dispatcher.h"
#include "clientinfo.h"

NetworkDispatcher::NetworkDispatcher()
{
}

NetworkDispatcher::~NetworkDispatcher()
{

}

void NetworkDispatcher::connectWithServer(const char* sname, int port) {
    if( !RemoteInterface::connectWithServer(sname, port) )
        return;
    startTimer(50);
}

inline void NetworkDispatcher::timerCallback() {
    update();
}

void NetworkDispatcher::handleData (MainToolbar* data) {
    std::cout << "AppMessageListener<MainToolbar>::handleData" << data->getServerName() << data->getServerPort() << std::endl;
    connectWithServer(data->getServerName().toCString(), data->getServerPort());
}

void NetworkDispatcher::handleData (int id, ConnectComponent* data) {
    switch (id)
    {
        case CONNECT_CLIENT:
        {
            const CubeJ::ClientInfo* ci = data->getSelectedClient();
            if( ci ) {
                connectWithClient(ci->getClientnum());
            }
            return;
        }
        case DISCONNECT_CLIENT:
        {
            std::cout << "NetworkDispatcher::handleData - DISCONNECT" << std::endl;
            return;
        }
        case MSG_DISCOVER_REMOTE:
        {
            std::cout << "NetworkDispatcher::handleData - MSG_DISCOVER_REMOTE" << std::endl;
            CubeJ::MsgDataType<CubeJ::MSG_DISCOVER_REMOTE> snd;
            SendMessage(snd);
            return;
        }
        default:
            return;
    }
}

void NetworkDispatcher::handleData (int id, SceneComponent* data) {
    if ( id == MSG_REQ_CHANGESCENE ) {
        RequestChangeScene(data->getSelectedSceneName().c_str());
    }
}

void NetworkDispatcher::handleData (int id, Console* data) {
    if ( id == MSG_FWD_RCIN ) {
        std::cout << "NetworkDispatcher::handleData: " <<  data->getCurrentCommand() << std::endl;
        SendCommand(data->getCurrentCommand().toCString() );
        data->clearCommandLine();
    }
}
