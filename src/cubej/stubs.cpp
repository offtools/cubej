#include "cubej.h"

namespace entities
{
    void editent(int i, bool local) {}
    const char *entnameinfo(entity &e) { return NULL; }
    const char *entname(int i) { return NULL; }
    int extraentinfosize() { return 0; }
    void writeent(entity &e, char *buf) {}
    void readent(entity &e, char *buf) {}
    float dropheight(entity &e) { return 0; }
    void fixentity(extentity &e) {}
    void entradius(extentity &e, bool color) {}
    bool mayattach(extentity &e) { return false; }
    bool attachent(extentity &e, extentity &a) { return false; }
    bool printent(extentity &e, char *buf) { return false; }
    extentity *newentity() { return NULL; }
    void deleteentity(extentity *e) {}
    void clearents() {}
    vector<extentity *> &getents() { return CubeJ::ents; }
    const char *entmodel(const entity &e) { return NULL; }
    void animatemapmodel(const extentity &e, int &anim, int &basetime) {}
}

namespace game
{
    void parseoptions(vector<const char *> &args) {}

    void gamedisconnect(bool cleanup) { conoutf("[DEBUG] game::gamedisconnect"); CubeJ::client.Disconnect(); }
    void parsepacketclient(int chan, packetbuf &p) {}
    void connectattempt(const char *name, const char *password, const ENetAddress &address) {}
    void connectfail() {}
    void gameconnect(bool _remote)  { conoutf("[DEBUG] game::gameconnect"); CubeJ::client.Connect(); }
    bool allowedittoggle() { return true; }
    void edittoggled(bool on) { CubeJ::client.onEditToggle(on); }
    void writeclientinfo(stream *f) {}
    void toserver(char *text) {}
    void changemap(const char *name) {}
    void forceedit(const char *name) {}
    bool ispaused() { return false; }

    const char *gameident() { return "cubej"; }
    const char *savedconfig() { return "config.cfg"; }
    const char *restoreconfig() { return "restore.cfg"; }
    const char *defaultconfig() { return "data/defaults.cfg"; }
    const char *autoexec() { return "autoexec.cfg"; }
    const char *savedservers() { return "servers.cfg"; }
    void loadconfigs() { execfile("auth.cfg", false); }

    void updateworld() { CubeJ::client.Update(); }
    void initclient() { CubeJ::client.Init(); }
    void physicstrigger(physent *d, bool local, int floorlevel, int waterlevel, int material) {}
    void bounced(physent *d, const vec &surface) {}
    void edittrigger(const selinfo &sel, int op, int arg1, int arg2, int arg3) {}
    void vartrigger(ident *id) {}
    void dynentcollide(physent *d, physent *o, const vec &dir) {}
    const char *getclientmap() { return NULL; }
    const char *getmapinfo() { return NULL; }
    void resetgamestate() {}
    void suicide(physent *d) {}
    void newmap(int size) {}
    void startmap(const char *name) { CubeJ::client.onStartMap(); }
    void preload() {}
    float abovegameplayhud(int w, int h) { return 0; }
    void gameplayhud(int w, int h) {}
    bool canjump() { return false; }
    bool allowmove(physent *d) { return false; }
    void doattack(bool on) {}
    dynent *iterdynents(int i) { return !i ? CubeJ::client.GetView() : NULL; }
    int numdynents() { return 1; }
    void rendergame(bool mainpass) {}
    void renderavatar() {}
    void writegamedata(vector<char> &extras) {}
    void readgamedata(vector<char> &extras) {}
    int clipconsole(int w, int h) { return 0; }
    void g3d_gamemenus() {}
    const char *defaultcrosshair(int index) { return "data/crosshair.png"; }
    int selectcrosshair(float &r, float &g, float &b) { return 0; }
    void lighteffects(dynent *d, vec &color, vec &dir) {}
    void setupcamera() {}
    bool detachcamera() { return false; }
    bool collidecamera() { return false; }
    void adddynlights() {}
    void particletrack(physent *owner, vec &o, vec &d) {}
    void dynlighttrack(physent *owner, vec &o, vec &hud) {}
    bool serverinfostartcolumn(g3d_gui *g, int i) { return true; }
    void serverinfoendcolumn(g3d_gui *g, int i) {}
    bool serverinfoentry(g3d_gui *g, int i, const char *name, int port, const char *desc, const char *map, int ping, const vector<int> &attr, int np) { return false; }
    bool needminimap() { return false; }
}

namespace server
{
    void *newclientinfo() { return NULL; }
    void deleteclientinfo(void *ci) {}
    void serverinit() {}
    int reserveclients() { return 0; }
    void clientdisconnect(int n) {}
    int clientconnect(int n, uint ip) { return DISC_NONE; }
    void localdisconnect(int n) {}
    void localconnect(int n) {}
    bool allowbroadcast(int n) { return false; }
    void recordpacket(int chan, void *data, int len) {}
    void parsepacket(int sender, int chan, packetbuf &p) {}
    void sendservmsg(const char *s) {}
    bool sendpackets(bool force) { return false; }
    void serverinforeply(ucharbuf &req, ucharbuf &p) {}
    void serverupdate() {}
    bool servercompatible(char *name, char *sdec, char *map, int ping, const vector<int> &attr, int np) { return true; }
	int laninfoport() { return 0; }
    int serverinfoport(int servport) { return 0; }
    int serverport(int infoport) { return 0; }
    const char *defaultmaster() { return ""; }
    int masterport() { return 0; }
    int numchannels() { return 3; }
    void processmasterinput(const char *cmd, int cmdlen, const char *args) {}
    bool ispaused() { return false; }
}
