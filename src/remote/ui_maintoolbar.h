#ifndef REMOTE_MAINTOOLBAR_H
#define REMOTE_MAINTOOLBAR_H

#include "juce_amalgamated.h"

class MainToolbar : public Component
{
    public:
        MainToolbar(ApplicationCommandManager& mgr);
        ~MainToolbar();

        void resized();

//    protected:
//
//    private:
        class ItemFactory : public ToolbarItemFactory
        {
            public:
                ItemFactory(ApplicationCommandManager& mgr);
                ~ItemFactory();

                enum DemoToolbarItemIds
                {
                    doc_new         = 1,
                    doc_open        = 2,
                    doc_save        = 3,
                    doc_saveAs      = 4,
                    connect_server  = 5
                };

                void getAllToolbarItemIds (Array <int>& ids);
                void getDefaultItemSet (Array <int>& ids);
                ToolbarItemComponent* createItem (int itemId);

//            protected:
//
            private:
                ApplicationCommandManager& commandmgr;

            public:
                class CustomConnectPanel : public ToolbarItemComponent, public ButtonListener {
                    public:

                        CustomConnectPanel (const int toolbarItemId, ApplicationCommandManager& mgr);
                        ~CustomConnectPanel();
                        bool getToolbarItemSizes (int toolbarDepth, bool isToolbarVertical, int& preferredSize, int& minSize, int& maxSize);
                        void paintButtonArea (Graphics&, int, int, bool, bool);
                        void contentAreaChanged (const Rectangle<int>& contentArea);
                        void buttonClicked (Button* button);
                    protected:
                    private:
                        TextEditor* editor_server;
                        TextEditor* editor_port;
                        Label* label_server;
                        Label* label_port;
                        TextButton* button_connect;
                        StretchableLayoutManager layout;
                        ApplicationCommandManager& commandmgr;
                };
        };
    private:
        Toolbar* toolbar;
        ApplicationCommandManager& commandmgr;
        ItemFactory factory;
};

#endif // REMOTE_MAINTOOLBAR_H
