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
        conoutf("[DEBUG] receiveMessage<MSG_SND_SERVINFO>");
        char text[MAXTRANS];
        getstring(text, p);
        MsgInfoType& info = GetMsgTypeInfo(MSG_REQ_CONNECT);
        if (info.channel == channel)
            CubeJSrv::GetServer().Connect(sender, text);
    }

    template <> void receiveMessage<MSG_CDIS>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_CDIS>: %d", clientnum);
    }

	static MsgInfoType typeinfo[NUM_MESSAGES] = {
		{ MSG_ERROR_OVERFLOW, "error_overflow", 1, 0, MSG_DIR_DROP, receiveMessage<MSG_ERROR_OVERFLOW> },
		{ MSG_ERROR_TAG, "error_tag", 1, 0, MSG_DIR_DROP, receiveMessage<MSG_ERROR_TAG> },
		{ MSG_SND_SERVINFO, "serverinfo", 1, ENET_PACKET_FLAG_RELIABLE, MSG_DIR_S2C, receiveMessage<MSG_SND_SERVINFO> },
        { MSG_REQ_CONNECT, "client_connect", 1, ENET_PACKET_FLAG_RELIABLE, MSG_DIR_C2S, receiveMessage<MSG_REQ_CONNECT> },
        { MSG_CDIS, "client_disconnect", 1, ENET_PACKET_FLAG_RELIABLE, MSG_DIR_S2C, receiveMessage<MSG_CDIS> }
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
}
