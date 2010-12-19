#ifndef CUBEJ_CLIENT_H_INCLUDED
#define CUBEJ_CLIENT_H_INCLUDED

#include "cube.h"

namespace CubeJ
{
    //holds information and status of an client (fpsent in sauer)
    class Peer {
    public:
        Peer(int cn, const char* s) : clientnum(cn) {
            if(!s)
                name[0] = '\0';
            else
                copystring(name, s);
        }
        Peer() : clientnum(-1) {
            name[0] = '\0';
        }
        ~Peer();

        int clientnum;
        string name;
    };

	class Client
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
			void ackConnect(int cn, char* text);
			void finishConnect(bool remote);

			void requestScene(bool srvscene);

		protected:
		private:
            Peer& self;
            vector<Peer*> clients;
			bool connected;
			bool remote;
			vector<dynent*> cameras;
			vector<uchar> messages;
			int messagecn;
			bool messagereliable;
	};

	Client & GetClient();
	//Scene& GetScene();
}

#endif // CUBEJ_CLIENT_H_INCLUDED
