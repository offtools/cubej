#ifndef CUBEJ_SERVER_H_
#define CUBEJ_SERVER_H_

#include "cube.h"

namespace CubeJSrv {

	class ClientInfo {
	public:
		ClientInfo();
		~ClientInfo();

	private:
		int clientnum;
		bool connected;
		bool local;
		vector<uchar> messages;
	};

	class MapObject {
	public:
		MapObject();
		~MapObject();

	private:
		const int id;
	};

	class MapManager {
	public:
		MapManager();
		~MapManager();

		int newobject(int type, int owner);
		int deleteobject(int id);

	private:
		hashet<MapObject> objects;
	};

	class Server {
	public:
		Server();
		~Server();

		void init();
		void connect();
		void disconnect();

	private:
		string map;
		vector<ClientInfo*> clients;
	};
}

#endif /* SERVER_H_ */
