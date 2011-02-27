#ifndef REMOTE_CONNECTCOMPONENT_H
#define REMOTE_CONNECTCOMPONENT_H

#include "config.h"
#include "juce_amalgamated.h"

class MainComponent;

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
        //void updateclientcache(int cn, int type, char* name);

    protected:

    private:
        //Layout Components
        ListBox* listBox;
        TextButton* conn;
        TextButton* disc;
        Component* hFrame;
        StretchableLayoutManager hLayout;
        StretchableLayoutManager vLayout;

        //Model
        //std::vector <CubeJ::ClientInfo*> clientcache;
};

#endif // REMOTE_CONNECTCOMPONENT_H
