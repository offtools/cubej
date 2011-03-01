#include "msghandler.h"

namespace CubeJRemote {

    MsgHandlerDispatcher::MsgHandlerDispatcher() {}
    MsgHandlerDispatcher::~MsgHandlerDispatcher() {}

    void MsgHandlerDispatcher ::receive(CubeJProtocol::MSG_TYPE n, int sender, int channel, packetbuf& p) {
        conoutf("[DEBUG] MsgHandlerDispatcher::receive Type: %d, Descr: %s", n, GetMsgTypeInfo(n).description);
        if (  handler[n] != 0 &&  channel == GetMsgTypeInfo(n).channel ) {
             (*handler[n]) (sender, channel, p);
        }
        else {
            conoutf("[DEBUG] CubeJProtocol::ReceiveMessage - Message Type or Message Handler \"%s\" not found", GetMsgTypeInfo(n).description);
            p.len = p.maxlen; //triggers !remaining()
        }
    }

    void MsgHandlerDispatcher ::receive(int sender, int channel, packetbuf& p) {
        CubeJProtocol::MSG_TYPE type = (CubeJProtocol::MSG_TYPE)getint(p);
        receive(type, sender, channel, p);
    }
}
