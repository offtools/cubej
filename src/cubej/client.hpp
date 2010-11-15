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

			void Init();
			void Update();
			void Connect();
			void Disconnect();

			dynent* GetView(int num = 0);

			void onEditToggle(bool on);
			void onStartMap();
		protected:
		private:
			//the client holds one entity for the engine, we just use this to provide a camera
			bool connected;
			vector<dynent*> views;
	};
}

#endif // CUBEJ_CLIENT_H_INCLUDED
