#ifndef CUBEJ_PROTOCOL_H_
#define CUBEJ_PROTOCOL_H_

#include "cube.h"

namespace CubeJProtocol
{
	static const int PROTOCOL_VERSION  = 1;
    static const int NUM_CHANNELS = 6;
    static const int MAXNAMELEN = 15;

	enum CHANNEL { CHANNEL_POSITIONS = 0, CHANNEL_PRECONNECT, CHANNEL_DEFAULT, CHANNEL_FILE, CHANNEL_REMOTE };

	enum MSG_TYPE {
		MSG_ERROR_OVERFLOW = 0, //0

		MSG_ERROR_TAG,          //1

		MSG_SND_SERVINFO,       //2

		MSG_REQ_CONNECT,        //3

		//remote client requests a list of free clients to connect with
		MSG_DISCOVER_REMOTE,    //4

        MSG_CDIS,               //5

        MSG_SND_CLIENTINFO,     //6

		//7: server runs an active scene or not
		MSG_SND_SCENESTATUS,

		//8:
		MSG_REQ_SCENEINFO,

        //9:
        MSG_SND_SCENEINFO,

		//10:
		MSG_REQ_CHANGESCENE,

		//Remote Gui Client sends request to an client
		MSG_REQ_REMOTE,

		//Client ack Remote Connection
		MSG_ACK_REMOTE,

        //request a listing of local maps from the client
        MSG_REQ_LISTMAPS,

        //client sends listing of avabiable maps over remote channel
        MSG_SND_LISTMAPS,

		MSG_PONG,

		MSG_PING,

		NUM_MESSAGES
	};

	typedef void (*ReceiveHandler)(int sender, int channel, packetbuf& p);

	struct MsgInfoType {
			const char* description;
			int channel;
			int flag;
			//~ ReceiveHandler receivehandler;
			//void (*receivehandler)(int sender, int channel, packetbuf& p);
	};

	struct MsgHandler {
		void registerMsgHandler(MSG_TYPE n, void (*func)(int, int, packetbuf&));
		void receive(MSG_TYPE n, int sender, int channel, packetbuf& p);
		void receive(int sender, int channel, packetbuf& p);
		CubeJProtocol::ReceiveHandler receivehandler[NUM_MESSAGES];
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

	template <> struct MsgDataType<MSG_DISCOVER_REMOTE> {
		MsgDataType();
		MsgInfoType& info;
		void addmsg(packetbuf& p);
	};

	template <> struct MsgDataType<MSG_CDIS> {
		MsgDataType(int i);
		MsgInfoType& info;
        int clientnum;
		void addmsg(packetbuf& p);
	};

	template <> struct MsgDataType<MSG_SND_CLIENTINFO> {
		MsgDataType(int i, int t, const char* text);
		MsgInfoType& info;
        int cn;
		int type;
        const char* name;
		void addmsg(packetbuf& p);
	};

	template <> struct MsgDataType<MSG_SND_SCENESTATUS> {
		MsgDataType(bool b);
		MsgInfoType& info;
        bool hasscene;
		void addmsg(packetbuf& p);
	};

	template <> struct MsgDataType<MSG_REQ_REMOTE> {
		MsgDataType(int clientnum);
		MsgInfoType& info;
        int clientnum;
		void addmsg(packetbuf& p);
	};

    template <> struct MsgDataType<MSG_ACK_REMOTE> {
        MsgDataType(int clientnum);
        MsgInfoType& info;
		int clientnum;
        void addmsg(packetbuf& p);
    };

    template <> struct MsgDataType<MSG_REQ_LISTMAPS> {
        MsgDataType();
        MsgInfoType& info;
        void addmsg(packetbuf& p);
    };

    template <> struct MsgDataType<MSG_SND_LISTMAPS> {
        MsgDataType(vector<char *> &files);
        MsgInfoType& info;
        vector<char *> &listing;
        void addmsg(packetbuf& p);
    };

	//~ void ReceiveMessage(MSG_TYPE n, int sender, int channel, packetbuf& p);
    //~ void ReceiveMessage(int sender, int channel, packetbuf& p);
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
