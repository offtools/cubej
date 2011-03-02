//__offtools__: engine interface extensions

namespace game
{
    // hook for console code, used to redirect console output to remote interface client
    extern void redirectcout(char *line);
}
