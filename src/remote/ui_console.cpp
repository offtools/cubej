#include "ui_console.h"
#include "dispatcher.h"

Console::Console() : commandline(0), console(0), send(0)
{
    commandline = new TextEditor();
    commandline->setMultiLine(false, false);
    commandline->setReadOnly(false);
    commandline->setScrollbarsShown(false);
    commandline->addListener(this);
    commandline->setText (String::empty, true);

    console = new TextEditor();
    console->setMultiLine(true, true);
    console->setReadOnly(true);
    console->setScrollbarsShown(true);
    console->setText (T("... CubeJ Console ..."), true);

    send = new TextButton(T("send"), T("Send Command"));
    send->addButtonListener(this);

    frame = new Component();
    verticalLayout.setItemLayout (0, -0.8, -1.0, -1.0);
    verticalLayout.setItemLayout (1, 30, 30, 30);
    horizontalLayout.setItemLayout(0, -0.8, -1.0, -1.0);
    horizontalLayout.setItemLayout (1, 80, 80, 80);

    addAndMakeVisible (frame);
    addAndMakeVisible (console);
    frame->addAndMakeVisible (commandline);
    frame->addAndMakeVisible (send);
}

Console::~Console()
{
    deleteAllChildren();
}

void Console::resized() {
    Component* verticalComponents[] = { console, frame };
    Component* horizontalComponents[] = { commandline, send };

    console->setBounds(0,0,getWidth(),getHeight());

    verticalLayout.layOutComponents (verticalComponents, 2,
                                 0,
                                 0,
                                 getWidth(),
                                 getHeight(),
                                 true,
                                 true);

    int currentHeight = verticalLayout.getItemCurrentAbsoluteSize(0);

    horizontalLayout.layOutComponents (horizontalComponents, 2,
                                     0,
                                     0,
                                     getWidth(),
                                     getHeight() - currentHeight,
                                     false,
                                     true);

}

const String Console::getCurrentCommand() {
    return commandline->getText();
}

void Console::addOnSendCommandListener(AppMessageCommandListener<Console> *listener) {
    sendlistener = listener;
}

void Console::onReceiveRCout(int sender, int channel, packetbuf& p) {
    CubeJ::MsgDataType<CubeJ::MSG_FWD_RCOUT> data(p);
    console->setCaretPosition (console->getText ().length ());
    String newline( data.line );
    console->insertTextAtCaret( newline + T("\n"));
}

void Console::buttonClicked(Button *button) {
    if ( button == send ) {
        std::cout << "Console::buttonClicked" << std::endl;
    }
}

void Console::clearCommandLine() {
    commandline->setText(String::empty);
}

void Console::textEditorReturnKeyPressed (TextEditor &editor) {
    std::cout << "Console::textEditorReturnKeyPressed: " << commandline->getText() << std::endl;
    sendlistener->postMessage(NetworkDispatcher::AppCommandID::MSG_FWD_RCIN, this);
}

void Console::textEditorEscapeKeyPressed (TextEditor &editor) {
    std::cout << "Console::textEditorEscapeKeyPressed" << std::endl;
    editor.setText(String::empty);
}

void Console::textEditorTextChanged (TextEditor &editor) {}

void Console::textEditorFocusLost (TextEditor &editor) {}
