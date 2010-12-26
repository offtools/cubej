#include "engine.h"

extern vector<const char *> gameargs;

#ifdef STANDALONE
int main(int argc, char* argv[])
{
    setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
    if(enet_initialize()<0) fatal("Unable to initialise network module");
    atexit(enet_deinitialize);
    enet_time_set(0);
    for(int i = 1; i<argc; i++) if(argv[i][0]!='-' || !serveroption(argv[i])) gameargs.add(argv[i]);
    game::parseoptions(gameargs);
    initserver(true, true);
    return 0;
}
#endif
