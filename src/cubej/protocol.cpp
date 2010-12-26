#include "protocol.h"

namespace CubeJProtocol
{

    ///TODO (__offtools__) this should be const
	static MsgInfoType typeinfo[NUM_MESSAGES];

    void registermsgtype(MSG_TYPE type, const char* descr, int channel, int flag) {
        if(type < NUM_MESSAGES) {
            typeinfo[type].description = descr;
            typeinfo[type].channel = channel;
            typeinfo[type].flag = flag;
            //~ typeinfo[type].receivehandler = NULL;
        }
    }

	void MsgHandler::registerMsgHandler(MSG_TYPE n, void (*func)(int, int, packetbuf&)) {
		if( n < NUM_MESSAGES )
			receivehandler[n] = func;
	}
	
	void MsgHandler::receive(MSG_TYPE n, int sender, int channel, packetbuf& p) {
	    if (  n < NUM_MESSAGES && receivehandler[n] &&  channel == GetMsgTypeInfo(n).channel ) {
            receivehandler[n](sender, channel, p);
	    }
	    else {
            conoutf("[DEBUG] CubeJProtocol::ReceiveMessage - Message Type or Message Handler \"%s\" not found", GetMsgTypeInfo(n).description);
            receivehandler[n](sender, channel, p);
	    }
	}
	
	void MsgHandler::receive(int sender, int channel, packetbuf& p) {
        MSG_TYPE type = (MSG_TYPE)getint(p);
        receive(type, sender, channel, p);
	}
	
    void Init() {
        conoutf("init: protocol");
        registermsgtype( MSG_ERROR_OVERFLOW, "error_overflow", CHANNEL_DEFAULT, 0);
        registermsgtype( MSG_ERROR_TAG, "error_tag", CHANNEL_DEFAULT, 0 );
		registermsgtype( MSG_SND_SERVINFO, "serverinfo", CHANNEL_DEFAULT, ENET_PACKET_FLAG_RELIABLE );
        registermsgtype( MSG_REQ_CONNECT, "client_connect", CHANNEL_PRECONNECT, ENET_PACKET_FLAG_RELIABLE );
        registermsgtype( MSG_DISCOVER_REMOTE, "remoteclient_connect", CHANNEL_PRECONNECT, ENET_PACKET_FLAG_RELIABLE );
        registermsgtype( MSG_CDIS, "client_disconnect", CHANNEL_PRECONNECT, ENET_PACKET_FLAG_RELIABLE );
        registermsgtype( MSG_SND_CLIENTINFO, "server_clientinfo", CHANNEL_DEFAULT, ENET_PACKET_FLAG_RELIABLE );
        registermsgtype( MSG_SND_SCENESTATUS, "server_scenestatus", CHANNEL_DEFAULT, ENET_PACKET_FLAG_RELIABLE );
		registermsgtype( MSG_REQ_REMOTE, "client_requestremote", CHANNEL_PRECONNECT, ENET_PACKET_FLAG_RELIABLE );
		registermsgtype( MSG_ACK_REMOTE, "client_ackremote", CHANNEL_PRECONNECT, ENET_PACKET_FLAG_RELIABLE );
    }

    MsgDataType<MSG_REQ_CONNECT>::MsgDataType(const char* text) : info(GetMsgTypeInfo(MSG_REQ_CONNECT)), name(text) {}

    void MsgDataType<MSG_REQ_CONNECT>::addmsg(packetbuf& p) {
        putint(p, MSG_REQ_CONNECT);
        sendstring(name, p);
    }

    MsgDataType<MSG_DISCOVER_REMOTE>::MsgDataType() : info(GetMsgTypeInfo(MSG_DISCOVER_REMOTE)){}

    void MsgDataType<MSG_DISCOVER_REMOTE>::addmsg(packetbuf& p) {
        putint(p, MSG_DISCOVER_REMOTE);
    }

    MsgDataType<MSG_SND_SERVINFO>::MsgDataType(int n, int p) : info(GetMsgTypeInfo(MSG_SND_SERVINFO)),  clientnum(n), protocol(p) {}

    void MsgDataType<MSG_SND_SERVINFO>::addmsg(packetbuf& p) {
        putint(p, MSG_SND_SERVINFO);
        putint(p, clientnum);
        putint(p, protocol);
    }

    MsgDataType<MSG_SND_CLIENTINFO>::MsgDataType(int i, const char* text) : info(GetMsgTypeInfo(MSG_SND_CLIENTINFO)), cn(i), name(text) {}

    void MsgDataType<MSG_SND_CLIENTINFO>::addmsg(packetbuf& p) {
        putint(p, MSG_SND_CLIENTINFO);
        putint(p, cn);
        sendstring(name, p);
    }

    MsgDataType<MSG_SND_SCENESTATUS>::MsgDataType(bool b) : info(GetMsgTypeInfo(MSG_SND_SCENESTATUS)), hasscene(b) {}

    void MsgDataType<MSG_SND_SCENESTATUS>::addmsg(packetbuf& p) {
        putint(p, MSG_SND_SCENESTATUS);
        putint(p, hasscene);
    }

    MsgDataType<MSG_REQ_REMOTE>::MsgDataType(int cn) : info(GetMsgTypeInfo(MSG_REQ_REMOTE)), clientnum(cn) {}

    void MsgDataType<MSG_REQ_REMOTE>::addmsg(packetbuf& p) {
        putint(p, MSG_REQ_REMOTE);
        putint(p, clientnum);
    }

    MsgDataType<MSG_ACK_REMOTE>::MsgDataType(int cn) : info(GetMsgTypeInfo(MSG_ACK_REMOTE)), clientnum(cn) {}

    void MsgDataType<MSG_ACK_REMOTE>::addmsg(packetbuf& p) {
        putint(p, MSG_ACK_REMOTE);
		putint(p, clientnum);
    }

    MsgInfoType& GetMsgTypeInfo(MSG_TYPE n) {
		return typeinfo[n];
	}

    //~ void RegisterMsgHandler(MSG_TYPE n, void (*func)(int, int, packetbuf&) ) {
        //~ if(!typeinfo[n].receivehandler)
            //~ typeinfo[n].receivehandler = func;
        //~ else
            //~ conoutf("[DEBUG] CubeJProtocol::RegisterMsgHandler - handler already registered");
    //~ }

	//~ void ReceiveMessage(MSG_TYPE n, int sender, int channel, packetbuf& p) {
	    //~ if (n < NUM_MESSAGES && typeinfo[n].receivehandler) {
            //~ typeinfo[n].receivehandler(sender, channel, p);
	    //~ }
	    //~ else {
            //~ conoutf("[DEBUG] CubeJProtocol::ReceiveMessage - Message Type or Message Handler \"%s\" not found", typeinfo[n].description);
            //~ typeinfo[MSG_ERROR_TAG].receivehandler(sender, channel, p);
	    //~ }
	//~ }

    //~ void ReceiveMessage(int sender, int channel, packetbuf& p) {
        //~ MSG_TYPE type = (MSG_TYPE)getint(p);
        //~ conoutf("ReceiveMessage: type %d, sender: %d", type, sender);
        //~ ReceiveMessage(type, sender, channel, p);
    //~ }
}
