#include "server.h"
#include "protocol.h"

///TODO: remote not in clients

namespace CubeJSrv {

	using namespace CubeJProtocol;

    //--------Client Info Implementation------------//

	SvClientInfo::SvClientInfo() : connectmillis(0), connected(false), pairnum(-1), local(false) {}

	SvClientInfo::~SvClientInfo() {}

	bool SvClientInfo::isConnected() {
		return connected;
	}

	void SvClientInfo::Init(int n, bool local) {
        clientnum = n;
        connectmillis = totalmillis;
        local = local;
        connected = false;
	}

	void SvClientInfo::Update() {
		//send remote messages
        if( isConnected() && remote.length() ) {
            conoutf("[DEBUG] SvClientInfo::Update len: %d", remote.length());
            packetbuf p(remote.length(), ENET_PACKET_FLAG_RELIABLE);
            p.put(remote.getbuf(), remote.length());
            remote.setsize(0);
            conoutf("[DEBUG] SvClientInfo::Update len: %d", remote.length());
            sendpacket(clientnum, CubeJProtocol::CHANNEL_REMOTE, p.finalize());
	    }
	}

    //--------Scene Manager Implementation------------//

    SceneManager::SceneManager() {
        reset();
    }

    SceneManager::~SceneManager() {
        reset();
    }

    int SceneManager::newobject(int type, int owner) { return 0; }

    int SceneManager::deleteobject(int id) { return 0; }

    void SceneManager::sendscenestatus(packetbuf& p) {
        MsgDataType<MSG_SND_SCENESTATUS> data(hasscene);
        data.addmsg(p);
    }

    void SceneManager::changeScene(const char* name) {
        conoutf("[DEBUG] SceneManager::changeScene: %s", name);
        if(name && name[0]) {
            hasscene = true;
            copystring(mapname, name);
            CubeJProtocol::MsgDataType<CubeJProtocol::MSG_CMD_CHANGESCENE> data(mapname);
            SendMessage(-1, data);
        }
    }

    void SceneManager::reset() {
        hasscene = false;
        mapname[0] = '\0';
    }

    //--------Server Implementation------------//

	Server::Server() : reliablemessages(false), lastsend(0), scenemillis(0) {
        registerMsgHandler( CubeJProtocol::MSG_ERROR_OVERFLOW , receiveMessage<CubeJProtocol::MSG_ERROR_OVERFLOW>);
        registerMsgHandler( CubeJProtocol::MSG_ERROR_TAG , receiveMessage<CubeJProtocol::MSG_ERROR_TAG>);
        registerMsgHandler( CubeJProtocol::MSG_REQ_CONNECT , receiveMessage<CubeJProtocol::MSG_REQ_CONNECT>);
        registerMsgHandler( CubeJProtocol::MSG_DISCOVER_REMOTE , receiveMessage<CubeJProtocol::MSG_DISCOVER_REMOTE>);
        registerMsgHandler( CubeJProtocol::MSG_REQ_REMOTE , receiveMessage<CubeJProtocol::MSG_REQ_REMOTE>);
        registerMsgHandler( CubeJProtocol::MSG_ACK_REMOTE , receiveMessage<CubeJProtocol::MSG_ACK_REMOTE>);
        registerMsgHandler( CubeJProtocol::MSG_REQ_CHANGESCENE , receiveMessage<CubeJProtocol::MSG_REQ_CHANGESCENE>);
	}

	Server::~Server() {}

	void Server::Init() {
        conoutf("[DEBUG] Server::Init");
		GetSceneManager().reset();
	}

	void Server::Update() {
	    loopv(clients) clients[i]->Update();

		scenemillis += curtime;
		loopv(clients) if(!clients[i]->connected && totalmillis-clients[i]->connectmillis>15000) disconnect_client(connecting[i]->clientnum, DISC_TIMEOUT);
	}

	int Server::numClients() {
		return clients.length();
	}

	int Server::sendServerInfo(int n, bool local) {
	    conoutf("[DEBUG] Server::sendServerInfo");
        SvClientInfo *ci = (SvClientInfo *)getclientinfo(n);
		ci->Init(n, local);
        connecting.add(ci);
        MsgDataType<CubeJProtocol::MSG_SND_SERVINFO> data(ci->clientnum, CubeJProtocol::PROTOCOL_VERSION);
        SendMessage(ci->clientnum, data);
		return DISC_NONE;
	}

	void Server::notifyDisconnect(int n) {
		//remove hooked actions (demoplayback ...)
		SvClientInfo *ci = (SvClientInfo*)getclientinfo(n);
        if(ci->connected)
        {
            MsgDataType<CubeJProtocol::MSG_CDIS> data(n);
            SendMessage(ci->clientnum, data);
        }
        clients.removeobj(ci);
	}

	bool Server::sendAllPackets(bool force) {
		if( clients.empty() ) return false;
        enet_uint32 curtime = enet_time_get()-lastsend;
        if(curtime<33 && !force) return false;
//        bool flush = buildworldstate();
        lastsend += curtime - (curtime%33);
        return true;
	}

	void Server::parsePacket(int sender, int chan, packetbuf &p) {
        if(sender<0) return;

        reliablemessages = p.packet->flags&ENET_PACKET_FLAG_RELIABLE ? true : false;

        SvClientInfo *ci = sender>=0 ? (SvClientInfo*)getclientinfo(sender) : NULL;

        if(chan == CHANNEL_REMOTE) {
            forwardMessage(ci->pairnum, chan, p);
        }

        while( p.remaining() )
        {
            CubeJProtocol::MSG_TYPE type = (CubeJProtocol::MSG_TYPE)getint(p);

            if(ci && !ci->connected) {
                if(chan == 0) {
                    return;
                }
				//validating should be handles by handlers itself, because they hold all the information about the message
                //~ if ( chan != 1 || ( type != CubeJProtocol::MSG_REQ_CONNECT && type != CubeJProtocol::MSG_REQ_REMOTE ) ) {
                if ( chan != CHANNEL_PRECONNECT ) {
                    disconnect_client(sender, DISC_TAGT); return;
                }
        	}

        	//parse all messages
            receive(type, sender, chan, p);
        }
	}

    void Server::registerClient(int cn, char* text) {
        CubeJSrv::SvClientInfo *ci = (CubeJSrv::SvClientInfo*)getclientinfo(cn);
        connecting.removeobj(ci);
        clients.add(ci);
        ci->type = CubeJ::CLIENT_TYPE_HEAD;
        ci->connected = true;
        ci->setName(text);

        MsgInfoType& info = CubeJProtocol::GetMsgTypeInfo(MSG_SND_CLIENTINFO);
        packetbuf p(MAXTRANS, info.flag);

        //send all client infos
        loopv(clients) if(clients[i]->type == CubeJ::CLIENT_TYPE_HEAD  &&  !clients[i]->isConnected() ) {
            MsgDataType<MSG_SND_CLIENTINFO> data(clients[i]->clientnum, clients[i]->type, clients[i]->name);
            data.addmsg(p);
        }

        //send scene status
        GetSceneManager().sendscenestatus(p);
        sendpacket(ci->clientnum, info.channel, p.finalize());
    }

    void Server::registerRemoteInterface(int cn) {
        CubeJSrv::SvClientInfo *ci = (CubeJSrv::SvClientInfo*)getclientinfo(cn);
        if(!ci) {
            conoutf("[DEBUG] Server::registerRemoteInterface - no clientinfo, abort connect");
            return;
        }

        ci->type = CubeJ::CLIENT_TYPE_REMOTE;

        MsgInfoType& info = CubeJProtocol::GetMsgTypeInfo(MSG_SND_CLIENTINFO);

        packetbuf p(MAXTRANS, info.flag);

        //send client infos of not connected (free) heads
        loopv(clients) {
            MsgDataType<MSG_SND_CLIENTINFO> data(clients[i]->clientnum, clients[i]->type, clients[i]->name);
            data.addmsg(p);
            conoutf("[DEBUG] Server::registerRemoteInterface - send %d:%s", clients[i]->clientnum, clients[i]->name);
        }

        //GetSceneManager().sendscenestatus(p);
        sendpacket(ci->clientnum, info.channel, p.finalize());
    }

	void Server::connectRemoteInterface(int head, int remote) {
        SvClientInfo *cihead = (SvClientInfo*)getclientinfo(head);
        SvClientInfo *ciremote = (SvClientInfo*)getclientinfo(remote);

		if(!cihead || !ciremote)
			return;

		ciremote->connected = true;
        connecting.removeobj(ciremote);
        clients.add(ciremote);
        ciremote->pairnum = cihead->clientnum;
        cihead->pairnum = ciremote->clientnum;

		MsgDataType<MSG_ACK_REMOTE> data(head);
        SendMessage(remote, data);
	}

    void Server::forwardMessage(int cn, int channel, packetbuf& p) {
        SvClientInfo *cm = (SvClientInfo*)getclientinfo(cn);
        int curmsg = 0;
        while(curmsg<p.remaining()) {
            cm->remote.add(p.buf[curmsg++]);
        }
        p.len = p.maxlen;
//        cm->remote.addbuf(p.subbuf(p.remaining()));
    }

	Server& GetServer() {
		static Server server;
		return server;
	}

	SceneManager& GetSceneManager() {
		static SceneManager mgr;
		return mgr;
	}
}
