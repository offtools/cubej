#include "remoteclient.h"
#include "ui_mainloop.h"

using namespace CubeJRemote;

MainLoop::MainLoop() {}

MainLoop::~MainLoop() {}

void MainLoop::timerCallback () {
    GetRemoteInterface().update();
}
