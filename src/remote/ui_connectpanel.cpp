/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  29 Jan 2011 12:43:24am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]
#include "remoteclient.h"
#include "ui_connectpanel.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ConnectPanel::ConnectPanel (MainLoop* _mainloop)
    : groupComponent (0),
      editor_server (0),
      editor_port (0),
      label_server (0),
      label_port (0),
      button_connect (0),
      mainloop(_mainloop)
{
    addAndMakeVisible (groupComponent = new GroupComponent (T("CONNECT_GROUP"),
                                                            T("Connect")));
    groupComponent->setTextLabelPosition (Justification::centredLeft);

    groupComponent->addAndMakeVisible (editor_server = new TextEditor (T("SERVER_EDITOR")));
    editor_server->setMultiLine (false);
    editor_server->setReturnKeyStartsNewLine (false);
    editor_server->setReadOnly (false);
    editor_server->setScrollbarsShown (false);
    editor_server->setCaretVisible (true);
    editor_server->setPopupMenuEnabled (false);
    editor_server->setText (T("localhost"));

    groupComponent->addAndMakeVisible (editor_port = new TextEditor (T("PORT_EDITOR")));
    editor_port->setMultiLine (false);
    editor_port->setReturnKeyStartsNewLine (false);
    editor_port->setReadOnly (false);
    editor_port->setScrollbarsShown (false);
    editor_port->setCaretVisible (true);
    editor_port->setPopupMenuEnabled (false);
    editor_port->setText (T("26785"));

    groupComponent->addAndMakeVisible (label_server = new Label (T("SERVER_LABEL"),
                                                 T("Server:")));
    label_server->setFont (Font (15.0000f, Font::plain));
    label_server->setJustificationType (Justification::centredLeft);
    label_server->setEditable (false, false, false);
    label_server->setColour (TextEditor::textColourId, Colours::black);
    label_server->setColour (TextEditor::backgroundColourId, Colour (0x0));

    groupComponent->addAndMakeVisible (label_port = new Label (T("PORT_LABEL"),
                                               T("Port:")));
    label_port->setFont (Font (15.0000f, Font::plain));
    label_port->setJustificationType (Justification::centredLeft);
    label_port->setEditable (false, false, false);
    label_port->setColour (TextEditor::textColourId, Colours::black);
    label_port->setColour (TextEditor::backgroundColourId, Colour (0x0));

    groupComponent->addAndMakeVisible (button_connect = new TextButton (T("BUTTON_CONNECT")));
    button_connect->setButtonText (T("connect"));
    button_connect->addButtonListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (800, 50);

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ConnectPanel::~ConnectPanel()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (groupComponent);
    deleteAndZero (editor_server);
    deleteAndZero (editor_port);
    deleteAndZero (label_server);
    deleteAndZero (label_port);
    deleteAndZero (button_connect);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ConnectPanel::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ConnectPanel::resized()
{
    groupComponent->setBounds (0, 0, proportionOfWidth (1.0000f), 50);
    editor_server->setBounds (((0) + 16) + roundFloatToInt ((80) * 1.0000f), (16) + roundFloatToInt ((24) * 0.0000f), 150, 24);
    editor_port->setBounds (((((0) + 16) + roundFloatToInt ((80) * 1.0000f)) + roundFloatToInt ((150) * 1.2000f)) + roundFloatToInt ((80) * 1.0000f), (((16) + roundFloatToInt ((24) * 0.0000f)) + roundFloatToInt ((24) * 0.0000f)) + roundFloatToInt ((24) * 0.0000f), 150, 24);
    label_server->setBounds ((0) + 16, 16, 80, 24);
    label_port->setBounds ((((0) + 16) + roundFloatToInt ((80) * 1.0000f)) + roundFloatToInt ((150) * 1.2000f), ((16) + roundFloatToInt ((24) * 0.0000f)) + roundFloatToInt ((24) * 0.0000f), 80, 24);
    button_connect->setBounds ((((((0) + 16) + roundFloatToInt ((80) * 1.0000f)) + roundFloatToInt ((150) * 1.2000f)) + roundFloatToInt ((80) * 1.0000f)) + roundFloatToInt ((150) * 1.2000f), ((((16) + roundFloatToInt ((24) * 0.0000f)) + roundFloatToInt ((24) * 0.0000f)) + roundFloatToInt ((24) * 0.0000f)) + roundFloatToInt ((24) * 0.0000f), 150, 24);

    //setBounds(0,0, getParentComponent()->getHeight(), 50);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ConnectPanel::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == button_connect)
    {
        //[UserButtonCode_button_connect] -- add your button handler code here..
        const char* server = editor_server->getText().toCString();
        const int port = editor_port->getText().getIntValue ();
        CubeJRemote::RemoteClient& remote = CubeJRemote::GetRemoteClient();


        remote.setServername(server);
        remote.setServerport(port);
        remote.connectWithServer();

        mainloop->startTimer(50);
        setEnabled(false);
        //[/UserButtonCode_button_connect]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void ConnectPanel::parentSizeChanged()
{
    //[UserCode_parentSizeChanged] -- Add your code here...
    //[/UserCode_parentSizeChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ConnectPanel" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="0" initialWidth="800" initialHeight="50">
  <METHODS>
    <METHOD name="parentSizeChanged()"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffffffff"/>
  <GROUPCOMPONENT name="CONNECT_GROUP" id="45caf54557cb9b40" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 0 100% 50" title="Connect"
                  textpos="33"/>
  <TEXTEDITOR name="SERVER_EDITOR" id="d5556d71d14f4023" memberName="editor_server"
              virtualName="" explicitFocusOrder="0" pos="100% 0% 150 24" posRelativeX="ca6410503ed6d777"
              posRelativeY="ca6410503ed6d777" initialText="localhost" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="0" caret="1" popupmenu="0"/>
  <TEXTEDITOR name="PORT_EDITOR" id="1b2cc8a5c2232940" memberName="editor_port"
              virtualName="" explicitFocusOrder="0" pos="100% 0% 150 24" posRelativeX="6db197d575c67464"
              posRelativeY="6db197d575c67464" initialText="26785" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="0" caret="1" popupmenu="0"/>
  <LABEL name="SERVER_LABEL" id="ca6410503ed6d777" memberName="label_server"
         virtualName="" explicitFocusOrder="0" pos="16 16 80 24" posRelativeX="45caf54557cb9b40"
         edTextCol="ff000000" edBkgCol="0" labelText="Server:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="PORT_LABEL" id="6db197d575c67464" memberName="label_port"
         virtualName="" explicitFocusOrder="0" pos="120% 0% 80 24" posRelativeX="d5556d71d14f4023"
         posRelativeY="d5556d71d14f4023" edTextCol="ff000000" edBkgCol="0"
         labelText="Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="BUTTON_CONNECT" id="f6f86504abd31633" memberName="button_connect"
              virtualName="" explicitFocusOrder="0" pos="120% 0% 150 24" posRelativeX="1b2cc8a5c2232940"
              posRelativeY="1b2cc8a5c2232940" buttonText="connect" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
