#ifndef REMOTE_LISTENER_H_INCLUDED
#define REMOTE_LISTENER_H_INCLUDED

#include "juce_amalgamated.h"

//template <typename LISTENER, typename T>
//class ListenerAdapter : public LISTENER {
//    public:
//        ListenerAdapter (T& source) : ref(source) {};
//        T& ref;
//};
//
//template <typename T>
//class ListenerAdapter<ButtonListener, T> : public ButtonListener {
//    public:
//        ListenerAdapter<ButtonListener, T> (T& source) : ref(source) {}
//    protected:
//        T& ref;
//};
//
//template <typename T>
//class ButtonListenerAdapter : public ListenerAdapter<ButtonListener, T> {
//    public:
//        ButtonListenerAdapter<T>(T& source) : ListenerAdapter<ButtonListener, T>(source) {}
//
//    protected:
//        using ListenerAdapter<ButtonListener, T>::ref;
//};

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

#endif // REMOTE_LISTENER_H_INCLUDED
