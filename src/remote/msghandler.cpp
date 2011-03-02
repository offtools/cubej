#include "msghandler.h"

namespace CubeJRemote {

    MsgHandlerDispatcher::MsgHandlerDispatcher() {}
    MsgHandlerDispatcher::~MsgHandlerDispatcher() {}

    void MsgHandlerDispatcher ::receive(CubeJ::MSG_TYPE n, int sender, int channel, packetbuf& p) {
        if (  handler[n] != 0 &&  channel == GetMsgTypeInfo(n).channel ) {
             (*handler[n]) (sender, channel, p);
        }
        else {
            conoutf("[DEBUG] CubeJ::ReceiveMessage - Message Type or Message Handler \"%s\" not found", GetMsgTypeInfo(n).description);
            p.len = p.maxlen; //triggers !remaining()
        }
    }

    void MsgHandlerDispatcher ::receive(int sender, int channel, packetbuf& p) {
        CubeJ::MSG_TYPE type = (CubeJ::MSG_TYPE)getint(p);
        receive(type, sender, channel, p);
    }
}
