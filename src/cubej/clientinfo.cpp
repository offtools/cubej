#include "clientinfo.h"
#include "protocol.h"

namespace CubeJ
{
    ClientInfo::ClientInfo()  : clientnum(-1), type(CLIENT_TYPE_NONE) {
        name[0] = '\0';
    }

    ClientInfo::ClientInfo(int n, const char* s)  : clientnum(n) {
        if(!s)
            name[0] = '\0';
        else
            copystring(name, s);
    }

	ClientInfo::ClientInfo(int t, int cn)  : clientnum(cn), type(t) {
		name[0] = '\0';
	}

    ClientInfo::~ClientInfo() {}

    void ClientInfo::setName(char* text)  {
        if(!text || !*text) copystring(text, "unnamed");
        copystring(name, text, CubeJProtocol::MAXNAMELEN+1);
    }

    int ClientInfo::getclientnum() {
        return clientnum;
    }

	int ClientInfo::gettype() {
		return type;
	}

}
