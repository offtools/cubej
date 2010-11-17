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

}

#endif /* SERVER_H_ */
