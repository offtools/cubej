#include "protocol.hpp"
#include "client.hpp"
#include "server.hpp"

namespace CubeJProtocol
{
#ifndef STANDALONE

#endif

	template <MSG_TYPE N> void receiveMessage(int sender, int channel, packetbuf& p) {}

    template <> void receiveMessage<MSG_ERROR_OVERFLOW>(int sender, int channel, packetbuf& p) {}

	template <> void receiveMessage<MSG_ERROR_TAG>(int sender, int channel, packetbuf& p) {}

#ifndef STANDALONE
	template <> void receiveMessage<MSG_SND_SERVINFO>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        int protocol = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_SND_SERVINFO>");
        CubeJ::GetClient().initConnect(clientnum, protocol);
	}
#endif

    template <> void receiveMessage<MSG_REQ_CONNECT>(int sender, int channel, packetbuf& p) {
        conoutf("[DEBUG] receiveMessage<MSG_REQ_CONNECT>");
        char text[MAXTRANS];
        getstring(text, p);
        filtertext(text, text, false, MAXNAMELEN);
        MsgInfoType& info = GetMsgTypeInfo(MSG_REQ_CONNECT);
        if (info.channel == channel)
            CubeJSrv::GetServer().connectClient(sender, text);
    }

    template <> void receiveMessage<MSG_CDIS>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_CDIS>: %d", clientnum);
    }

#ifndef STANDALONE
    template <> void receiveMessage<MSG_SND_CLIENTINFO>(int sender, int channel, packetbuf& p) {
        int cn = getint(p);
        char text[MAXTRANS];
        getstring(text, p);
		CubeJ::GetClient().ackConnect(cn, text);
        conoutf("[DEBUG] receiveMessage<MSG_SND_CLIENTINFO>: %d %s", cn, text);
    }
#endif

    template <> void receiveMessage<MSG_SND_SCENESTATUS>(int sender, int channel, packetbuf& p) {
        int hasscene = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_SND_SCENESTATUS> hasscene: %d", hasscene);
    }

	static MsgInfoType typeinfo[NUM_MESSAGES] = {
		{ MSG_ERROR_OVERFLOW, "error_overflow", 1, 0, MSG_DIR_DROP, receiveMessage<MSG_ERROR_OVERFLOW> },
		{ MSG_ERROR_TAG, "error_tag", 1, 0, MSG_DIR_DROP, receiveMessage<MSG_ERROR_TAG> },
		{ MSG_SND_SERVINFO, "serverinfo", 1, ENET_PACKET_FLAG_RELIABLE, MSG_DIR_S2C, receiveMessage<MSG_SND_SERVINFO> },
        { MSG_REQ_CONNECT, "client_connect", 1, ENET_PACKET_FLAG_RELIABLE, MSG_DIR_S2C, receiveMessage<MSG_REQ_CONNECT> },
        { MSG_CDIS, "client_disconnect", 1, ENET_PACKET_FLAG_RELIABLE, MSG_DIR_S2C, receiveMessage<MSG_CDIS> },
        { MSG_SND_CLIENTINFO, "server_clientinfo", 1, ENET_PACKET_FLAG_RELIABLE, MSG_DIR_S2C, receiveMessage<MSG_SND_CLIENTINFO> },
        { MSG_SND_SCENESTATUS, "server_sceneinfo", 1, ENET_PACKET_FLAG_RELIABLE, MSG_DIR_S2C, receiveMessage<MSG_SND_SCENESTATUS> }
	};

    MsgInfoType& GetMsgTypeInfo(MSG_TYPE n) {
		return typeinfo[n];
	}

    MsgDataType<MSG_REQ_CONNECT>::MsgDataType(const char* text) : info(GetMsgTypeInfo(MSG_REQ_CONNECT)), name(text) {}

    void MsgDataType<MSG_REQ_CONNECT>::addmsg(packetbuf& p) {
        putint(p, info.id);
        sendstring(name, p);
    }

	void ReceiveMessage(MSG_TYPE n, int sender, int channel, packetbuf& p) {
	    if (n < NUM_MESSAGES) {
            typeinfo[n].receivehandler(sender, channel, p);
	    }
	}

    void ReceiveMessageAll(int sender, int channel, packetbuf& p) {
        MSG_TYPE type = (MSG_TYPE)getint(p);
        ReceiveMessage(type, sender, channel, p);
    }
}
