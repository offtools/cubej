#include "client.h"
#include "protocol.h"

namespace CubeJ
{
    template <MSG_TYPE N> void receiveMessage(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<MSG_ERROR_OVERFLOW>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<MSG_ERROR_TAG>(int sender, int channel, packetbuf& p) { p.cleanup(); }

    template <> void receiveMessage<MSG_SND_SERVINFO>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        int protocol = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_SND_SERVINFO>");
        CubeJ::GetClient().initConnect(clientnum, protocol);
	}

    template <> void receiveMessage<MSG_REQ_CONNECT>(int sender, int channel, packetbuf& p) {
        conoutf("[DEBUG] receiveMessage<MSG_REQ_CONNECT>");
        char text[MAXTRANS];
        getstring(text, p);
        filtertext(text, text, false, MAXNAMELEN);
        conoutf("[DEBUG] received by client - error %d:%s", sender, text);
    }

    template <> void receiveMessage<MSG_CDIS>(int sender, int channel, packetbuf& p) {
        int clientnum = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_CDIS>: %d", clientnum);
    }

    template <> void receiveMessage<MSG_SND_CLIENTINFO>(int sender, int channel, packetbuf& p) {
        int cn = getint(p);
        char text[MAXTRANS];
        getstring(text, p);
		CubeJ::GetClient().ackConnect(cn, text);
        conoutf("[DEBUG] receiveMessage<MSG_SND_CLIENTINFO>: %d %s", cn, text);
    }

    template <> void receiveMessage<MSG_SND_SCENESTATUS>(int sender, int channel, packetbuf& p) {
        int hasscene = getint(p);
        conoutf("[DEBUG] receiveMessage<MSG_SND_SCENESTATUS> hasscene: %d", hasscene);
    }

	Client::Client() : self(*new ClientInfo), connected(false), messagecn(-1), messagereliable(false)
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
	    conoutf("[DEBUG] Client::Init");
		setvar("mainmenu", false);
		setvar("hidehud", 1);
        CubeJProtocol::Init();
        CubeJProtocol::RegisterMsgHandler( MSG_ERROR_OVERFLOW , receiveMessage<MSG_ERROR_OVERFLOW>);
        CubeJProtocol::RegisterMsgHandler( MSG_ERROR_TAG , receiveMessage<MSG_ERROR_TAG>);
        CubeJProtocol::RegisterMsgHandler( MSG_SND_SERVINFO , receiveMessage<MSG_SND_SERVINFO>);
        CubeJProtocol::RegisterMsgHandler( MSG_CDIS , receiveMessage<MSG_CDIS>);
        CubeJProtocol::RegisterMsgHandler( MSG_SND_CLIENTINFO , receiveMessage<MSG_SND_CLIENTINFO>);
        CubeJProtocol::RegisterMsgHandler( MSG_SND_SCENESTATUS , receiveMessage<MSG_SND_SCENESTATUS>);
        CubeJProtocol::RegisterMsgHandler( MSG_REQ_CONNECT , receiveMessage<MSG_REQ_CONNECT>);

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
            ClientInfo *p = new ClientInfo(cn, text);
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
