#include "ui_mainwindow.h"

MainComponent::MainComponent (MainWindow* mwin) : mainwindow(mwin), nethandle(new NetworkDispatcher)
{
    addAndMakeVisible (toolbar = new MainToolbar( * mainwindow->commandManager));

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
    right->addTab (T("Connect"), Colours::lightgrey, clientlist = new ConnectComponent(), false);
    right->addTab (T("Scene"), Colours::lightgrey, scenes = new SceneComponent(), false);
    right->addTab (T("Console"), Colours::lightgrey, console = new Console(), false);
    right->setCurrentTabIndex (0);
    verticalDividerBar = new StretchableLayoutResizerBar (&verticalLayout, 1, true);

    addAndMakeVisible (left);
    addAndMakeVisible (verticalDividerBar);
    addAndMakeVisible (right);

    toolbar->addCommandListener(nethandle);
    clientlist->addConnectListener(nethandle);
    scenes->addLoadListener(nethandle);
    console->addOnSendCommandListener(nethandle);

    nethandle->addMessageHandler<CubeJ::MSG_SND_SERVINFO, ConnectComponent>(clientlist, &ConnectComponent::CallbackSrvInfo);
    nethandle->addMessageHandler<CubeJ::MSG_SND_CLIENTINFO, ConnectComponent>(clientlist, &ConnectComponent::CallbackClientInfo);
//    nethandler.addMessageHandler<CubeJ::MSG_CDIS, MainComponent>(this, &MainComponent::CallbackCDis);
//    nethandler.addMessageHandler<CubeJ::MSG_SND_SCENESTATUS, MainComponent>(this, &MainComponent::CallbackSceneStatus);
    nethandle->addMessageHandler<CubeJ::MSG_ACK_REMOTE, MainComponent>(this, &MainComponent::CallbackAckRemote);
    nethandle->addMessageHandler<CubeJ::MSG_SND_SCENEINFO, SceneComponent>(scenes, &SceneComponent::CallbackSceneInfo);
    nethandle->addMessageHandler<CubeJ::MSG_FWD_LISTMAPS, SceneComponent>(scenes, &SceneComponent::CallbackListScenes);
    nethandle->addMessageHandler<CubeJ::MSG_FWD_RCOUT, Console>(console, &Console::onReceiveRCout);
}

MainComponent::~MainComponent() {
    deleteAllChildren();
}

void MainComponent::resized()
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

const StringArray MainComponent::getMenuBarNames() {
    const tchar* const names[] = { T("Server"), T("Map"), 0 };
    return StringArray ((const tchar**) names);
}

const PopupMenu MainComponent::getMenuForIndex (int menuIndex, const String& menuName)
{
    ApplicationCommandManager* const commandManager = mainwindow->commandManager;

    PopupMenu menu;

    if (menuIndex == 0)
    {
        menu.addCommandItem (commandManager, ConnectWithServer);
        menu.addSeparator();
        menu.addCommandItem (commandManager, StandardApplicationCommandIDs::quit);
    }
    else if (menuIndex == 1)
    {
        menu.addCommandItem (commandManager, showMapDialog);
    }
    return menu;
}

void MainComponent::menuItemSelected (int menuItemID, int topLevelMenuIndex) {

}

ApplicationCommandTarget* MainComponent::getNextCommandTarget() {
    return findFirstTargetParentComponent();
}

void MainComponent::getAllCommands (Array <CommandID>& commands) {

    const CommandID ids[] = { ConnectWithServer,
                              showMapDialog
    };

    commands.addArray (ids, numElementsInArray (ids));
}

void MainComponent::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) {
        const String serverCategory (T("Server"));
        const String mapsCategory (T("Maps"));

        switch (commandID)
        {
        case ConnectWithServer:
            result.setInfo (T("Connect"), T("Connect with server"), serverCategory, 0);
            result.addDefaultKeypress (T('1'), ModifierKeys::commandModifier);
            break;
        case showMapDialog:
            result.setInfo (T("Maps"), T("Maps"), mapsCategory, 0);
            result.addDefaultKeypress (T('2'), ModifierKeys::commandModifier);
            break;
        }
}

bool MainComponent::perform (const InvocationInfo& info) {
    switch (info.commandID)
    {
        case ConnectWithServer:
        {
            std::cout << "ConnectWithServer" << std::endl;
            nethandle->connectWithServer( toolbar->getServerName().toCString(), toolbar->getServerPort() );
            break;
        }
        default:
            return false;
    }
    return true;
}

//Callbacks

//void MainComponent::CallbackCDis(int sender, int channel, packetbuf& p) {
//    int clientnum = getint(p);
//    std::cout << "[DEBUG] MainComponent::receiveMessageCallback<CubeJ::MSG_CDIS> clientnum: " << clientnum << std::endl;
//
//}
//
//void MainComponent::CallbackSceneStatus(int sender, int channel, packetbuf& p) {
//    int hasscene = getint(p);
//    conoutf("[DEBUG] MainComponent::receiveMessageCallback<MSG_SND_SCENESTATUS> hasscene: %d", hasscene);
//
//    //request list of maps
//    CubeJ::MsgDataType<CubeJ::MSG_REQ_LISTMAPS> data;
//    dispatcher.SendMessage(data);
//}
//
void MainComponent::CallbackAckRemote(int sender, int channel, packetbuf& p) {
    int clientnum = getint(p);
    std::cout  << "[DEBUG] MainComponent::receiveMessageCallback<MSG_ACK_REMOTE> control over client: " << clientnum << std::endl;
    nethandle->RequestMapList();
}

//void MainComponent::CallbackSceneInfo(int sender, int channel, packetbuf& p) {
////    CubeJ::MsgDataType<CubeJ::MSG_SND_SCENEINFO> data(p);
////    dispatcher.GetSceneManager().setCurrentScene(data.mapname, data.worldsize, data.mapversion);
//}
//
//void MainComponent::CallbackListMaps(int sender, int channel, packetbuf& p) {
//    scenes->receiveMessageListMaps(sender, channel, p);
//}

MainWindow::MainWindow() : DocumentWindow (AppInfo::projectName, Colours::lightgrey, DocumentWindow::allButtons, true)
{
    commandManager = new ApplicationCommandManager();

    setResizable (true, false); // resizability is a property of ResizableWindow
    setResizeLimits (800, 600, 8192, 8192);
    setUsingNativeTitleBar(true);

    MainComponent* maincomponent = new MainComponent (this);

    commandManager->registerAllCommandsForTarget (maincomponent);
    commandManager->registerAllCommandsForTarget (JUCEApplication::getInstance());

    // this lets the command manager use keypresses that arrive in our window to send
    // out commands
    addKeyListener (commandManager->getKeyMappings());

    // sets the main content component for the window to be this tabbed
    // panel. This will be deleted when the window is deleted.
    setContentComponent (maincomponent);

    // this tells the DocumentWindow to automatically create and manage a MenuBarComponent
    // which uses our maincomponent as its MenuBarModel
    setMenuBar (maincomponent);

#if JUCE_MAC  // ..and also the main bar if we're using that on a Mac...
    MenuBarModel::setMacMainMenu (1);
#endif

    // tells our menu bar model that it should watch this command manager for
    // changes, and send change messages accordingly.
    maincomponent->setApplicationCommandManagerToWatch (commandManager);

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

