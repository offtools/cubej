#ifndef CUBEJ_SERVER_H_
#define CUBEJ_SERVER_H_

#include "cube.h"

namespace CubeJSrv {

	static const int RESERVE_CLIENTS = 3;
	static const int NUM_CHANNELS = 3;

	static int CUBEJ_LANINFO_PORT = 26784;
	static int CUBEJ_SERVER_PORT = 26785;
	static int CUBEJ_SERVINFO_PORT = 26786;
	static int CUBEJ_MASTER_PORT = 26787;
	static const char DEMO_MAGIC[] = "CUBEJ_DEMO";

	class Server;

	class ClientInfo {
	friend class Server;
	public:
		ClientInfo();
		~ClientInfo();

		void Init(int n, bool local);
		void Update();

		void setName(char* name);
		bool isConnected();

	private:
		//unique id for an client
		int clientnum;
		//time since trying to connect
		int connectmillis;
		//is connected
		bool connected; //( will be status )
		//local client or not
		bool local;
		//name used for the client
		string name;
		//messages
		vector<uchar> messages;
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
		void sendsceneinfo(packetbuf& p);
		void reset();

	private:
        bool hasscene;
		string mapname;
		hashset<SceneObject*> objects;
	};

	class Server {
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

        void connectClient(int cn, char* name);

		int numClients();

	private:
		vector<ClientInfo*> clients;
		vector<ClientInfo*> connecting;
		bool reliablemessages;
		//time since message was send
		enet_uint32 lastsend;
		//time since current scene started
		int scenemillis;
	};

	Server& GetServer();
	SceneManager& GetSceneManager();
}

#endif /* SERVER_H_ */