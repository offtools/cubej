#include "server.hpp"

namespace CubeJSrv {

	ClientInfo::ClientInfo() : clientnum(-1), connected(false), local(false) {}

	ClientInfo::~ClientInfo() {}

	Server::Server() {
		map = '\0';
	}

	Server::~Server() {}

	Server::init() {

	}

	Server::connect() {

	}

	Server::disconnect() {

	}
}
