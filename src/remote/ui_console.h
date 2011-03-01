#ifndef REMOTE_CONSOLE_H
#define REMOTE_CONSOLE_H

#include "protocol.h"
#include "config.h"
#include "juce_amalgamated.h"
#include "ui_listener.h"

class Console : public Component, public ButtonListener, public TextEditorListener
{
    public:
        Console();
        ~Console();

        void resized();
        const String getCurrentCommand();
        void clearCommandLine();
        void addOnSendCommandListener(AppMessageCommandListener<Console> *sendlistener);
        void onReceiveRCout(int sender, int channel, packetbuf& p);

    private:
        TextEditor* commandline;
        TextEditor* console;
        TextButton* send;

        Component* frame;
        StretchableLayoutManager horizontalLayout;
        StretchableLayoutManager verticalLayout;

        AppMessageCommandListener<Console> *sendlistener;

        void buttonClicked(Button *button);
        void textEditorReturnKeyPressed (TextEditor &editor);
        void textEditorEscapeKeyPressed (TextEditor &editor);
        void textEditorTextChanged (TextEditor &editor);
        void textEditorFocusLost (TextEditor &editor);
};

#endif // REMOTE_CONSOLE_H
