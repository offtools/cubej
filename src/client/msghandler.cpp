#include "client.h"

namespace CubeJ {
	using namespace CubeJ;

    ///TODO: do not use p.cleanup, use subbuf instead
    template <> void receiveMessage<MSG_ERROR_OVERFLOW>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<MSG_ERROR_TAG>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<MSG_SND_SERVINFO>(int sender, int channel, packetbuf& p) {
        conoutf("[DEBUG] receiveMessage<MSG_SND_SERVINFO>");
        MsgDataType<MSG_SND_SERVINFO> data(p);
        CubeJ::GetClient().initConnect(data.clientnum, data.protocol);
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
		int type = getint(p);
        char text[MAXTRANS];
        getstring(text, p);
		CubeJ::GetClient().ackConnect(cn, type, text);
        conoutf("[DEBUG] receiveMessage<MSG_SND_CLIENTINFO>: cn: %d type: %d name: %s", cn, type, text);
    }

    template <> void receiveMessage<MSG_SND_SCENESTATUS>(int sender, int channel, packetbuf& p) {
        int hasscene = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_SND_SCENESTATUS> hasscene: %d", hasscene);
		conoutf("[TODO] get scene from server or upload running scene");
    }

    template <> void receiveMessage<MSG_REQ_REMOTE>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_REQ_REMOTE> sender: %d, remote client: %d", sender, clientnum);
		GetClient().connectRemoteInterface(clientnum);
    }

    template <> void receiveMessage<MSG_REQ_LISTMAPS>(int sender, int channel, packetbuf& p) {
        vector<char *> files;
        if ( ! listfiles("packages/base", "ogz", files) ) {
			conoutf ("[DEBUG] listdir: no scenes found");
		}
		MsgDataType<MSG_FWD_LISTMAPS> data(files);
		SendMessage(data);
	}

    template <> void receiveMessage<MSG_CMD_CHANGESCENE>(int sender, int channel, packetbuf& p) {
        MsgDataType<MSG_CMD_CHANGESCENE> data(p);
        GetClient().loadScene(data.name);
    }

    template <> void receiveMessage<MSG_FWD_RCIN>(int sender, int channel, packetbuf& p) {
        MsgDataType<MSG_FWD_RCIN> data(p);
        GetClient().executeremote(data.line);
    }
}
