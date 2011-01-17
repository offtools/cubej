#include "remoteclient.h"

namespace CubeJRemote {

    template <> void receiveMessage<CubeJProtocol::MSG_ERROR_OVERFLOW>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<CubeJProtocol::MSG_ERROR_TAG>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<CubeJProtocol::MSG_SND_SERVINFO>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        int protocol = getint(p);
        if(protocol != CubeJProtocol::PROTOCOL_VERSION) {
            std::cout << "[DEBUG] receiveMessage<CubeJProtocol::MSG_SND_SERVINFO> - wrong protocol version" << std::endl;
            return;
        }
        std::cout << "[DEBUG] receiveMessage<MSG_SND_SERVINFO> clientnum: " << clientnum << ", protocol: " << protocol << std::endl;

        GetRemoteClient().setclientnum(clientnum);

        CubeJProtocol::MsgDataType<CubeJProtocol::MSG_DISCOVER_REMOTE> data;
        SendMessage(data);
	}

    template <> void receiveMessage<CubeJProtocol::MSG_SND_CLIENTINFO>(int sender, int channel, packetbuf& p) {
        int cn = getint(p);
		int type = getint(p);
        char text[MAXTRANS];
        getstring(text, p);
        if(GetRemoteClient().getclientnum() != cn) {
            std::cout << "[DEBUG] receiveMessage<CubeJProtocol::MSG_SND_CLIENTINFO> clientnum: " << cn << "type: " << type << " name: " << text << std::endl;
			GetRemoteClient().updateclientcache(cn, type, text);
			CubeJProtocol::MsgDataType<CubeJProtocol::MSG_REQ_REMOTE> data(cn);
            SendMessage(data);
        }
    }

    template <> void receiveMessage<CubeJProtocol::MSG_CDIS>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        std::cout << "[DEBUG] receiveMessage<CubeJProtocol::MSG_CDIS> clientnum: " << clientnum << std::endl;

    }

    template <> void receiveMessage<CubeJProtocol::MSG_SND_SCENESTATUS>(int sender, int channel, packetbuf& p) {
        int hasscene = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_SND_SCENESTATUS> hasscene: %d", hasscene);
    }

	template <> void receiveMessage<CubeJProtocol::MSG_ACK_REMOTE>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        std::cout  << "[DEBUG] receiveMessage<MSG_ACK_REMOTE> control over client: " << clientnum << std::endl;
        GetRemoteClient().connectClient(clientnum);
    }

	template <> void receiveMessage<CubeJProtocol::MSG_SND_LISTMAPS>(int sender, int channel, packetbuf& p) {
        char text[MAXTRANS];
        int length = getint(p);
        conoutf("[DEBUG] receiveMessage<CubeJProtocol::MSG_SND_LISTMAPS>");
        loopi(length) {
            getstring(text, p);
            conoutf("[DEBUG] map: %s", text);
        }
	}
}
