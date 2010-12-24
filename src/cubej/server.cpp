#include "server.h"
#include "protocol.h"

namespace CubeJSrv {
	using namespace CubeJProtocol;

    //-------------Message Handler-----------------//
	template <MSG_TYPE N> void receiveMessage(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<MSG_ERROR_OVERFLOW>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<MSG_ERROR_TAG>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<MSG_REQ_CONNECT>(int sender, int channel, packetbuf& p) {
        conoutf("[DEBUG] receiveMessage<MSG_REQ_CONNECT>");
        char text[MAXTRANS];
        getstring(text, p);
        filtertext(text, text, false, MAXNAMELEN);
        MsgInfoType& info = GetMsgTypeInfo(MSG_REQ_CONNECT);
        conoutf("[DEBUG] info.channel: %d, channel: %d", info.channel, channel);
        //if (info.channel == channel)
            CubeJSrv::GetServer().connectClient(sender, text);
    }

    template <> void receiveMessage<MSG_REQ_REMOTE>(int sender, int channel, packetbuf& p) {
        conoutf("[DEBUG] receiveMessage<MSG_REQ_REMOTE> sender: %d, channel: %d", sender, channel);
        GetServer().connectRemoteClient(sender);
    }

    //--------Client Info Implementation------------//

	SvClientInfo::SvClientInfo() : connectmillis(0), connected(false), local(false) {}

	SvClientInfo::~SvClientInfo() {}

//	void SvClientInfo::setName(char* text) {
//		if(!text[0]) copystring(text, "unnamed");
//		copystring(name, text, CubeJSrv::MAXNAMELEN+1);
//	}

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
		//update[status]();
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

    void SceneManager::sendsceneinfo(packetbuf& p) {
        MsgDataType<MSG_SND_SCENESTATUS> data(hasscene, mapname);
        data.addmsg(p);
    }

    void SceneManager::reset() {
        hasscene = false;
        mapname[0] = '\0';
    }

    //--------Server Implementation------------//

	Server::Server() : reliablemessages(false), lastsend(0), scenemillis(0) {}

	Server::~Server() {}

	void Server::Init() {
        conoutf("[DEBUG] Server::Init");
#ifndef STANDALONE
	    CubeJProtocol::Init();
#endif
        CubeJProtocol::RegisterMsgHandler( MSG_ERROR_OVERFLOW , receiveMessage<MSG_ERROR_OVERFLOW>);
        CubeJProtocol::RegisterMsgHandler( MSG_ERROR_TAG , receiveMessage<MSG_ERROR_TAG>);
        CubeJProtocol::RegisterMsgHandler( MSG_REQ_CONNECT , receiveMessage<MSG_REQ_CONNECT>);
        CubeJProtocol::RegisterMsgHandler( MSG_REQ_REMOTE , receiveMessage<MSG_REQ_REMOTE>);
        GetSceneManager().reset();
	}

	void Server::Update() {
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
        MsgDataType<MSG_SND_SERVINFO> data(ci->clientnum, PROTOCOL_VERSION);
        SendMessage(ci->clientnum, data);
		return DISC_NONE;
	}

	void Server::notifyDisconnect(int n) {
		//remove hooked actions (demoplayback ...)
		SvClientInfo *ci = (SvClientInfo*)getclientinfo(n);
        if(ci->connected)
        {
            MsgDataType<MSG_CDIS> data(n);
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
        	if(ci && !ci->connected) {
                if(chan == 0) {
                    return;
                }
                if(chan != 1 || ( type != CubeJProtocol::MSG_REQ_CONNECT && type != CubeJProtocol::MSG_REQ_REMOTE )) {
                    disconnect_client(sender, DISC_TAGT); return;
                }
        	}

        	//parse all messages
            CubeJProtocol::ReceiveMessage(type, sender, chan, p);
        }
	}

    void Server::connectClient(int cn, char* text) {
        CubeJSrv::SvClientInfo *ci = (CubeJSrv::SvClientInfo*)getclientinfo(cn);
        connecting.removeobj(ci);
        clients.add(ci);
        ci->type = CubeJ::CLIENT_TYPE_HEAD;
        ci->connected = true;
        ci->setName(text);

        MsgInfoType& info = CubeJProtocol::GetMsgTypeInfo(MSG_SND_CLIENTINFO);
        packetbuf p(MAXTRANS, info.flag);

        //send all client infos
        loopv(clients) {
            MsgDataType<MSG_SND_CLIENTINFO> data(clients[i]->clientnum, clients[i]->name);
            data.addmsg(p);
        }

        //send scene info
        //GetSceneManager().sendsceneinfo(p);

        sendpacket(ci->clientnum, info.channel, p.finalize());
    }

    void Server::connectRemoteClient(int cn) {
        CubeJSrv::SvClientInfo *ci = (CubeJSrv::SvClientInfo*)getclientinfo(cn);
        if(!ci) {
            conoutf("[DEBUG] Server::connectRemoteClient - no clientinfo, abort connect");
            return;
        }

        ci->type = CubeJ::CLIENT_TYPE_REMOTE;

        MsgInfoType& info = CubeJProtocol::GetMsgTypeInfo(MSG_SND_CLIENTINFO);

        packetbuf p(MAXTRANS, info.flag);

        //send all client infos
        loopv(clients) { //if(clients[i]->type == SDL_HEAD) {
            MsgDataType<MSG_SND_CLIENTINFO> data(clients[i]->clientnum, clients[i]->name);
            data.addmsg(p);
            conoutf("[DEBUG] Server::connectRemoteClient - send %d:%s", clients[i]->clientnum, clients[i]->name);
        }

        sendpacket(ci->clientnum, info.channel, p.finalize());
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
