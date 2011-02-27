#ifndef REMOTE_MSGHANDLER_H
#define REMOTE_MSGHANDLER_H

#include <string>
#include <iostream>

#include "protocol.h"

namespace CubeJRemote {

    class MsgFunctor {
        public:
            virtual void operator() (int sender, int channel, packetbuf& p) = 0;
    };

    template < typename T >
    class MsgHandler : public MsgFunctor
    {
        public:
            MsgHandler(T* o, void (T::*func)(int sender, int channel, packetbuf& p)) : obj(o), cb(func) {}
            void operator() (int sender, int channel, packetbuf& p) {
                (obj->*cb)(sender, channel, p);
            }
        private:
            T* obj;
            typedef void (T::*callback)(int sender, int channel, packetbuf& p);
            callback cb;
    };

	class MsgHandlerDispatcher {
	    public:
            MsgHandlerDispatcher();
            ~MsgHandlerDispatcher();

            template <typename CubeJProtocol::MSG_TYPE N, typename T>
            void addMessageHandler(T* obj, void(T::*func)(int sender, int channel, packetbuf& p)) {
                handler[N] = new MsgHandler<T>(obj, func);
            }

            void receive(CubeJProtocol::MSG_TYPE n, int sender, int channel, packetbuf& p);
            void receive(int sender, int channel, packetbuf& p);

        private:
            MsgFunctor* handler[CubeJProtocol::NUM_MESSAGES];

	};
}
#endif
