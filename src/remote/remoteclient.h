#ifndef REMOTECLIENT_H
#define REMOTECLIENT_H


//#include "config.h"
#include "protocol.h"
#include "clientinfo.h"
#include "msghandler.h"

namespace CubeJRemote {

    class RemoteInterface : public MsgHandlerDispatcher
    {
        public:
            RemoteInterface();
            ~RemoteInterface();

            //connect with Server
            bool connectWithServer();
            bool connectWithServer(const char* name, int port);

            //connect with Client (clientnum's received by sending MSG_DISCOVER_REMOTE)
            void connectWithClient(int cn);

            //Disconnect from Server
            void disconnect();

            //close Client Connection
            void disconnectClient();

            //Main Update Routine (receive and dispatch new Messages)
			void update();

            void RequestMapList();
            void RequestClientInfoList();
            void RequestChangeScene(const char* path);
            void SendCommand(const char *command);

            void SendMessage(int channel, packetbuf& p);

            template <CubeJ::MSG_TYPE N> void SendMessage(CubeJ::MsgDataType<N>& data) {
                packetbuf p(MAXTRANS, data.info.flag);
                data.addmsg(p);
                SendMessage(data.info.channel, p);
            }

        private:
            int port;
            ENetPeer* peer;
            ENetHost* host;
            ENetAddress address;
            ENetEvent event;
            int rate;
            int numchannels;
    };

    bool Init();
}
#endif // REMOTECLIENT_H
