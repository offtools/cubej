#include "cube.h"

//separted main from the master modul, not functional at the moment, as we dont need it for now

int main(int argc, char **argv)
{
    if(enet_initialize()<0) fatal("Unable to initialise network module");
    atexit(enet_deinitialize);

    const char *dir = "", *ip = NULL;
    int port = 28787;
    if(argc>=2) dir = argv[1];
    if(argc>=3) port = atoi(argv[2]);
    if(argc>=4) ip = argv[3];
    defformatstring(logname)("%smaster.log", dir);
    defformatstring(cfgname)("%smaster.cfg", dir);
    path(logname);
    path(cfgname);
    logfile = fopen(logname, "a");
    if(!logfile) logfile = stdout;
    setvbuf(logfile, NULL, _IOLBF, BUFSIZ);
#ifndef WIN32
    signal(SIGUSR1, reloadsignal);
#endif
    setupserver(port, ip);
    for(;;)
    {
        if(reloadcfg)
        {
            conoutf("reloading master.cfg");
            execfile(cfgname);
            bangameservers();
            banclients();
            gengbanlist();
            reloadcfg = false;
        }

        servtime = enet_time_get();
        checkclients();
        checkgameservers();
    }

    return EXIT_SUCCESS;
}

