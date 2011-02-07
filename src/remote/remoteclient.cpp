#include "remoteclient.h"
namespace CubeJRemote {

    RemoteClient::RemoteClient() : peer(NULL), host(NULL), rate(25000), numchannels(CubeJProtocol::NUM_CHANNELS), clientnum(-1), head(NULL), scenemgr(0) {
        registerMsgHandler( CubeJProtocol::MSG_ERROR_OVERFLOW , receiveMessage<CubeJProtocol::MSG_ERROR_OVERFLOW>);
        registerMsgHandler( CubeJProtocol::MSG_ERROR_TAG , receiveMessage<CubeJProtocol::MSG_ERROR_TAG>);
        registerMsgHandler( CubeJProtocol::MSG_SND_SERVINFO , receiveMessage<CubeJProtocol::MSG_SND_SERVINFO>);
        registerMsgHandler( CubeJProtocol::MSG_SND_CLIENTINFO , receiveMessage<CubeJProtocol::MSG_SND_CLIENTINFO>);
        registerMsgHandler( CubeJProtocol::MSG_SND_SCENESTATUS , receiveMessage<CubeJProtocol::MSG_SND_SCENESTATUS>);
        registerMsgHandler( CubeJProtocol::MSG_ACK_REMOTE , receiveMessage<CubeJProtocol::MSG_ACK_REMOTE>);
        registerMsgHandler( CubeJProtocol::MSG_FWD_LISTMAPS , receiveMessage<CubeJProtocol::MSG_FWD_LISTMAPS>);
        registerMsgHandler( CubeJProtocol::MSG_SND_SCENEINFO , receiveMessage<CubeJProtocol::MSG_SND_SCENEINFO>);
    }

    RemoteClient::~RemoteClient()
    {
        disconnect();
    }

    void RemoteClient::registerSceneManager(SceneMgr* mgr) {
        ///TODO: if(scenemgr) notify changed
        scenemgr = mgr;
    }

    SceneMgr& RemoteClient::GetSceneManager() {
        ///TODO: assert
        return *scenemgr;
    }

    void RemoteClient::setServername(const char* sname) {
        ///TODO: check connected
        servername = sname;
    }

    void RemoteClient::setServerport(int port) {
        ///TODO: check connected
        address.port = port;
    }

    void RemoteClient::connectWithServer() {
        if(enet_address_set_host (&address, servername.c_str()) < 0 )
        {
            std::cout << "[DEBUG] RemoteClient::connectWithServer - could not resolve server " << servername;
            return;
        }

        host = enet_host_create(NULL, 2, numchannels, rate, rate);

        if(host)
        {
            std::cout << "[DEBUG] RemoteClient::connectWithServer - connecting to host: " << servername << ":" << address.port << std::endl;
            peer = enet_host_connect(host, &address, numchannels, 0);
            enet_host_flush(host);
        }
        else std::cout << "[DEBUG] RemoteClient::connectWithServer - could not connect to server";
    }

    void RemoteClient::connectWithServer(const char* sname, int port) {
        setServername(sname);
        setServerport(port);
        connectWithServer();
    }

    void RemoteClient::connectWithClient(int cn) {
        for (std::vector<CubeJ::ClientInfo*>::iterator it = clientcache.begin(); it < clientcache.end(); it++) {
            if((*it)->getclientnum() == cn) {
                if(head)
                    GetRemoteClient().disconnectClient();
                head = *it;
                ///TEST
                CubeJProtocol::MsgDataType<CubeJProtocol::MSG_REQ_LISTMAPS> data;
                SendMessage(data);
                return;
            }
        }
		std::cout << "[DEBUG] RemoteClient::connectWithClient: error - client " << cn << " not found in clientcache" << std::endl;
    }

    void RemoteClient::disconnectClient() {
        head = NULL;
    }

    void RemoteClient::disconnectClient(int cn) {
        for (std::vector<CubeJ::ClientInfo*>::iterator it = clientcache.begin(); it != clientcache.end(); it++)
        {
            if( (*it)->getclientnum() == cn) {
                if(head)
                    GetRemoteClient().disconnectClient();
                delete *it;
                clientcache.erase(it);
            }

        }
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
        for (std::vector<CubeJ::ClientInfo*>::iterator it = clientcache.begin(); it != clientcache.end(); it++) {
            if((*it)->getclientnum() == cn) {
                if(head == *it) {
                    disconnectClient();
                }
                delete *it;
                clientcache.erase(it);
            }
        }
        clientcache.push_back(new CubeJ::ClientInfo(cn, type, name));
	}

    void RemoteClient::disconnect() {
        std::cout << "[DEBUG] RemoteClient::disconnect"<< std::endl;
        if(peer) {
            enet_peer_disconnect(peer, DISC_NONE);
            enet_peer_reset (peer);
        }
        if(host)
            enet_host_flush(host);

        std::vector<CubeJ::ClientInfo*>::iterator it = clientcache.begin();
        for(; it < clientcache.end(); it++)
            delete *it;
        clientcache.clear();
    }

    RemoteClient& GetRemoteClient() {
        static RemoteClient remoteclient;
        return remoteclient;
    }
}
