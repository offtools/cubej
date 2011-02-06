#include <string>
#include <iostream>
#include "remoteclient.h"
#include "ui_mainwindow.h"
#include "ui_connectcomponent.h"

ContentComp::ContentComp (MainWindow* _mainwindow) : mainwindow(_mainwindow), mainloop(new MainLoop) {

    toolbar = new Toolbar();
    toolbar->setSize(getWidth(), 30);
    addAndMakeVisible (toolbar);

    horizontalLayout.setItemLayout (0, 30, 30, 30);
    verticalLayout.setItemLayout (0, -0.2, -0.8, -0.4);
    verticalLayout.setItemLayout (1, 8, 8, 8);
    verticalLayout.setItemLayout (2, -0.2, -0.8, -0.6);

    left = new TabbedComponent (TabbedButtonBar::TabsAtBottom);
    left->setTabBarDepth (30);
    left->addTab (T("Control"), Colours::lightgrey, new Label(T("Control"), T("Control")), false);
    left->setCurrentTabIndex (0);

    right = new TabbedComponent (TabbedButtonBar::TabsAtBottom);
    right->setTabBarDepth (30);
//    right->addTab (T("Connect"), Colours::lightgrey, new ConnectPanel(mainloop), false);
    right->addTab (T("Connect"), Colours::lightgrey, new ConnectComponent, false);
    scenes = new SceneComponent;
    CubeJRemote::GetRemoteClient().registerSceneManager(scenes);
    right->addTab (T("Scene"), Colours::lightgrey, scenes, false);
    right->setCurrentTabIndex (0);

    verticalDividerBar = new StretchableLayoutResizerBar (&verticalLayout, 1, true);

    addAndMakeVisible (left);
    addAndMakeVisible (verticalDividerBar);
    addAndMakeVisible (right);
}


ContentComp::~ContentComp() {
    deleteAllChildren();
}

void ContentComp::resized()
{
    Component* hcomps[] = { toolbar, 0 };
    horizontalLayout.layOutComponents (hcomps, 2,
                                 0,
                                 0,
                                 getWidth(),
                                 getHeight(),
                                 true,     // lay out side-by-side
                                 true);     // resize the components' heights as well as widths
    Component* vcomps[] = { left, verticalDividerBar, right };
    verticalLayout.layOutComponents (vcomps, 3,
                                     0,
                                     30,
                                     getWidth(),
                                     getHeight()-30,
                                     false,     // lay out side-by-side
                                     true);     // resize the components' heights as well as widths
}

const StringArray ContentComp::getMenuBarNames() {
    const tchar* const names[] = { T("Server"), T("Map"), 0 };
    return StringArray ((const tchar**) names);
}

const PopupMenu ContentComp::getMenuForIndex (int menuIndex, const String& menuName)
{
    ApplicationCommandManager* const commandManager = mainwindow->commandManager;

    PopupMenu menu;

    if (menuIndex == 0)
    {
        menu.addCommandItem (commandManager, showServerDialog);
        menu.addSeparator();
        menu.addCommandItem (commandManager, StandardApplicationCommandIDs::quit);
    }
    else if (menuIndex == 1)
    {
        menu.addCommandItem (commandManager, showMapDialog);
    }
    return menu;
}

void ContentComp::menuItemSelected (int menuItemID, int topLevelMenuIndex) {

}

ApplicationCommandTarget* ContentComp::getNextCommandTarget() {
    return findFirstTargetParentComponent();
}

void ContentComp::getAllCommands (Array <CommandID>& commands) {

    const CommandID ids[] = { showServerDialog,
                              showMapDialog
    };

    commands.addArray (ids, numElementsInArray (ids));
}

void ContentComp::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) {
        const String serverCategory (T("Server"));
        const String mapsCategory (T("Maps"));

        switch (commandID)
        {
        case showServerDialog:
            result.setInfo (T("Connect"), T("Connect to a server"), serverCategory, 0);
            result.addDefaultKeypress (T('1'), ModifierKeys::commandModifier);
            break;
        case showMapDialog:
            result.setInfo (T("Maps"), T("Maps"), mapsCategory, 0);
            result.addDefaultKeypress (T('2'), ModifierKeys::commandModifier);
            break;
        }
}

bool ContentComp::perform (const InvocationInfo& info) {
    switch (info.commandID)
    {
        case showServerDialog:
            std::cout << "showServerDialog" << std::endl;
            break;
        default:
            return false;
    }
    return true;
}



MainWindow::MainWindow() : DocumentWindow (AppInfo::projectName, Colours::lightgrey, DocumentWindow::allButtons, true)
{
    commandManager = new ApplicationCommandManager();

    setResizable (true, false); // resizability is a property of ResizableWindow
    setResizeLimits (800, 600, 8192, 8192);

    ContentComp* contentComp = new ContentComp (this);

    commandManager->registerAllCommandsForTarget (contentComp);
    commandManager->registerAllCommandsForTarget (JUCEApplication::getInstance());

    // this lets the command manager use keypresses that arrive in our window to send
    // out commands
    addKeyListener (commandManager->getKeyMappings());

    // sets the main content component for the window to be this tabbed
    // panel. This will be deleted when the window is deleted.
    setContentComponent (contentComp);

    // this tells the DocumentWindow to automatically create and manage a MenuBarComponent
    // which uses our contentComp as its MenuBarModel
    setMenuBar (contentComp);

    // tells our menu bar model that it should watch this command manager for
    // changes, and send change messages accordingly.
    contentComp->setApplicationCommandManagerToWatch (commandManager);

    setVisible (true);
}

MainWindow::~MainWindow()
{
    // because we've set the content comp to be used as our menu bar model, we
    // have to switch this off before deleting the content comp..
    setMenuBar (0);

#if JUCE_MAC  // ..and also the main bar if we're using that on a Mac...
    MenuBarModel::setMacMainMenu (0);
#endif

    // setting our content component to 0 will delete the current one, and
    // that will in turn delete all its child components. You don't always
    // have to do this explicitly, because the base class's destructor will
    // also delete the content component, but in this case we need to
    // make sure our content comp has gone away before deleting our command
    // manager.
    setContentComponent (0, true);

    delete commandManager;
}

void MainWindow::closeButtonPressed() {
    JUCEApplication::getInstance()->systemRequestedQuit();
}
