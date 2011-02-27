#ifndef REMOTE_MAINWINDOW_H
#define REMOTE_MAINWINDOW_H

#include "config.h"
#include "ui_listener.h"
#include "dispatcher.h"
#include "juce_amalgamated.h"
#include "ui_connectcomponent.h"
#include "ui_scenecomponent.h"
#include "ui_maintoolbar.h"

//template <typename T>
//class KeyedVector : std::vector<T> {
//    public:
//        void add ( const T& x ) {
//            find_if()
//        }
//}

class MainWindow  : public DocumentWindow
{
public:
    MainWindow();
    ~MainWindow();

    void closeButtonPressed();

    ApplicationCommandManager* commandManager;
private:
};

//Session holds a Client, Server and Gui Data (Snapshot of your current activity)
//class SessionComponent
//{
//
//};

class MainComponent : public Component,
                      public MenuBarModel,
                      public ApplicationCommandTarget
{
public:
    MainComponent (MainWindow* win);
    ~MainComponent();

    void resized();
    const StringArray getMenuBarNames();
    const PopupMenu getMenuForIndex (int menuIndex, const String& menuName);
    void menuItemSelected (int menuItemID, int topLevelMenuIndex);
    ApplicationCommandTarget* getNextCommandTarget();
    void getAllCommands (Array <CommandID>& commands);
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
    bool perform (const InvocationInfo& info);

    enum CommandIDs
    {
        ConnectWithServer           = 0x2000,
        showMapDialog               = 0x2001
    };

    //Callbacks
//    void CallbackClientInfo(int sender, int channel, packetbuf& p);
//    void CallbackCDis(int sender, int channel, packetbuf& p);
//    void CallbackSceneStatus(int sender, int channel, packetbuf& p);
    void CallbackAckRemote(int sender, int channel, packetbuf& p);
//    void CallbackSceneInfo(int sender, int channel, packetbuf& p);
//    void CallbackListMaps(int sender, int channel, packetbuf& p);

private:
    MainWindow* mainwindow;

    //========Window Layout==========================
    StretchableLayoutManager        verticalLayout;
    StretchableLayoutManager        horizontalLayout;
    StretchableLayoutResizerBar*    verticalDividerBar;
    Component*                      vFrame;
    TabbedComponent*                left;
    TabbedComponent*                right;

    //=======Toolbar=================================
    MainToolbar*                    toolbar;

    //========Content Tabs===========================
    SceneComponent*                 scenes;
    ConnectComponent*               clientlist;

    //======RemoteInterface==========================
    NetworkDispatcher*              nethandle;
};

#endif // REMOTE_MAINWINDOW_H
