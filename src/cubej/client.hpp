#ifndef CUBEJ_CLIENT_H_INCLUDED
#define CUBEJ_CLIENT_H_INCLUDED

#include "cube.h"

namespace CubeJ
{
	class Client
	{
		public:
			Client();
			~Client();

			void init();
			void update();
			void connect();
			void disconnect();

			dynent* getCamera(int num = 0);
			void setCameraPosition(int num, vec o);
			vec getCameraPosition(int num = 0);

			void edittoggle(bool on);
			void startMap();
		protected:
		private:
			//the client holds one entity for the engine, we just use this to provide a camera
			bool connected;
			vector<dynent*> cameras;
	};
}

#endif // CUBEJ_CLIENT_H_INCLUDED
