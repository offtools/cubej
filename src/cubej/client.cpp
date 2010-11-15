#include "client.hpp"

namespace CubeJ
{
	Client::Client() : connected(false)
	{
		views.add(new dynent);
	}

	Client::~Client()
	{
		views.deletecontents();
	}

	dynent* Client::GetView(int i)
	{
		if(views.inrange(i))
			return views[i];
		return views[0];
	}

	void Client::Init()
	{
		setvar("mainmenu", false);
		setvar("hidehud", 1);
		if(!connected)
			localconnect();
	}

	void Client::Connect()
	{
		connected = true;
	}

	void Client::Disconnect()
	{
		connected = false;
	}

	void Client::Update()
	{

	}

	void Client::onEditToggle(bool on)
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

	void Client::onStartMap()
	{
		findplayerspawn(views[0], -1);
	}

	Client client;
}
