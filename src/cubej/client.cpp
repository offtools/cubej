#include "client.hpp"
#include "protocol.hpp"

namespace CubeJ
{
	Client::Client() : self(*new Peer), connected(false), messagecn(-1), messagereliable(false)
	{
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

    void Client::ackConnect(int cn, char* text) {
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
            Peer *p = new Peer(cn, text);
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
            CubeJProtocol::ReceiveMessage(type, -1, chan, p);
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


	Client& GetClient() {
		static Client client;
		return client;
	}
}
