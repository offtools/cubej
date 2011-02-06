#ifndef REMOTE_MAINWINDOW_H
#define REMOTE_MAINWINDOW_H

#include "config.h"
#include "juce_amalgamated.h"
#include "ui_connectpanel.h"
#include "ui_mainloop.h"
#include "ui_scenecomponent.h"

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

class ContentComp  : public Component,
                     public MenuBarModel,
                     public ApplicationCommandTarget
{
public:
    ContentComp (MainWindow* _mainwindow);
    ~ContentComp();

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
        showServerDialog           = 0x2000,
        showMapDialog              = 0x2001
    };

private:
    MainWindow* mainwindow;

    //========Window Layout=======
    StretchableLayoutManager verticalLayout;
    StretchableLayoutManager horizontalLayout;
    StretchableLayoutResizerBar* verticalDividerBar;
    Component* vFrame;
    TabbedComponent* left;
    TabbedComponent* right;

    //=======Toolbar==============
    Toolbar* toolbar;

    //========Content Tabs========
    SceneComponent* scenes;

    MainLoop* mainloop;
};

#endif // REMOTE_MAINWINDOW_H
