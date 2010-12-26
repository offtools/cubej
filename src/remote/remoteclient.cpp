#include "remoteclient.h"
namespace CubeJRemote {

    RemoteClient::RemoteClient() : peer(NULL), host(NULL), rate(0), numchannels(3), clientnum(-1) {
        registerMsgHandler( CubeJProtocol::MSG_ERROR_OVERFLOW , receiveMessage<CubeJProtocol::MSG_ERROR_OVERFLOW>);
        registerMsgHandler( CubeJProtocol::MSG_ERROR_TAG , receiveMessage<CubeJProtocol::MSG_ERROR_TAG>);
        registerMsgHandler( CubeJProtocol::MSG_SND_SERVINFO , receiveMessage<CubeJProtocol::MSG_SND_SERVINFO>);
        registerMsgHandler( CubeJProtocol::MSG_SND_CLIENTINFO , receiveMessage<CubeJProtocol::MSG_SND_CLIENTINFO>);
        registerMsgHandler( CubeJProtocol::MSG_SND_SCENESTATUS , receiveMessage<CubeJProtocol::MSG_SND_SCENESTATUS>);
        registerMsgHandler( CubeJProtocol::MSG_ACK_REMOTE , receiveMessage<CubeJProtocol::MSG_ACK_REMOTE>);
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
                    while(p.remaining()) {
                        receive( -1, chan, p);
                    }
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

    int RemoteClient::getclientnum() {
        return clientnum;
    }

	void RemoteClient::updateclientcache(int cn, int type, char* name) {
		loopv(clientcache) if(clientcache[i]->getclientnum() == cn) {
			delete clientcache.remove(i);
		}
		CubeJ::ClientInfo* ci = clientcache.add(new CubeJ::ClientInfo(type, cn));
		if(name && *name && ci->gettype() == CubeJ::CLIENT_TYPE_HEAD) {
			ci->setName(name);
		}
	}

    void RemoteClient::disconnect() {
        std::cout << "[DEBUG] RemoteClient::disconnect"<< std::endl;
        enet_peer_disconnect(peer, DISC_NONE);
        enet_host_flush(host);
        enet_peer_reset (peer);
		clientcache.deletecontents();
    }

    RemoteClient& GetRemoteClient() {
        static RemoteClient remoteclient;
        return remoteclient;
    }
}
