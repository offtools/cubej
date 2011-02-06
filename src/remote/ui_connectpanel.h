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

#ifndef __JUCER_HEADER_CONNECTPANEL_UICONNECTPANEL_23BE61C7__
#define __JUCER_HEADER_CONNECTPANEL_UICONNECTPANEL_23BE61C7__

//[Headers]     -- You can add your own extra header files here --
#include "juce_amalgamated.h"
#include "ui_mainloop.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ConnectPanel  : public Component,
                      public ButtonListener
{
public:
    //==============================================================================
    ConnectPanel (MainLoop* _mainloop);
    ~ConnectPanel();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void parentSizeChanged();


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    GroupComponent* groupComponent;
    TextEditor* editor_server;
    TextEditor* editor_port;
    Label* label_server;
    Label* label_port;
    TextButton* button_connect;

    MainLoop* mainloop;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    ConnectPanel (const ConnectPanel&);
    const ConnectPanel& operator= (const ConnectPanel&);
};


#endif   // __JUCER_HEADER_CONNECTPANEL_UICONNECTPANEL_23BE61C7__
