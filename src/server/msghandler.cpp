#include "server.h"

namespace CubeJSrv {
	using namespace CubeJProtocol;

    template <> void receiveMessage<MSG_ERROR_OVERFLOW>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<MSG_ERROR_TAG>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<MSG_REQ_CONNECT>(int sender, int channel, packetbuf& p) {
        conoutf("[DEBUG] receiveMessage<MSG_REQ_CONNECT>");
		char text[MAXTRANS];
        getstring(text, p);
        filtertext(text, text, false, MAXNAMELEN);

		MsgInfoType& info = GetMsgTypeInfo(MSG_REQ_CONNECT);

		if (info.channel == channel)
            CubeJSrv::GetServer().registerClient(sender, text);
    }

    template <> void receiveMessage<MSG_DISCOVER_REMOTE>(int sender, int channel, packetbuf& p) {
        conoutf("[DEBUG] receiveMessage<MSG_DISCOVER_REMOTE> sender: %d, channel: %d", sender, channel);
        GetServer().registerRemoteClient(sender);
    }

    template <> void receiveMessage<MSG_REQ_REMOTE>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_REQ_REMOTE> remote: %d, client: %d", sender, clientnum);
        SvClientInfo *ci = (SvClientInfo*)getclientinfo(clientnum);
        if(!ci)
            return;
        MsgDataType<MSG_REQ_REMOTE> data(sender);
        SendMessage(ci->getclientnum(), data);
    }

	template <> void receiveMessage<MSG_ACK_REMOTE>(int sender, int channel, packetbuf& p) {
        int remote = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_ACK_REMOTE> client: %d, remote: %d", sender, remote);
		GetServer ().connectRemoteClient(sender, remote);
    }
}
