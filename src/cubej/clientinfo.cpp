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
