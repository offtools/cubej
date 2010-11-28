#include "client.hpp"

namespace CubeJ
{
	Client::Client() : connected(false)
	{
		cameras.add(new dynent);
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

	void Client::init()
	{
		setvar("mainmenu", false);
		setvar("hidehud", 1);
		if(!connected)
			localconnect();
	}

	void Client::connect()
	{
		connected = true;
	}

	void Client::disconnect()
	{
		connected = false;
	}

	void Client::update()
	{

	}

	void Client::edittoggle(bool on)
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

	void Client::startMap()
	{
		findplayerspawn(cameras[0], -1);
	}

	void Client::setCameraPosition(int i, vec o) {
		if(cameras.inrange(i)) {
			cameras[i]->o = o;
		}
	}

	vec Client::getCameraPosition(int i) {
		if(cameras.inrange(i)) {
			return cameras[i]->o;
		}
		return vec(0,0,0);
	}

	Client client;

	void setcameraposition(int *i, float *x, float *y, float *z) {
		client.setCameraPosition(*i, vec(*x,*y,*z));
	}

	void getcameraposition(int *i) {
		vec pos = client.getCameraPosition(*i);
		conoutf("camera[%d] position: %f %f %f", pos.x, pos.y, pos.z);
	}

	COMMAND(setcameraposition, "ifff");
	COMMAND(getcameraposition, "i");
}
