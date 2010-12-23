#include "remoteclient.h"
namespace CubeJRemote {

    template <CubeJProtocol::MSG_TYPE N> void receiveMessage(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<CubeJProtocol::MSG_ERROR_OVERFLOW>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<CubeJProtocol::MSG_ERROR_TAG>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<CubeJProtocol::MSG_SND_SERVINFO>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        int protocol = getint(p);
        if(protocol != CubeJProtocol::PROTOCOL_VERSION) {
            std::cout << "[DEBUG] receiveMessage<CubeJProtocol::MSG_SND_SERVINFO> - wrong protocol version" << std::endl;
            return;
        }
        std::cout << "[DEBUG] receiveMessage<MSG_SND_SERVINFO> clientnum: " << clientnum << ", protocol: " << protocol << std::endl;

        GetRemoteClient().setclientnum(clientnum);

        CubeJProtocol::MsgDataType<CubeJProtocol::MSG_REQ_REMOTE> data;
        SendMessage(data);
	}

    template <> void receiveMessage<CubeJProtocol::MSG_SND_CLIENTINFO>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        char text[MAXTRANS];
        getstring(text, p);
        std::cout << "[DEBUG] receiveMessage<CubeJProtocol::MSG_SND_CLIENTINFO> clientnum: %d, name: %s" << clientnum << text << std::endl;
    }

    RemoteClient::RemoteClient() : peer(NULL), host(NULL), rate(0), numchannels(3), clientnum(-1) {
        CubeJProtocol::RegisterMsgHandler( CubeJProtocol::MSG_SND_SERVINFO , receiveMessage<CubeJProtocol::MSG_SND_SERVINFO>);
        CubeJProtocol::RegisterMsgHandler( CubeJProtocol::MSG_ERROR_OVERFLOW , receiveMessage<CubeJProtocol::MSG_ERROR_OVERFLOW>);
        CubeJProtocol::RegisterMsgHandler( CubeJProtocol::MSG_ERROR_TAG , receiveMessage<CubeJProtocol::MSG_ERROR_TAG>);
    }

    RemoteClient::~RemoteClient()
    {
        disconnect();
    }

    void RemoteClient::connect(const char* sname, int port, int r) {
        servername = sname;
        rate = r;
        address.port = port;

        if(enet_address_set_host (&address, servername.c_str()) < 0 )
        {
            std::cout << "[DEBUG] RemoteClient::connect - could not resolve server " << servername;
            return;
        }

        host = enet_host_create(NULL, 2, numchannels, rate, rate);

        if(host)
        {
            std::cout << "[DEBUG] RemoteClient::connect - connecting to host" << std::endl;
            peer = enet_host_connect(host, &address, numchannels, 0);
            enet_host_flush(host);
        }
        else std::cout << "[DEBUG] RemoteClient::connect - could not connect to server";
    }

    void RemoteClient::update() {
        ENetEvent event;
        while(host && enet_host_service(host, &event, 0)>0) {
            switch(event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "[DEBUG] RemoteClient::update - connected to server" << std::endl;

                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                {
                    packetbuf p(event.packet);
                    int chan = event.channelID;
                    CubeJProtocol::ReceiveMessage( -1, chan, p);
                    enet_packet_destroy(event.packet);
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "[DEBUG] RemoteClient::update - ENET_EVENT_TYPE_DISCONNECT" << std::endl;
                    disconnect();
                    break;

                default:
                    break;
            }
        }
    }

    void RemoteClient::setclientnum(int n) {
        clientnum = n;
        std::cout << "[DEBUG] RemoteClient::setclientnum " << clientnum << std::endl;
    }

    void RemoteClient::disconnect() {
        std::cout << "[DEBUG] RemoteClient::disconnect"<< std::endl;
        enet_peer_disconnect(peer, DISC_NONE);
        enet_host_flush(host);
        enet_peer_reset (peer);

    }

    RemoteClient& GetRemoteClient() {
        static RemoteClient remoteclient;
        return remoteclient;
    }
}
