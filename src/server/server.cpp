#include "server.h"
#include "protocol.h"

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
        if( connected && remote.length() ) {
            packetbuf p(remote.length(), 0);
            p.put(remote.getbuf(), remote.length());
            remote.setsize(0);
            sendpacket(clientnum, CubeJProtocol::CHANNEL_REMOTE, p.finalize(), pairnum);
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
        registerMsgHandler( CubeJProtocol::MSG_REQ_LISTMAPS , receiveMessage<CubeJProtocol::MSG_REQ_LISTMAPS>);
	}

	Server::~Server() {}

	//--------Server: Network Message Handler------------//

	//~ void Server::registerMsgHandler(MSG_TYPE n, void (*func)(int, int, packetbuf&)) {
		//~ if( n < CubeJProtocol::NUM_MESSAGES )
			//~ receivehandler[n] = func;
	//~ }

	//~ void Server::receive(CubeJProtocol::MSG_TYPE n, int sender, int channel, packetbuf& p) {
	    //~ if (  n < CubeJProtocol::NUM_MESSAGES && receivehandler[n] &&  channel == CubeJProtocol::GetMsgTypeInfo(n).channel ) {
            //~ receivehandler[n](sender, channel, p);
	    //~ }
	    //~ else {
            //~ conoutf("[DEBUG] CubeJProtocol::ReceiveMessage - Message Type or Message Handler \"%s\" not found", CubeJProtocol::GetMsgTypeInfo(n).description);
            //~ receivehandler[n](sender, channel, p);
	    //~ }
	//~ }

	//~ void Server::receive(int sender, int channel, packetbuf& p) {
        //~ CubeJProtocol::MSG_TYPE type = (CubeJProtocol::MSG_TYPE)getint(p);
        //~ receive(type, sender, channel, p);
	//~ }

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
        CubeJProtocol::MSG_TYPE type;
		int curmsg;

        reliablemessages = p.packet->flags&ENET_PACKET_FLAG_RELIABLE ? true : false;

        SvClientInfo *ci = sender>=0 ? (SvClientInfo*)getclientinfo(sender) : NULL;

        while( (curmsg = p.length()) < p.maxlen)
        {
        	type = (CubeJProtocol::MSG_TYPE)getint(p);
            conoutf("[DEBUG] parsing Msg type: %d", type);
        	if(ci && !ci->connected) {
                if(chan == 0) {
                    return;
                }
				//validating should be handles by handlers itself, because they hold all the information about the message
                //~ if ( chan != 1 || ( type != CubeJProtocol::MSG_REQ_CONNECT && type != CubeJProtocol::MSG_REQ_REMOTE ) ) {
                if ( chan != CHANNEL_PRECONNECT ) {
					conoutf("[DEBUG] Disconnect: clientnum: %d, type %d", ci->getclientnum(), type);
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

    void Server::registerRemoteClient(int cn) {
        CubeJSrv::SvClientInfo *ci = (CubeJSrv::SvClientInfo*)getclientinfo(cn);
        if(!ci) {
            conoutf("[DEBUG] Server::registerRemoteClient - no clientinfo, abort connect");
            return;
        }

        ci->type = CubeJ::CLIENT_TYPE_REMOTE;

        MsgInfoType& info = CubeJProtocol::GetMsgTypeInfo(MSG_SND_CLIENTINFO);

        packetbuf p(MAXTRANS, info.flag);

        //send client infos of not connected (free) heads
        loopv(clients) {
            MsgDataType<MSG_SND_CLIENTINFO> data(clients[i]->clientnum, clients[i]->type, clients[i]->name);
            data.addmsg(p);
            conoutf("[DEBUG] Server::registerRemoteClient - send %d:%s", clients[i]->clientnum, clients[i]->name);
        }

        //GetSceneManager().sendscenestatus(p);
        sendpacket(ci->clientnum, info.channel, p.finalize());
    }

	void Server::connectRemoteClient(int head, int remote) {
        SvClientInfo *cihead = (SvClientInfo*)getclientinfo(head);
        SvClientInfo *ciremote = (SvClientInfo*)getclientinfo(remote);

		if(!cihead || !ciremote)
			return;

		ciremote->connected = true;
        ciremote->pairnum = cihead->clientnum;
        cihead->pairnum = ciremote->clientnum;

		MsgDataType<MSG_ACK_REMOTE> data(head);
        SendMessage(remote, data);
	}

    void Server::forwardMessage(int sender, int channel, packetbuf& p) {
        SvClientInfo *ci = (SvClientInfo*)getclientinfo(sender);
        SvClientInfo *cm = (SvClientInfo*)getclientinfo(ci->pairnum);
        conoutf("[DEBUG] Server::forwardMessage %d -> %d", ci->clientnum, cm->clientnum);
        int curmsg = 0;
        while(curmsg<p.length()) cm->remote.add(p.buf[curmsg++]);
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
