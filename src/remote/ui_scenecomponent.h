#ifndef REMOTE_SCENECOMPONENT_H
#define REMOTE_SCENECOMPONENT_H

#include "config.h"
#include "juce_amalgamated.h"

class SceneComponent : public Component,
                       public ListBoxModel
{
    public:
        SceneComponent();
        ~SceneComponent();

        void resized();
        int getNumRows();
        void paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected);
        void selectedRowsChanged (int lastRowselected);
        //void updateSceneListing(const char *name);

        //Callback
        //void receiveMessageListMaps(int sender, int channel, packetbuf& p);

        std::string getSelectedSceneName();

        //=============Load Button Connector==============================
        void connectLoadListener(ButtonListener* listener);

    protected:
    private:
        ListBox* listBox;
        TextButton* loadbutton;
        StretchableLayoutManager layout;
};

#endif // REMOTE_SCENECOMPONENT_H
