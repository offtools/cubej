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
        char text[MAXTRANS];
        getstring(text, p);
        if(GetRemoteClient().getclientnum() != cn) {
            std::cout << "[DEBUG] receiveMessage<CubeJProtocol::MSG_SND_CLIENTINFO> clientnum: " << cn << " name: " << text << std::endl;
            ///TODO: add add client info first ..., call connect to head separate
            CubeJProtocol::MsgDataType<CubeJProtocol::MSG_REQ_REMOTE> data(cn);
            SendMessage(data);
            std::cout << "[DEBUG] sending MSG_REQ_REMOTE> clientnum: " << cn << std::endl;
        }
    }

    template <> void receiveMessage<CubeJProtocol::MSG_SND_SCENESTATUS>(int sender, int channel, packetbuf& p) {
        int hasscene = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_SND_SCENESTATUS> hasscene: %d", hasscene);
    }

	template <> void receiveMessage<CubeJProtocol::MSG_ACK_REMOTE>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        std::cout  << "[DEBUG] receiveMessage<MSG_ACK_REMOTE> control over client: " << clientnum << std::endl;
    }
}
