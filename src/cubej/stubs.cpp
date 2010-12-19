#include "cubej.h"

#ifndef STANDALONE
namespace entities
{
    void editent(int i, bool local) {
    	CubeJ::EditEntity(i, local);
    }
    const char *entnameinfo(entity &e) {
    	return CubeJ::GetEntityHandler(e.type).entnameinfo(e);
    }

    const char *entname(int i) {
    	return  CubeJ::GetEntityHandler(i).entname();
    }

    int extraentinfosize() {
    	return CubeJ::EntityInfosize();
    }

    void writeent(entity &e, char *buf) {
    	CubeJ::GetEntityHandler(e.type).writeent(e, buf);
    }

    void readent(entity &e, char *buf) {
    	CubeJ::GetEntityHandler(e.type).readent(e, buf);
    }

    float dropheight(entity &e) {
    	return CubeJ::GetEntityHandler(e.type).dropheight(e);
    }

    void fixentity(extentity &e) {
    	CubeJ::GetEntityHandler(e.type).fixentity(e);
    }

    void entradius(extentity &e, bool color) {
    	CubeJ::GetEntityHandler(e.type).entradius(e, color);
    }

    bool mayattach(extentity &e) {
    	return CubeJ::GetEntityHandler(e.type).mayattach(e);
    }

    bool attachent(extentity &e, extentity &a) {
    	return CubeJ::GetEntityHandler(e.type).attachent(e, a);
    }

    bool printent(extentity &e, char *buf) {
    	return CubeJ::GetEntityHandler(e.type).printent(e, buf);
    }

    extentity *newentity() {
    	return CubeJ::NewEntity();
    }

    void deleteentity(extentity *e) {
    	CubeJ::GetEntityHandler(e->type).deleteentity(e);
    }

    void clearents() {
    	CubeJ::ClearEnts();
    }

    vector<extentity *> &getents() {
    	return CubeJ::ents;
    }

    const char *entmodel(const entity &e) {
    	return CubeJ::GetEntityHandler(e.type).entmodel(e);
    }

    void animatemapmodel(const extentity &e, int &anim, int &basetime) {
    	return CubeJ::GetEntityHandler(e.type).animatemapmodel(e, anim, basetime);
    }
}


namespace game
{
//    void parseoptions(vector<const char *> &args) {}

    void gamedisconnect(bool cleanup) { conoutf("[DEBUG] game::gamedisconnect"); CubeJ::GetClient().Disconnect(); }
    void parsepacketclient(int chan, packetbuf &p) { CubeJ::GetClient().parsePacket(chan, p); }
    void connectattempt(const char *name, const char *password, const ENetAddress &address) { conoutf("game::connectattempt");}
    void connectfail() {conoutf("game::fail");}
    void gameconnect(bool _remote)  { conoutf("[DEBUG] game::gameconnect"); CubeJ::GetClient().finishConnect(_remote); }
    bool allowedittoggle() { return true; }
    void edittoggled(bool on) { CubeJ::GetClient().editMode(on); }
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

    void updateworld() { CubeJ::GetClient().Update(); }
    void initclient() { CubeJ::GetClient().Init(); }
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
    void startmap(const char *name) { CubeJ::GetClient().startScene(); }
    void preload() {}
    float abovegameplayhud(int w, int h) { return 0; }
    void gameplayhud(int w, int h) {}
    bool canjump() { return false; }
    bool allowmove(physent *d) { return false; }
    void doattack(bool on) {}
    dynent *iterdynents(int i) { return !i ? CubeJ::GetClient().getCamera() : NULL; }
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
#endif

#include "server.hpp"

namespace game
{
    void parseoptions(vector<const char *> &args) {}
}

namespace server
{
    void *newclientinfo() {
    	return new CubeJSrv::ClientInfo();
    	return NULL;
    }

    void deleteclientinfo(void *ci) {
    	delete (CubeJSrv::ClientInfo*)ci;
    }

    void serverinit()
    {
    	CubeJSrv::GetServer().Init();
    }

    int reserveclients()
    {
    	return CubeJSrv::RESERVE_CLIENTS;
    }

    void clientdisconnect(int n) {
		CubeJSrv::GetServer().notifyDisconnect(n);
    }

    int clientconnect(int n, uint ip) {
    	return CubeJSrv::GetServer().sendServerInfo(n, false);
	}

    void localdisconnect(int n) {
		CubeJSrv::GetServer().notifyDisconnect(n);
    }

    void localconnect(int n) {
        conoutf("game::localconnect");
    	CubeJSrv::GetServer().sendServerInfo(n);
	}

    bool allowbroadcast(int n) {
        conoutf("game::allowbroadcast");
		CubeJSrv::ClientInfo *ci = (CubeJSrv::ClientInfo*)getclientinfo(n);
        return ci && ci->isConnected();
	}

    void recordpacket(int chan, void *data, int len) {}
    void parsepacket(int sender, int chan, packetbuf &p) {
		CubeJSrv::GetServer().parsePacket(sender, chan, p);
    }

    void sendservmsg(const char *s) {}

    bool sendpackets(bool force) {
		return CubeJSrv::GetServer().sendAllPackets(force);
	}

    void serverinforeply(ucharbuf &req, ucharbuf &p) {}
    void serverupdate() {
        CubeJSrv::GetServer().Update();
    }
    bool servercompatible(char *name, char *sdec, char *map, int ping, const vector<int> &attr, int np) { return true; }

	int laninfoport() {
		return CubeJSrv::CUBEJ_LANINFO_PORT;
	}

    int serverinfoport(int servport) {
    	return servport < 0 ? CubeJSrv::CUBEJ_SERVINFO_PORT : servport+1;
	}

    int serverport(int infoport) {
		return infoport < 0 ? CubeJSrv::CUBEJ_SERVER_PORT : infoport-1;
	}

    const char *defaultmaster() { return "cubej.org"; }

    int masterport() {
    	return CubeJSrv::CUBEJ_MASTER_PORT;
	}

    int numchannels() {
    	return CubeJSrv::NUM_CHANNELS;
	}

    void processmasterinput(const char *cmd, int cmdlen, const char *args) {}

    bool ispaused() { return false; }
}
