#ifndef REMOTE_CONNECTCOMPONENT_H
#define REMOTE_CONNECTCOMPONENT_H

#include "juce_amalgamated.h"

class ConnectComponent : public Component,
                         public ListBoxModel,
                         public ButtonListener
{
    public:
        ConnectComponent();
        ~ConnectComponent();

        void resized();
        int getNumRows();
        void paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected);
        void selectedRowsChanged (int lastRowselected);
        void buttonClicked (Button *);

    protected:

    private:
        ListBox* listBox;
        TextButton* conn;
        TextButton* disc;
        Component* hFrame;
        StretchableLayoutManager hLayout;
        StretchableLayoutManager vLayout;
};

#endif // REMOTE_CONNECTCOMPONENT_H
