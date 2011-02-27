#ifndef REMOTE_MAINTOOLBAR_H
#define REMOTE_MAINTOOLBAR_H

#include "config.h"
#include "juce_amalgamated.h"


class ConnectItem : public ToolbarItemComponent {
    public:
        ConnectItem (const int toolbarItemId);
        ~ConnectItem();

        bool getToolbarItemSizes (int toolbarDepth, bool isToolbarVertical, int& preferredSize, int& minSize, int& maxSize);
        void paintButtonArea (Graphics&, int, int, bool, bool);
        void contentAreaChanged (const Rectangle<int>& contentArea);

        TextEditor* editor_server;
        TextEditor* editor_port;
        Label* label_server;
        Label* label_port;
        TextButton* button_connect;
        StretchableLayoutManager layout;
};

class MainToolbar : public Component,
                    public ToolbarItemFactory
{
    public:
        MainToolbar(ApplicationCommandManager& mgr);
        ~MainToolbar();

        enum ToolbarItemIds
        {
            doc_new         = 1,
            doc_open        = 2,
            doc_save        = 3,
            doc_saveAs      = 4,
            connect_server  = 5
        };

        //Toobar Component Method
        void resized();

        //Toolbar Factory Methods
        void getAllToolbarItemIds (Array <int>& ids);
        void getDefaultItemSet (Array <int>& ids);
        ToolbarItemComponent* createItem (int itemId);

        //register extra Listener for Connect Button
        void addConnectListener(ButtonListener* listener);

        //Values
        String getServerName();
        int getServerPort();

    private:
        ApplicationCommandManager&      commandmgr;
        ConnectItem*                    connectitem;
        Toolbar*                        toolbar;
};


#endif // REMOTE_MAINTOOLBAR_H
