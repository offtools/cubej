#include "client.h"

namespace CubeJ {
	using namespace CubeJProtocol;	
	
    template <> void receiveMessage<MSG_ERROR_OVERFLOW>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<MSG_ERROR_TAG>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<MSG_SND_SERVINFO>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        int protocol = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_SND_SERVINFO>");
        CubeJ::GetClient().initConnect(clientnum, protocol);
	}

    template <> void receiveMessage<MSG_REQ_CONNECT>(int sender, int channel, packetbuf& p) {
        conoutf("[DEBUG] receiveMessage<MSG_REQ_CONNECT>");
        char text[MAXTRANS];
        getstring(text, p);
        filtertext(text, text, false, MAXNAMELEN);
        conoutf("[DEBUG] received by client - error %d:%s", sender, text);
    }

    template <> void receiveMessage<MSG_CDIS>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_CDIS>: %d", clientnum);
    }

    template <> void receiveMessage<MSG_SND_CLIENTINFO>(int sender, int channel, packetbuf& p) {
        int cn = getint(p);
        char text[MAXTRANS];
        getstring(text, p);
		CubeJ::GetClient().ackConnect(cn, text);
        conoutf("[DEBUG] receiveMessage<MSG_SND_CLIENTINFO>: %d %s", cn, text);
    }

    template <> void receiveMessage<MSG_SND_SCENESTATUS>(int sender, int channel, packetbuf& p) {
        int hasscene = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_SND_SCENESTATUS> hasscene: %d", hasscene);
		conoutf("[TODO] get scene from server or upload running scene");
    }

    template <> void receiveMessage<MSG_REQ_REMOTE>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_REQ_REMOTE> sender: %d, remote client: %d", sender, clientnum);
		GetClient().connectRemoteClient(clientnum);
    }
}
