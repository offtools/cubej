#ifndef REMOTECLIENT_H
#define REMOTECLIENT_H

#include <string>
#include <iostream>

#include "engine.h"
#include "cube.h"

#include "protocol.h"
#include "clientinfo.h"

namespace CubeJRemote {

    class RemoteClient : public CubeJProtocol::MsgHandler {
        public:
            RemoteClient();
            ~RemoteClient();

            void connectToServer(const char* sname, int port, int rate);
			void update();
            void disconnect();

            void connectClient(int cn);
            void disconnectClient();
            void disconnectClient(int cn);

            void setclientnum(int n);
            int getclientnum();

			void updateclientcache(int cn, int type, char* name);

            ENetPeer* peer;
        private:
            std::string servername;
            ENetHost* host;
            ENetAddress address;
            ENetEvent event;
            int rate;
            int numchannels;
            int clientnum;
            CubeJ::ClientInfo* head;
            vector <CubeJ::ClientInfo*> clientcache;
    };

    RemoteClient& GetRemoteClient();

    template <CubeJProtocol::MSG_TYPE N> void receiveMessage(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <CubeJProtocol::MSG_TYPE N> void SendMessage(CubeJProtocol::MsgDataType<N>& data) {
        packetbuf p(MAXTRANS, data.info.flag);
        data.addmsg(p);
        conoutf("[DEBUG] sending msg - type: %d, channel: %d", N, data.info.channel);
        if(GetRemoteClient().peer) enet_peer_send(GetRemoteClient().peer, data.info.channel, p.finalize());
 	}
}
#endif // REMOTECLIENT_H
