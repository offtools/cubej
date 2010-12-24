#ifndef CUBEJ_CLIENTINFO_H_INCLUDED
#define CUBEJ_CLIENTINFO_H_INCLUDED

#include "cube.h"

namespace CubeJ
{
    enum { CLIENT_TYPE_NONE = 0, CLIENT_TYPE_HEAD, CLIENT_TYPE_REMOTE };

    class Client;

    class ClientInfo {
    friend class Client;
    public:
        ClientInfo();
        ClientInfo(int n, const char* s);
        ~ClientInfo();

        void setName(char* text);

    protected:
        //unique id for an client
		int clientnum;

		//type of the client (either SDL Head or RemoteCLient)
		int type;

		//name used for the client
		string name;
    };
}
#endif // CUBEJ_CLIENTINFO_H_INCLUDED
