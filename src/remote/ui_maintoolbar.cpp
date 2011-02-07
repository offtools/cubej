#include "remoteclient.h"
#include "config.h"
#include "ui_maintoolbar.h"
#include "ui_mainwindow.h"

MainToolbar::MainToolbar(ApplicationCommandManager& mgr) : commandmgr(mgr), factory(commandmgr) {
    addAndMakeVisible (toolbar = new Toolbar());
    toolbar->setSize(getWidth(), 30);
    toolbar->addDefaultItems (factory);
}

MainToolbar::~MainToolbar() {
    deleteAllChildren();
}

void MainToolbar::resized() {
    toolbar->setBounds (0, 0, getWidth(), getHeight());
}

MainToolbar::ItemFactory::ItemFactory(ApplicationCommandManager& mgr) : commandmgr(mgr) {}

MainToolbar::ItemFactory::~ItemFactory() {}

void MainToolbar::ItemFactory::getAllToolbarItemIds (Array <int>& ids) {
    ids.add (doc_new);
    ids.add (doc_open);
    ids.add (doc_save);
    ids.add (doc_saveAs);
    ids.add (connect_server);
}

void MainToolbar::ItemFactory::getDefaultItemSet (Array <int>& ids) {
    ids.add (doc_new);
    ids.add (doc_open);
    ids.add (doc_save);
    ids.add (doc_saveAs);
    ids.add (connect_server);
}

ToolbarItemComponent* MainToolbar::ItemFactory::createItem (int itemId) {
    switch (itemId)
    {
        case doc_new:
        {
            Drawable* drawable = Drawable::createFromImageFile(File::getCurrentWorkingDirectory().getChildFile("data/icons/gtk-new.png"));
            return new ToolbarButton (itemId, "new", drawable, 0);
        }
        case doc_open:
        {
            Drawable* drawable = Drawable::createFromImageFile(File::getCurrentWorkingDirectory().getChildFile("data/icons/gtk-open.png"));
            return new ToolbarButton (itemId, "open", drawable, 0);
        }
        case doc_save:
        {
            Drawable* drawable = Drawable::createFromImageFile(File::getCurrentWorkingDirectory().getChildFile("data/icons/gtk-save.png"));
            return new ToolbarButton (itemId, "save", drawable, 0);
        }
        case doc_saveAs:
        {
            Drawable* drawable = Drawable::createFromImageFile(File::getCurrentWorkingDirectory().getChildFile("data/icons/gtk-save-as.png"));
            return new ToolbarButton (itemId, "save as", drawable, 0);
        }
        case connect_server:
        {
            return new CustomConnectPanel (itemId, commandmgr);
        }

        default:
            break;
    }

    return 0;
}

MainToolbar::ItemFactory::CustomConnectPanel::CustomConnectPanel (const int toolbarItemId, ApplicationCommandManager& mgr) : ToolbarItemComponent(toolbarItemId, "Connect to Server", false), commandmgr(mgr) {
    layout.setItemLayout (0, -0.2, -0.2, -0.2);
    layout.setItemLayout (1, -0.2, -0.2, -0.2);
    layout.setItemLayout (2, -0.2, -0.2, -0.2);
    layout.setItemLayout (3, -0.2, -0.2, -0.2);
    layout.setItemLayout (4, -0.2, -0.2, -0.2);

    addAndMakeVisible (editor_server = new TextEditor (T("SERVER_EDITOR")));
    editor_server->setMultiLine (false);
    editor_server->setReturnKeyStartsNewLine (false);
    editor_server->setReadOnly (false);
    editor_server->setScrollbarsShown (false);
    editor_server->setCaretVisible (true);
    editor_server->setPopupMenuEnabled (false);
    editor_server->setText (T("localhost"));

    addAndMakeVisible (editor_port = new TextEditor (T("PORT_EDITOR")));
    editor_port->setMultiLine (false);
    editor_port->setReturnKeyStartsNewLine (false);
    editor_port->setReadOnly (false);
    editor_port->setScrollbarsShown (false);
    editor_port->setCaretVisible (true);
    editor_port->setPopupMenuEnabled (false);
    editor_port->setText (T("26785"));

    addAndMakeVisible (label_server = new Label (T("SERVER_LABEL"),
                                                 T("Server:")));
    label_server->setFont (Font (15.0000f, Font::plain));
    label_server->setJustificationType (Justification::centredLeft);
    label_server->setEditable (false, false, false);
    label_server->setColour (TextEditor::textColourId, Colours::black);
    label_server->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label_port = new Label (T("PORT_LABEL"),
                                               T("Port:")));
    label_port->setFont (Font (15.0000f, Font::plain));
    label_port->setJustificationType (Justification::centredLeft);
    label_port->setEditable (false, false, false);
    label_port->setColour (TextEditor::textColourId, Colours::black);
    label_port->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (button_connect = new TextButton (T("BUTTON_CONNECT")));
    button_connect->setButtonText (T("connect"));
    button_connect->addButtonListener (this);

    editor_server->setSize(80, 24);
    editor_port->setSize(80,24);
    label_server->setSize(80,24);
    label_port->setSize(80,24);
    button_connect->setSize(80,24);
}

MainToolbar::ItemFactory::CustomConnectPanel::~CustomConnectPanel() {
    deleteAllChildren();
}

bool MainToolbar::ItemFactory::CustomConnectPanel::getToolbarItemSizes (int toolbarDepth, bool isToolbarVertical, int& preferredSize, int& minSize, int& maxSize) {
    preferredSize = 300;
    minSize = 200;
    maxSize = 400;
    return true;
}

void MainToolbar::ItemFactory::CustomConnectPanel::paintButtonArea (Graphics&, int, int, bool, bool) {}

void MainToolbar::ItemFactory::CustomConnectPanel::contentAreaChanged (const Rectangle<int>& contentArea) {
    Component* comps[] = { label_server, editor_server, label_port, editor_port, button_connect};
    layout.layOutComponents (comps, 5,
                                 0,
                                 AppLayout::HorizontalSpacing,
                                 getWidth(),
                                 getHeight()-(AppLayout::HorizontalSpacing*2),
                                 false,     // lay out side-by-side
                                 true);
}

void MainToolbar::ItemFactory::CustomConnectPanel::buttonClicked (Button* button) {
    if (button == button_connect)
    {
        const char* server = editor_server->getText().toCString();
        const int port = editor_port->getText().getIntValue ();
        CubeJRemote::RemoteClient& remote = CubeJRemote::GetRemoteClient();
        remote.setServername(server);
        remote.setServerport(port);
        setEnabled(false);
        commandmgr.invokeDirectly(ContentComp::ConnectWithServer, false);
        std::cout << "buttonClicked" << std::endl;
    }
}
