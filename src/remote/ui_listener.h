#ifndef REMOTE_LISTENER_H_INCLUDED
#define REMOTE_LISTENER_H_INCLUDED

#include "juce_amalgamated.h"

template <typename T>
class AppMessageListener : public MessageListener {
    public:
        virtual void handleData(T* userdata) = 0;

        void handleMessage (const Message &message) {
            T* data = static_cast <T*> (message.pointerParameter);
            handleData(data);
        }

        void postMessage(T* message) {
            MessageListener::postMessage(new Message(0,0,0,message) );
        }
};

template <typename T>
class AppMessageCommandListener : public MessageListener {
    public:

        virtual void handleData(int id, T* userdata) = 0;

        void handleMessage (const Message &message) {
            T* data = static_cast <T*> (message.pointerParameter);
            handleData(message.intParameter1, data);
        }

        void postMessage(int id, T* message) {
            MessageListener::postMessage(new Message(id,0,0,message) );
        }
};
#endif // REMOTE_LISTENER_H_INCLUDED
