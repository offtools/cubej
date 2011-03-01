#ifndef CUBEJ_SERVER_H_
#define CUBEJ_SERVER_H_

#include "cube.h"
#include "clientinfo.h"
#include "protocol.h"

namespace CubeJSrv {

	static const int RESERVE_CLIENTS = 3;

	static int CUBEJ_LANINFO_PORT = 26784;
	static int CUBEJ_SERVER_PORT = 26785;
	static int CUBEJ_SERVINFO_PORT = 26786;
	static int CUBEJ_MASTER_PORT = 26787;
	static const char DEMO_MAGIC[] = "CUBEJ_DEMO";

	class Server;

	class SvClientInfo : public CubeJ::ClientInfo {
	friend class Server;
	public:
		SvClientInfo();
		~SvClientInfo();

		void Init(int n, bool local);
		void Update();

		bool isConnected();

	private:
		//time since trying to connect
		int connectmillis;
		//is connected
		bool connected; //( will be status )
		//clientnum pair client
		int pairnum;
		//local client or not
		bool local;
		//messages
		vector<uchar> messages;
		vector<uchar> remote;
	};

	class SceneObject {
	public:
		SceneObject();
		~SceneObject();

	private:
		const int id;
	};

	class SceneManager {
	public:
		SceneManager();
		~SceneManager();

		int newobject(int type, int owner);
		int deleteobject(int id);
		void sendscenestatus(packetbuf& p);
		void changeScene(const char* name);
		void reset();

	private:
        bool hasscene;
		string mapname;
		hashset<SceneObject*> objects;
	};

	class Server : public CubeJProtocol::MsgHandler {
	public:
		Server();
		~Server();

		void Init();
		void Update();

		//start connect routine for a new client (send clientnum, protocol version)
		int sendServerInfo(int n, bool local = true);

		//send client and scene status information (update client)
		void sendServerStatus(int cn);

		//notify clients on diconnected clients
		void notifyDisconnect(int n);

		//tell server to send packets or not
		bool sendAllPackets(bool force);

		//parse received packets from clients
		void parsePacket(int sender, int chan, packetbuf &p);

        void registerClient(int cn, char* name);

        void registerRemoteInterface(int cn);

        //after receiving ack from head, etablish connection between remote client and head
        void connectRemoteInterface(int head, int remote);

		int numClients();

        void forwardMessage(int cn, int channel, packetbuf& p);

	private:
		vector<SvClientInfo*> clients;
		vector<SvClientInfo*> connecting;
		bool reliablemessages;
		//time since message was send
		enet_uint32 lastsend;
		//time since current scene started
		int scenemillis;
	};

	Server& GetServer();
	SceneManager& GetSceneManager();

	template <MSG_TYPE N> void receiveMessage(int sender, int channel, packetbuf& p) { p.cleanup(); }
}

#endif /* SERVER_H_ */
