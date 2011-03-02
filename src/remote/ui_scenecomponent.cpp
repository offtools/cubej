#include <string>
#include <iostream>

#include "dispatcher.h"
#include "ui_scenecomponent.h"

SceneComponent::SceneComponent() : loadlistener(0)
{
    listBox = new ListBox (T("SCENES"), this);
    listBox->setRowHeight (28);
    listBox->setMultipleSelectionEnabled (false);

    loadbutton = new TextButton ( T("load"), T("Load selected Scene") );
    loadbutton->addButtonListener(this);

    layout.setItemLayout (0, -0.8, -1.0, -1.0);
    layout.setItemLayout (1, 30, 30, 30);

    addAndMakeVisible (listBox);
    addAndMakeVisible (loadbutton);
}

SceneComponent::~SceneComponent()
{
    deleteAllChildren();
}

int SceneComponent::getNumRows()
{
    return sceneinfo.size();
}

void SceneComponent::resized()
{
    listBox->setBounds(0,0,getWidth(),getHeight());

    Component* hcomps[] = { listBox, loadbutton };
    layout.layOutComponents (hcomps, 2,
                                 0,
                                 0,
                                 getWidth(),
                                 getHeight(),
                                 true,     // lay out side-by-side
                                 true);
}

void SceneComponent::paintListBoxItem (int rowNumber,
                       Graphics& g,
                       int width, int height,
                       bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll (Colours::lightblue);

    g.setColour (Colours::black);
    g.drawText ( sceneinfo[rowNumber]->getSceneName(),
                4, 0, width - 4, height,
                Justification::centredLeft, true);
}

void SceneComponent::selectedRowsChanged(int) {

}

std::string SceneComponent::getSelectedSceneName() {
    SparseSet< int > rows = listBox->getSelectedRows ();
    if( !rows.size() )
        return 0;
    int idx = rows[0];
    return sceneinfo[idx]->getSceneName();
}

void SceneComponent::buttonClicked (Button *button) {
    loadlistener->postMessage(NetworkDispatcher::AppCommandID::MSG_REQ_CHANGESCENE, this);
}

void SceneComponent::addLoadListener(AppMessageCommandListener<SceneComponent> *listener) {
    loadlistener = listener;
}

class eqalSceneInfo : public std::unary_function<CubeJRemote::SceneInfo, bool> {
        std::string path;
    public:
        explicit eqalSceneInfo (const std::string s) : path(s) {}
        bool operator() (const CubeJRemote::SceneInfo* si) const { std::string s(si->getSceneName()); return path == s; }
};

void SceneComponent::CallbackListScenes(int sender, int channel, packetbuf& p) {
    CubeJ::MsgDataType<CubeJ::MSG_FWD_LISTMAPS> data(p);
    std::vector<CubeJRemote::SceneInfo*>::iterator it;
    for(int i = 0; i < data.len; i++) {
        it = find_if(sceneinfo.begin(), sceneinfo.end(), eqalSceneInfo( data.listing[i] ) );
        if ( it != sceneinfo.end() && sceneinfo.size() ) {
            return;
        }
        else {
            sceneinfo.push_back( new CubeJRemote::SceneInfo(data.listing[i]) );
        }
    }
    listBox->updateContent();
}

void SceneComponent::CallbackSceneInfo(int sender, int channel, packetbuf& p) {
    CubeJ::MsgDataType<CubeJ::MSG_SND_SCENEINFO> data(p);
    std::vector<CubeJRemote::SceneInfo*>::iterator it;
    it = find_if(sceneinfo.begin(), sceneinfo.end(), eqalSceneInfo( data.mapname ) );
    if( it !=  sceneinfo.end() ) {
        CubeJRemote::SceneInfo* si = *it;
        si->setWorldSize(data.worldsize);
        si->setMapVersion(data.mapversion);
    }
    else {
        CubeJRemote::SceneInfo* si = new CubeJRemote::SceneInfo(data.mapname);
        si->setWorldSize(data.worldsize);
        si->setMapVersion(data.mapversion);
        sceneinfo.push_back( si );
    }
    listBox->updateContent();
}
