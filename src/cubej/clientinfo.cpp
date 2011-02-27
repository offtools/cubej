#include "clientinfo.h"
#include "protocol.h"

namespace CubeJ
{
    ClientInfo::ClientInfo()  : clientnum(-1), type(CLIENT_TYPE_NONE) {
        name[0] = '\0';
    }

    ClientInfo::ClientInfo(int n, int t, const char* s)  : clientnum(n), type(t) {
        if(!s)
            name[0] = '\0';
        else
            copystring(name, s);
    }

    ClientInfo::~ClientInfo() {}

    void ClientInfo::setName(const char* text)  {
        if(!text || !*text)
            copystring(name, "unamed");
        else
            copystring(name, text, CubeJProtocol::MAXNAMELEN+1);
    }

    void ClientInfo::setClientnum(int cn) {
        clientnum = cn;
    }

    const char* ClientInfo::getName() {
        return name;
    }

    int ClientInfo::getClientnum() {
        return clientnum;
    }

	int ClientInfo::getType() {
		return type;
	}

}
