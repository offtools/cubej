#include "dispatcher.h"
#include "ui_connectcomponent.h"

ConnectComponent::ConnectComponent() : clientnum(0)
{
    listBox = new ListBox (T("CLIENTCONNECT"), this);
    listBox->setRowHeight (28);
    listBox->setMultipleSelectionEnabled (false);

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

int ConnectComponent::getNumRows() { return clients.size(); }

void ConnectComponent::paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) {
    if (rowIsSelected)
        g.fillAll (Colours::lightblue);

    g.setColour (Colours::black);
    g.drawText ( clients[rowNumber]->getName(),
                4, 0, width - 4, height,
                Justification::centredLeft, true);
}

void ConnectComponent::selectedRowsChanged (int lastRowselected) {}

void ConnectComponent::addConnectListener(AppMessageCommandListener<ConnectComponent>* listener) {
    connect = listener;
}

void ConnectComponent::buttonClicked (Button *button) {
    if(button == conn) {
        connect->postMessage(NetworkDispatcher::AppCommandID::CONNECT_CLIENT, this);
    }

    else if (button == disc) {
        connect->postMessage(NetworkDispatcher::AppCommandID::DISCONNECT_CLIENT, this);
    }
}

void ConnectComponent::CallbackSrvInfo(int sender, int channel, packetbuf& p) {
    CubeJ::MsgDataType<CubeJ::MSG_SND_SERVINFO> data(p);
    if(data.protocol != CubeJ::PROTOCOL_VERSION) {
        std::cout << "[DEBUG] NetworkDispatcher::receiveMessage<CubeJ::MSG_SND_SERVINFO> - wrong protocol version" << std::endl;
        return;
    }
    clientnum = data.clientnum;
    connect->postMessage(NetworkDispatcher::AppCommandID::MSG_DISCOVER_REMOTE, this);
}

class eqalClientInfo : public std::unary_function<CubeJ::ClientInfo, bool> {
        int cn;
    public:
        explicit eqalClientInfo (const int i) : cn(i) {}
        bool operator() (const CubeJ::ClientInfo* ci) const { return cn == ci->getClientnum(); }
};

void ConnectComponent::CallbackClientInfo(int sender, int channel, packetbuf& p) {
    CubeJ::MsgDataType<CubeJ::MSG_SND_CLIENTINFO> data(p);
    std::vector<CubeJ::ClientInfo*>::iterator it;
    it = find_if(clients.begin(), clients.end(), eqalClientInfo(clientnum));
    if( it != clients.end() && !clients.size() ) return;
    clients.push_back( new CubeJ::ClientInfo(data.cn, data.type, data.name) );
    listBox->updateContent();
}

const CubeJ::ClientInfo* ConnectComponent::getSelectedClient() const {
    SparseSet< int > rows = listBox->getSelectedRows ();
    if( !rows.size() )
        return 0;
    int idx = rows[0];
    return clients[idx];
}
