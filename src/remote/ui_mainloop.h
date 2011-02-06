#ifndef REMOTE_MAINLOOP_H
#define REMOTE_MAINLOOP_H

#include "juce_amalgamated.h"

class MainLoop : public Timer {
public:
    MainLoop();
    ~MainLoop();

    void timerCallback();
private:

};

#endif
