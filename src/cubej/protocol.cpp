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
            receivehandler[MSG_ERROR_TAG](sender, channel, p);
	    }
	}

	void MsgHandler::receive(int sender, int channel, packetbuf& p) {
        MSG_TYPE type = (MSG_TYPE)getint(p);
        receive(type, sender, channel, p);
	}

    void Init() {
        conoutf("init: protocol");
        registermsgtype( MSG_ERROR_OVERFLOW,    "MSG_ERROR_OVERFLOW",   CHANNEL_DEFAULT,        0);
        registermsgtype( MSG_ERROR_TAG,         "MSG_ERROR_TAG",        CHANNEL_DEFAULT,        0 );
		registermsgtype( MSG_SND_SERVINFO,      "MSG_SND_SERVINFO",     CHANNEL_DEFAULT,        ENET_PACKET_FLAG_RELIABLE );
        registermsgtype( MSG_REQ_CONNECT,       "MSG_REQ_CONNECT",      CHANNEL_PRECONNECT,     ENET_PACKET_FLAG_RELIABLE );
        registermsgtype( MSG_DISCOVER_REMOTE,   "MSG_DISCOVER_REMOTE",  CHANNEL_PRECONNECT,     ENET_PACKET_FLAG_RELIABLE );
        registermsgtype( MSG_CDIS,              "MSG_CDIS",             CHANNEL_PRECONNECT,     ENET_PACKET_FLAG_RELIABLE );
		registermsgtype( MSG_REQ_REMOTE,        "MSG_REQ_REMOTE",       CHANNEL_PRECONNECT,     ENET_PACKET_FLAG_RELIABLE );
		registermsgtype( MSG_ACK_REMOTE,        "MSG_ACK_REMOTE",       CHANNEL_PRECONNECT,     ENET_PACKET_FLAG_RELIABLE );
        registermsgtype( MSG_SND_CLIENTINFO,    "MSG_SND_CLIENTINFO",   CHANNEL_DEFAULT,        ENET_PACKET_FLAG_RELIABLE );
        registermsgtype( MSG_SND_SCENESTATUS,   "MSG_SND_SCENESTATUS",  CHANNEL_DEFAULT,        ENET_PACKET_FLAG_RELIABLE );
		registermsgtype( MSG_REQ_LISTMAPS,      "MSG_REQ_LISTMAPS",     CHANNEL_REMOTE,         ENET_PACKET_FLAG_RELIABLE );
		registermsgtype( MSG_SND_LISTMAPS,      "MSG_SND_LISTMAPS",     CHANNEL_REMOTE,         ENET_PACKET_FLAG_RELIABLE );
    }

    MsgInfoType& GetMsgTypeInfo(MSG_TYPE n) {
		return typeinfo[n];
	}

    MsgDataType<MSG_REQ_CONNECT>::MsgDataType(const char* text) : info(GetMsgTypeInfo(MSG_REQ_CONNECT)) {
        copystring(name, text, MAXNAMELEN);
    }

    MsgDataType<MSG_REQ_CONNECT>::MsgDataType(packetbuf& p) : info(GetMsgTypeInfo(MSG_REQ_CONNECT)) {
        getstring(name, p);
        filtertext(name, name, false, MAXNAMELEN);
    }

    void MsgDataType<MSG_REQ_CONNECT>::addmsg(packetbuf& p) {
        putint(p, MSG_REQ_CONNECT);
        sendstring(name, p);
    }

    MsgDataType<MSG_DISCOVER_REMOTE>::MsgDataType() : info(GetMsgTypeInfo(MSG_DISCOVER_REMOTE)){}

    void MsgDataType<MSG_DISCOVER_REMOTE>::addmsg(packetbuf& p) {
        putint(p, MSG_DISCOVER_REMOTE);
    }

    MsgDataType<MSG_SND_SERVINFO>::MsgDataType(int n, int p) : info(GetMsgTypeInfo(MSG_SND_SERVINFO)), clientnum(n), protocol(p) {}
    MsgDataType<MSG_SND_SERVINFO>::MsgDataType(packetbuf& p) : info(GetMsgTypeInfo(MSG_SND_SERVINFO)), clientnum(getint(p)), protocol(getint(p)) {}

    void MsgDataType<MSG_SND_SERVINFO>::addmsg(packetbuf& p) {
        putint(p, MSG_SND_SERVINFO);
        putint(p, clientnum);
        putint(p, protocol);
    }

    MsgDataType<MSG_CDIS>::MsgDataType(int cn) : info(GetMsgTypeInfo(MSG_CDIS)), clientnum(cn) {}

    void MsgDataType<MSG_CDIS>::addmsg(packetbuf& p) {
        putint(p, MSG_CDIS);
        putint(p, clientnum);
    }

    MsgDataType<MSG_SND_CLIENTINFO>::MsgDataType(int i, int t, const char* text) : info(GetMsgTypeInfo(MSG_SND_CLIENTINFO)), cn(i), type(t), name(text) {}

    void MsgDataType<MSG_SND_CLIENTINFO>::addmsg(packetbuf& p) {
        putint(p, MSG_SND_CLIENTINFO);
        putint(p, cn);
		putint(p, type);
        sendstring(name, p);
    }

    MsgDataType<MSG_SND_SCENESTATUS>::MsgDataType(bool b) : info(GetMsgTypeInfo(MSG_SND_SCENESTATUS)), hasscene(b) {}

    void MsgDataType<MSG_SND_SCENESTATUS>::addmsg(packetbuf& p) {
        putint(p, MSG_SND_SCENESTATUS);
        putint(p, hasscene);
    }

    MsgDataType<MSG_REQ_REMOTE>::MsgDataType(int cn) : info(GetMsgTypeInfo(MSG_REQ_REMOTE)), clientnum(cn) {}
    MsgDataType<MSG_REQ_REMOTE>::MsgDataType(packetbuf& p) : info(GetMsgTypeInfo(MSG_REQ_REMOTE)), clientnum(getint(p)) {}

    void MsgDataType<MSG_REQ_REMOTE>::addmsg(packetbuf& p) {
        putint(p, MSG_REQ_REMOTE);
        putint(p, clientnum);
    }

    MsgDataType<MSG_ACK_REMOTE>::MsgDataType(int cn) : info(GetMsgTypeInfo(MSG_ACK_REMOTE)), clientnum(cn) {}

    void MsgDataType<MSG_ACK_REMOTE>::addmsg(packetbuf& p) {
        putint(p, MSG_ACK_REMOTE);
		putint(p, clientnum);
    }

    MsgDataType<MSG_REQ_LISTMAPS>::MsgDataType() : info(GetMsgTypeInfo(MSG_REQ_LISTMAPS)) {}

    void MsgDataType<MSG_REQ_LISTMAPS>::addmsg(packetbuf& p) {
        putint(p, MSG_REQ_LISTMAPS);
    }

    MsgDataType<MSG_SND_LISTMAPS>::MsgDataType(vector<char *> &files) : info(GetMsgTypeInfo(MSG_SND_LISTMAPS)), listing(files) {
        length = files.length();
    }

    void MsgDataType<MSG_SND_LISTMAPS>::addmsg(packetbuf& p) {
        putint(p, MSG_SND_LISTMAPS);
        putint(p, length);
        loopv(listing) {
            sendstring(listing[i], p);
        }
    }
}
