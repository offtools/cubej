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

    ClientInfo::~ClientInfo() {}

    void ClientInfo::setName(char* text)  {
        if(!text[0]) copystring(text, "unnamed");
        copystring(name, text, CubeJProtocol::MAXNAMELEN+1);
    }
}
