#ifndef REMOTE_SCENECOMPONENT_H
#define REMOTE_SCENECOMPONENT_H

#include "scene.h"
#include "juce_amalgamated.h"

class SceneComponent : public Component,
                       public ListBoxModel,
                       public CubeJRemote::SceneMgr,
                       public ChangeBroadcaster,
                       public ChangeListener,
                       public ButtonListener
{
    public:
        SceneComponent();
        ~SceneComponent();

        void resized();
        int getNumRows();
        void paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected);
        void selectedRowsChanged (int lastRowselected);
        void updateSceneListing(const char* name);
        void changeListenerCallback (void *);
        void buttonClicked (Button *);

    protected:
    private:
        ListBox* listBox;
        TextButton* loadbutton;
        TextButton* savebutton;
        TextButton* newbutton;
        StretchableLayoutManager layout;
};

#endif // REMOTE_SCENECOMPONENT_H
