#include "ui_scenecomponent.h"

SceneComponent::SceneComponent()
{
    listBox = new ListBox (T("SCENES"), this);
    listBox->setRowHeight (28);
    addChangeListener(this);

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
    return getNumScenes();
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
    g.drawText ( getSceneInfo(rowNumber)->getSceneName(),
                4, 0, width - 4, height,
                Justification::centredLeft, true);

}

void SceneComponent::selectedRowsChanged(int) {

}

void SceneComponent::updateSceneListing(const char* name) {
    std::cout << "SceneComponent::updateSceneListing: " << name << std::endl;
    loopv(scenes) if( strcmp(name, scenes[i]->getSceneName()) == 0) {
        //update existing

        return;
    }
    scenes.add(new CubeJRemote::SceneInfo(name));
    sendChangeMessage (this);
}

void SceneComponent::buttonClicked (Button *button) {
    const SparseSet<int> selection = listBox->getSelectedRows ();
    if(selection.isEmpty())
        return;
    std::cout << "selection: " << selection[0] << " " << getSceneInfo(selection[0])->getSceneName() << std::endl;
    loadScene(selection[0]);
}

void SceneComponent::changeListenerCallback (void *source) {
    listBox->updateContent();
}
