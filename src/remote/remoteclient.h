#ifndef REMOTECLIENT_H
#define REMOTECLIENT_H

#include <string>
#include <iostream>
#include <vector>

#include "engine.h"
#include "cube.h"
#include "protocol.h"
#include "clientinfo.h"
#include "scene.h"

namespace CubeJRemote {

    class RemoteClient : public CubeJProtocol::MsgHandler {
        public:
            RemoteClient();
            ~RemoteClient();

            void registerSceneManager(SceneMgr* );
            SceneMgr& GetSceneManager();

            void setServername(const char* name);
            void setServerport(int port);
            void connectWithServer();
            void connectWithServer(const char* sname, int port);
			void update();
            void disconnect();
            void connectWithClient(int cn);
            void disconnectClient();
            void disconnectClient(int cn);

            void setclientnum(int n);
            int getclientnum();

			void updateclientcache(int cn, int type, char* name);

            ENetPeer* peer;
        private:
            std::string servername;
            int port;
            ENetHost* host;
            ENetAddress address;
            ENetEvent event;
            int rate;
            int numchannels;
            int clientnum;
            CubeJ::ClientInfo* head;
            std::vector <CubeJ::ClientInfo*> clientcache;

            SceneMgr* scenemgr;
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
