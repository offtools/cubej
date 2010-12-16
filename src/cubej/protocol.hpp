#ifndef CUBEJ_PROTOCOL_H_
#define CUBEJ_PROTOCOL_H_

#include "cube.h"

namespace CubeJProtocol
{
	static const int PROTOCOL_VERSION  = 1;
    static const int MAXNAMELEN = 15;
	enum MSG_DIR {
		MSG_DIR_S2C = 0,
		MSG_DIR_C2S,
		MSG_DIR_C2C,
		MSG_DIR_DROP
	};

	enum MSG_TYPE {
		MSG_ERROR_OVERFLOW = 0,
		MSG_ERROR_TAG,
		MSG_SND_SERVINFO,
		MSG_REQ_CONNECT,
        MSG_CDIS,
		MSG_SND_SRVSTATUS,
		MSG_SND_CLIENTSTATUS,
		MSG_SND_SCENESTATUS,
		MSG_PONG,
		MSG_PING,
		NUM_MESSAGES
	};

	struct MsgInfoType {
			const MSG_TYPE id;
			const char* description;
			const int channel;
			const int flag;
			const int dir;
			void (*receivehandler)(int sender, int channel, packetbuf& p);
	};

	MsgInfoType& GetMsgTypeInfo(MSG_TYPE n);

    template <MSG_TYPE N> struct MsgDataType {
		MsgDataType() : info(GetMsgTypeInfo(N)) {}
		MsgInfoType& info;
		void addmsg(packetbuf& p);
	};

	template <> struct MsgDataType<MSG_SND_SERVINFO> {
		MsgDataType(int n, int p) : info(GetMsgTypeInfo(MSG_SND_SERVINFO)),  clientnum(n), protocol(p) {}
		MsgInfoType& info;
		int clientnum;
		int protocol;

		void addmsg(packetbuf& p) {
            putint(p, info.id);
            putint(p, clientnum);
            putint(p, protocol);
		}
	};

	template <> struct MsgDataType<MSG_REQ_CONNECT> {
		MsgDataType(const char* text);
		MsgInfoType& info;
        const char* name;
		void addmsg(packetbuf& p);
	};

	template <> struct MsgDataType<MSG_CDIS> {
		MsgDataType(int i) : info(GetMsgTypeInfo(MSG_CDIS)), cn(i) {}
		MsgInfoType& info;
        int cn;

		void addmsg(packetbuf& p) {
            putint(p, info.id);
            putint(p, cn);
		}
	};

	void ReceiveMessage(MSG_TYPE n, int sender, int channel, packetbuf& p);
}

namespace CubeJ
{
    using namespace CubeJProtocol;
    template <CubeJProtocol::MSG_TYPE N> void SendMessage(CubeJProtocol::MsgDataType<N>& data) {
        packetbuf p(MAXTRANS, data.info.flag);
        data.addmsg(p);
		sendclientpacket(p.finalize(), data.info.channel);
	}
}

namespace CubeJSrv
{
    using namespace CubeJProtocol;
    template <MSG_TYPE N> void SendMessage(int cn, MsgDataType<N>& data, int exclude=-1) {
        packetbuf p(MAXTRANS, data.info.flag);
        data.addmsg(p);
		ENetPacket *packet = p.finalize();
		sendpacket(cn, data.info.channel, packet, exclude);
	}
}
#endif
