#include "client.h"

namespace CubeJ
{

	Client::Client() : self(*new ClientInfo), connected(false), messagecn(-1), messagereliable(false)
	{

        registerMsgHandler( MSG_ERROR_OVERFLOW , receiveMessage<MSG_ERROR_OVERFLOW>);
        registerMsgHandler( MSG_ERROR_TAG , receiveMessage<MSG_ERROR_TAG>);
        registerMsgHandler( MSG_SND_SERVINFO , receiveMessage<MSG_SND_SERVINFO>);
        registerMsgHandler( MSG_CDIS , receiveMessage<MSG_CDIS>);
        registerMsgHandler( MSG_SND_CLIENTINFO , receiveMessage<MSG_SND_CLIENTINFO>);
        registerMsgHandler( MSG_SND_SCENESTATUS , receiveMessage<MSG_SND_SCENESTATUS>);
        registerMsgHandler( MSG_DISCOVER_REMOTE , receiveMessage<MSG_DISCOVER_REMOTE>);
        registerMsgHandler( MSG_REQ_REMOTE , receiveMessage<MSG_REQ_REMOTE>);
        registerMsgHandler( MSG_REQ_LISTMAPS, receiveMessage<MSG_REQ_LISTMAPS>);
        registerMsgHandler( MSG_CMD_CHANGESCENE, receiveMessage<MSG_CMD_CHANGESCENE>);

        clientmap[0] = '\0';
		cameras.add(new dynent);
		clients.add(&self);
	}

	Client::~Client()
	{
		cameras.deletecontents();
	}

	dynent* Client::getCamera(int i)
	{
		if(cameras.inrange(i))
			return cameras[i];
		return cameras[0];
	}

	void Client::Init()
	{
	    conoutf("[DEBUG] Client::Init");
		setvar("mainmenu", false);
		setvar("hidehud", 1);

		if(!connected)
			localconnect();
	}

	void Client::sendAllMessages(bool force) {
        static int lastupdate = -1000;
        if(totalmillis - lastupdate < 33 && !force) return; // don't update faster than 30fps
        lastupdate = totalmillis;
		///TODO:
		//loopv(packets) packets[i]->send(); ??
        flushclient();
	}

	void Client::initConnect(int cn, int protocol)
	{
	    conoutf("Client::initConnect");
		if(protocol!=CubeJProtocol::PROTOCOL_VERSION)
		{
			conoutf(CON_ERROR, "you are using a different game protocol (you: %d, server: %d)", CubeJProtocol::PROTOCOL_VERSION, protocol);
			disconnect();
			return;
		}

		self.clientnum = cn;
        CubeJProtocol::MsgDataType<CubeJProtocol::MSG_REQ_CONNECT> data(self.name);
        SendMessage(data);
    }

    void Client::ackConnect(int cn, int type, char* text) {
        filtertext(text, text, false);
		if(!text[0]) copystring(text, "unnamed");

        if(cn < 0 || cn > MAXCLIENTS)
        {
            neterr("clientnum", false);
            return;
        }

        if(cn == self.clientnum) {
            return;
        }

        while(cn >= clients.length()) clients.add(NULL);
        if(!clients[cn])
        {
            ClientInfo *p = new ClientInfo(cn, type, text);
            clients[cn] = p;
        }
    }

	void Client::finishConnect(bool _remote)
	{
		connected = true;
		remote = _remote;
	}

	void Client::Disconnect()
	{
		connected = false;
	}

	void Client::Update()
	{
        if(!curtime) {
            gets2c();
            if(self.clientnum>=0) sendAllMessages(false);
            return;
        }
        gets2c();
        if(self.clientnum>=0) sendAllMessages();
	}

	void Client::editMode(bool on)
	{
		if(on)
		{
			setvar("hidehud", 0);
			setvar("grabinput", 1);
		}
		else
		{
			setvar("hidehud", 1);
			setvar("grabinput", 0);
		}
	}

	void Client::startScene()
	{
		findplayerspawn(cameras[0], -1);
	}

	void Client::parsePacket(int chan, packetbuf &p) {
        while(p.remaining())
        {
            CubeJProtocol::MSG_TYPE type = (CubeJProtocol::MSG_TYPE)getint(p);
            receive(type, -1, chan, p);
        }
	}

    void Client::requestScene(bool srv_has_scene) {
        if(srv_has_scene) {
            //request scene info
        }
        else {
            //change scene
        }
    }

    void Client::loadScene(const char* name) {
        if(load_world(name)) {
            copystring(clientmap, name);
        }
        CubeJProtocol::MsgDataType<CubeJProtocol::MSG_SND_SCENEINFO> data(clientmap, getworldsize(), getmapversion());
        SendMessage(data);
    }

	void Client::connectRemoteInterface(int n) {
		conoutf("[DEBUG] Client::connectRemoteInterface");
		//TODO: check remote client
		clients.add(new ClientInfo (n, CLIENT_TYPE_REMOTE, NULL));
        CubeJProtocol::MsgDataType<CubeJProtocol::MSG_ACK_REMOTE> data(n);
        SendMessage(data);
	}

	Client& GetClient() {
		static Client client;
		return client;
	}

    char* retstr;

	void testexec( char* s) {
        retstr = newstring(executeret(s));
        conoutf("TESTexec: %s", retstr);
	}

	void testresult() {
        conoutf("TEST: %s", retstr);
	}

	ICOMMAND(testresult, "", (), testresult());
	ICOMMAND(testexec, "s", ( char* s ), testexec(s));
}
