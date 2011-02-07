#include "ui_connectcomponent.h"

ConnectComponent::ConnectComponent()
{
    listBox = new ListBox (T("CLIENTCONNECT"), this);
    listBox->setRowHeight (28);

    hFrame = new Component();
    conn = new TextButton ( T("connect"), T("Connect with Client") );
    conn->addButtonListener(this);
    disc = new TextButton ( T("disconnect"), T("Disconnect from Client") );
    disc->addButtonListener(this);

    hLayout.setItemLayout (0, -0.8, -1.0, -1.0);
    hLayout.setItemLayout (1, 30, 30, 30);
    vLayout.setItemLayout (0, -0.5, -0.5, -0.5);
    vLayout.setItemLayout (1, -0.5, -0.5, -0.5);

    addAndMakeVisible (listBox);
    hFrame->addAndMakeVisible (conn);
    hFrame->addAndMakeVisible (disc);
    addAndMakeVisible (hFrame);
}

ConnectComponent::~ConnectComponent() {
    deleteAllChildren();
}

void ConnectComponent::resized() {
    listBox->setBounds(0,0,getWidth(),getHeight());

    Component* hcomps[] = { listBox, hFrame };
    hLayout.layOutComponents (hcomps, 2,
                                 0,
                                 0,
                                 getWidth(),
                                 getHeight(),
                                 true,     // lay out side-by-side
                                 true);
    int y = hLayout.getItemCurrentAbsoluteSize(0);
    Component* vcomps[] = { conn, disc };
    vLayout.layOutComponents (vcomps, 2,
                                 0,
                                 0,
                                 getWidth(),
                                 getHeight()-y,
                                 false,     // lay out side-by-side
                                 true);
}

int ConnectComponent::getNumRows() { return 0; }

void ConnectComponent::paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) {}

void ConnectComponent::selectedRowsChanged (int lastRowselected) {}

void ConnectComponent::buttonClicked (Button *) {}