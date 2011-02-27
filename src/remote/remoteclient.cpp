#include "remoteclient.h"
namespace CubeJRemote {

    RemoteInterface::RemoteInterface() : peer(NULL), host(NULL), rate(25000), numchannels(CubeJProtocol::NUM_CHANNELS) {}

    RemoteInterface::~RemoteInterface()
    {
        disconnect();
    }

    bool RemoteInterface::connectWithServer(const char* sname, int port) {
        address.port = port;

        if(enet_address_set_host (&address, sname) < 0 )
        {
            std::cout << "[DEBUG] RemoteInterface::connectWithServer - could not resolve server " << sname;
            return false;
        }

        host = enet_host_create(NULL, 2, numchannels, rate, rate);

        if(host)
        {
            std::cout << "[DEBUG] RemoteInterface::connectWithServer - connecting to host: " << sname << ":" << address.port << std::endl;
            peer = enet_host_connect(host, &address, numchannels, 0);
            enet_host_flush(host);
            return true;
        }
        else std::cout << "[DEBUG] RemoteInterface::connectWithServer - could not connect to server";
        return false;
    }

    void RemoteInterface::connectWithClient(int cn) {
        CubeJProtocol::MsgDataType<CubeJProtocol::MSG_REQ_REMOTE> data(cn);
        SendMessage(data);
    }

    void RemoteInterface::disconnectClient() {
    }

    void RemoteInterface::update() {
        ENetEvent event;
        while(host && enet_host_service(host, &event, 0)>0) {
            switch(event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "[DEBUG] RemoteInterface::update - connected to server" << std::endl;

                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                {
                    packetbuf p(event.packet);
                    int chan = event.channelID;
                    while(p.remaining()) {
                        receive( -1, chan, p);
                    }
                    enet_packet_destroy(event.packet);
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "[DEBUG] RemoteInterface::update - ENET_EVENT_TYPE_DISCONNECT" << std::endl;
                    disconnect();
                    break;

                default:
                    break;
            }
        }
    }

    void RemoteInterface::SendMessage(int channel, packetbuf& p) {
        if(peer)
            enet_peer_send(peer, channel, p.finalize());
    }

    void RemoteInterface::disconnect() {
        std::cout << "[DEBUG] RemoteInterface::disconnect"<< std::endl;
        if(peer) {
            enet_peer_disconnect(peer, DISC_NONE);
            enet_peer_reset (peer);
        }
        if(host)
            enet_host_flush(host);
    }


    void RemoteInterface::RequestMapList() {
        CubeJProtocol::MsgDataType<CubeJProtocol::MSG_REQ_LISTMAPS> data;
        SendMessage(data);
    }

    void RemoteInterface::RequestClientInfoList() {}

    void RemoteInterface::RequestChangeScene(const char* path) {
        CubeJProtocol::MsgDataType<CubeJProtocol::MSG_REQ_CHANGESCENE> data(path);
        SendMessage(data);
    }

    bool Init() {
        if(enet_initialize () < 0)
            return false;

        CubeJProtocol::Init();
        return true;
    }
}
