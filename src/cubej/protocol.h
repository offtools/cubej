#ifndef CUBEJ_PROTOCOL_H_
#define CUBEJ_PROTOCOL_H_

#include "cube.h"

namespace CubeJProtocol
{
	static const int PROTOCOL_VERSION  = 1;
    static const int MAXNAMELEN = 15;

	enum MSG_TYPE {
		MSG_ERROR_OVERFLOW = 0, //0
		MSG_ERROR_TAG,          //1
		MSG_SND_SERVINFO,       //2
		MSG_REQ_CONNECT,        //3
        MSG_REQ_REMOTE,         //4
        MSG_CDIS,               //5
        MSG_SND_CLIENTINFO,     //6
		MSG_SND_SCENESTATUS,
		MSG_REQ_SCENEINFO,
        MSG_SND_SCENEINFO,
		MSG_REQ_CHANGESCENE,
		MSG_PONG,
		MSG_PING,
		NUM_MESSAGES
	};

	struct MsgInfoType {
			const MSG_TYPE id;
			const char* description;
			int channel;
			int flag;
			void (*receivehandler)(int sender, int channel, packetbuf& p);
	};

    void Init();
	MsgInfoType& GetMsgTypeInfo(MSG_TYPE n);
    void RegisterMsgHandler(MSG_TYPE n, void (*func)(int, int, packetbuf&));

    template <MSG_TYPE N> struct MsgDataType {
		MsgDataType() : info(GetMsgTypeInfo(N)) {}
		MsgInfoType& info;
		void addmsg(packetbuf& p);
	};

	template <> struct MsgDataType<MSG_SND_SERVINFO> {
		MsgDataType(int n, int p);
		MsgInfoType& info;
		int clientnum;
		int protocol;
		void addmsg(packetbuf& p);
	};

	template <> struct MsgDataType<MSG_REQ_CONNECT> {
		MsgDataType(const char* text);
		MsgInfoType& info;
        const char* name;
		void addmsg(packetbuf& p);
	};

	template <> struct MsgDataType<MSG_REQ_REMOTE> {
		MsgDataType();
		MsgInfoType& info;
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

	template <> struct MsgDataType<MSG_SND_CLIENTINFO> {
		MsgDataType(int i, const char* text);
		MsgInfoType& info;
        int cn;
        const char* name;
		void addmsg(packetbuf& p);
	};

	template <> struct MsgDataType<MSG_SND_SCENESTATUS> {
		MsgDataType(bool b, const char* s) : info(GetMsgTypeInfo(MSG_SND_SCENESTATUS)), hasscene(b) {}
		MsgInfoType& info;
        bool hasscene;

		void addmsg(packetbuf& p) {
            putint(p, info.id);
            putint(p, hasscene);
		}
	};

	void ReceiveMessage(MSG_TYPE n, int sender, int channel, packetbuf& p);
    void ReceiveMessage(int sender, int channel, packetbuf& p);
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
