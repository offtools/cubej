#ifndef REMOTE_SCENECOMPONENT_H
#define REMOTE_SCENECOMPONENT_H

#include "config.h"
#include "scene.h"
#include "juce_amalgamated.h"
#include "ui_listener.h"

class SceneComponent : public Component,
                       public ListBoxModel,
                       public ButtonListener
{
    public:
        SceneComponent();
        ~SceneComponent();

        void resized();
        int getNumRows();
        void paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected);
        void selectedRowsChanged (int lastRowselected);

        //Callback
        void CallbackListScenes(int sender, int channel, packetbuf& p);
        void CallbackSceneInfo(int sender, int channel, packetbuf& p);

        //Widget Callback
        void buttonClicked (Button *);

        std::string getSelectedSceneName();

        //=============Load Button Connector==============================
        void addLoadListener(AppMessageCommandListener<SceneComponent> *listener);

    protected:
    private:
        ListBox* listBox;
        TextButton* loadbutton;
        StretchableLayoutManager layout;

        //Command Listener
        AppMessageCommandListener<SceneComponent> *loadlistener;

        //Scenes
        std::vector<CubeJRemote::SceneInfo*> sceneinfo;
};

#endif // REMOTE_SCENECOMPONENT_H
