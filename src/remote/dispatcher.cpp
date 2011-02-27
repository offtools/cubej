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
        case REQ_CLIENTLIST:
        {
            std::cout << "NetworkDispatcher::handleData - REQ_CLIENTLIST" << std::endl;
            CubeJProtocol::MsgDataType<CubeJProtocol::MSG_DISCOVER_REMOTE> snd;
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
