#include "server.h"

namespace CubeJSrv {
	using namespace CubeJProtocol;

    template <> void receiveMessage<MSG_ERROR_OVERFLOW>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<MSG_ERROR_TAG>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<MSG_REQ_CONNECT>(int sender, int channel, packetbuf& p) {
        conoutf("[DEBUG] receiveMessage<MSG_REQ_CONNECT>");
        MsgDataType<MSG_REQ_CONNECT> data(p);
		if (data.info.channel == channel)
            CubeJSrv::GetServer().registerClient(sender, data.name);
    }

    template <> void receiveMessage<MSG_DISCOVER_REMOTE>(int sender, int channel, packetbuf& p) {
        conoutf("[DEBUG] receiveMessage<MSG_DISCOVER_REMOTE> sender: %d, channel: %d", sender, channel);
        GetServer().registerRemoteClient(sender);
    }

    template <> void receiveMessage<MSG_REQ_REMOTE>(int sender, int channel, packetbuf& p) {
        MsgDataType<MSG_REQ_REMOTE> rcv(p);
        conoutf("[DEBUG] receiveMessage<MSG_REQ_REMOTE> remote: %d, client: %d", sender, rcv.clientnum);
        SvClientInfo *ci = (SvClientInfo*)getclientinfo(rcv.clientnum);
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

    template <> void receiveMessage<MSG_REQ_LISTMAPS>(int sender, int channel, packetbuf& p) {
		conoutf("[DEBUG] receiveMessage<MSG_REQ_LISTMAPS>");
		GetServer ().forwardMessage(sender, channel, p);
    }

    template <> void receiveMessage<MSG_SND_LISTMAPS>(int sender, int channel, packetbuf& p) {
		conoutf("[DEBUG] receiveMessage<MSG_SND_LISTMAPS>");
		GetServer ().forwardMessage(sender, channel, p);
		p.cleanup();
    }
}
