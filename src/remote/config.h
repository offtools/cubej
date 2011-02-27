#ifndef REMOTE_CONFIG_H
#define REMOTE_CONFIG_H

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

#include "cube.h"
#include "clientinfo.h"

namespace AppInfo
{
    const char* const  projectName    = "CubejRemoteInterface";
    const char* const  versionString  = "0.0.1";
    const int          versionNumber  = 0x00001;
};

//==============Layout=============

namespace AppLayout
{
    const int HorizontalSpacing = 3;
    const int VerticalSpacing = 5;
};
#endif
