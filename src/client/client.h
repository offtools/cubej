#ifndef CUBEJ_CLIENT_H_INCLUDED
#define CUBEJ_CLIENT_H_INCLUDED

#include "cube.h"
#include "clientinfo.h"
#include "protocol.h"

namespace CubeJ
{
	class Client : public CubeJProtocol::MsgHandler
	{
		public:
			Client();
			~Client();

			void Init();
			void Update();
			void Disconnect();

			//collect and send all messages to the server - formerly c2sinfo
			void sendAllMessages(bool force = false);

			dynent* getCamera(int num = 0);

			void editMode(bool on);
			void startScene();
			void parsePacket(int chan, packetbuf &p);

			void initConnect(int cn, int protocol);

			//called when received a client info message about new connected client
			void ackConnect(int cn, int type, char* text);

			void finishConnect(bool remote);

			void requestScene(bool srvscene);
			void loadScene(const char* name);

			void connectRemoteInterface(int n);
			//send console output to remote interface
            void redirectcout(char *line);
            //excute command from remote interface
            void executeremote(char* line);

		protected:
		private:
            ClientInfo& self;
            vector<ClientInfo*> clients;
			bool connected;
			bool remote;
			int remoteclientnum;
			vector<dynent*> cameras;
			vector<uchar> messages;
			int messagecn;
			bool messagereliable;
            string clientmap;
	};

	Client & GetClient();
	//Scene& GetScene();

	template <CubeJProtocol::MSG_TYPE N> void receiveMessage(int sender, int channel, packetbuf& p) { p.cleanup(); }
}

#endif // CUBEJ_CLIENT_H_INCLUDED
