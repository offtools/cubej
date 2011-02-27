#include "dispatcher.h"

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

void NetworkDispatcher::CallbackSrvInfo(int sender, int channel, packetbuf& p) {
    CubeJProtocol::MsgDataType<CubeJProtocol::MSG_SND_SERVINFO> data(p);
    if(data.protocol != CubeJProtocol::PROTOCOL_VERSION) {
        std::cout << "[DEBUG] NetworkDispatcher::receiveMessage<CubeJProtocol::MSG_SND_SERVINFO> - wrong protocol version" << std::endl;
        return;
    }
    std::cout << "[DEBUG] NetworkDispatcher::receiveMessageCallback<MSG_SND_SERVINFO> clientnum: " << data.clientnum << ", protocol: " << data.protocol << std::endl;

    CubeJProtocol::MsgDataType<CubeJProtocol::MSG_DISCOVER_REMOTE> snd;
    SendMessage(snd);
}

//void ConnectServerButtonListener::buttonClicked(Button* button) {
//    std::cout << "ConnectServerButtonListener::buttonClicked" << std::endl;
//    nethandle.connectWithServer(ref.getServerName().toCString (), ref.getServerPort());
//}

void NetworkDispatcher::handleData (ConnectComponent* data) {
    std::cout << "AppMessageListener::handleMessage: " << data->teststring << std::endl;
}
