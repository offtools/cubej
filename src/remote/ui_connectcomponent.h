#ifndef REMOTE_CONNECTCOMPONENT_H
#define REMOTE_CONNECTCOMPONENT_H

#include "config.h"
#include "protocol.h"
#include "juce_amalgamated.h"
#include "ui_listener.h"

class ConnectComponent : public Component,
                         public ListBoxModel,
                         public ButtonListener
{
    public:
        ConnectComponent();
        ~ConnectComponent();

        void resized();
        int getNumRows();
        void paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected);
        void selectedRowsChanged (int lastRowselected);
        void buttonClicked (Button *);
        void addConnectListener(AppMessageCommandListener<ConnectComponent>* messagelistener);

        void CallbackSrvInfo(int sender, int channel, packetbuf& p);
        void CallbackClientInfo(int sender, int channel, packetbuf& p);

        const CubeJ::ClientInfo* getSelectedClient() const;

    protected:

    private:
        //Layout Components
        ListBox* listBox;
        TextButton* conn;
        TextButton* disc;
        Component* hFrame;
        StretchableLayoutManager hLayout;
        StretchableLayoutManager vLayout;

        std::vector<CubeJ::ClientInfo*> clients;
        int clientnum;
        AppMessageCommandListener<ConnectComponent> *connect;
};
#endif // REMOTE_CONNECTCOMPONENT_H
