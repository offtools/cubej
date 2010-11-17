#include "entities.hpp"

namespace CubeJ
{
	vector<extentity *> ents;

	extentity *NewEntity() { return new extentity; }

	void ClearEnts() { ents.deletecontents(); }

	vector<extentity *> &GetEnts() { return ents; }

	int EntityInfosize() { return 0; }

	void EditEntity(int i, bool local)
	{

	}

	EntityHandler::EntityHandler()
	{
	}

	EntityHandler::~EntityHandler()	{}

	const char* EntityHandler::entnameinfo(entity &e)
	{
		return "";
	}

	inline const char* EntityHandler::entname()
	{
		return name;
	}

	void EntityHandler::writeent(entity &e, char *buf)
	{

	}

	void EntityHandler::readent(entity &e, char *buf)
	{

	}

	float EntityHandler::dropheight(entity &e)
	{
		return 0.0f;
	}

	void EntityHandler::fixentity(extentity &e)
	{

	}

	void EntityHandler::entradius(extentity &e, bool color)
	{

	}

	bool EntityHandler::mayattach(extentity &e)
	{
		return false;
	}

	bool EntityHandler::attachent(extentity &e, extentity &a)
	{
		return false;
	}

	bool EntityHandler::printent(extentity &e, char *buf)
	{
		return false;
	}

	void EntityHandler::deleteentity(extentity *e)
	{
		delete e;
	}

	const char *EntityHandler::entmodel(const entity &e)
	{
		return "";
	}

	void EntityHandler::animatemapmodel(const extentity &e, int &anim, int &basetime)
	{

	}

	template <int T = NOTUSED> class EntityHandlerType : public EntityHandler
	{
	public:
		EntityHandlerType()
		{
			name = newstring("none?");
		}

		~EntityHandlerType()
		{
			delete[] name;
		}
	};

	template <> class EntityHandlerType<LIGHT> : public EntityHandler
	{
	public:
		EntityHandlerType()
		{
			conoutf("EntityHandlerType<LIGHT>::EntityHandlerType");
			name = newstring("light");
		}
		~EntityHandlerType()
		{
			delete[] name;
		}
	};

	template <> class EntityHandlerType<MAPMODEL> : public EntityHandler
	{
	public:
		EntityHandlerType()
		{
			name = newstring("mapmodel");
		}

		~EntityHandlerType()
		{
			delete[] name;
		}
	};

	template <> class EntityHandlerType<PLAYERSTART> : public EntityHandler
	{
	public:
		EntityHandlerType()
		{
			name = newstring("start");
		}

		~EntityHandlerType()
		{
			delete[] name;
		}
	};

	template <> class EntityHandlerType<ENVMAP> : public EntityHandler
	{
	public:
		EntityHandlerType()
		{
			name = newstring("envmap");
		}

		~EntityHandlerType()
		{
			delete[] name;
		}

		const char *entname()
		{
			return name;
		}
	};

	template <> class EntityHandlerType<PARTICLES> : public EntityHandler
	{
	public:
		EntityHandlerType()
		{
			name = newstring("envmap");
		}

		~EntityHandlerType()
		{
			delete[] name;
		}
	};

	template <> class EntityHandlerType<MAPSOUND> : public EntityHandler
	{
	public:
		EntityHandlerType()
		{
			name = newstring("mapsound");
		}

		~EntityHandlerType()
		{
			delete[] name;
		}
	};

	template <> class EntityHandlerType<SPOTLIGHT> : public EntityHandler
	{
	public:
		EntityHandlerType()
		{
			name = newstring("spotlight");
		}

		~EntityHandlerType()
		{
			delete[] name;
		}
	};

	template <> class EntityHandlerType<TELEPORT> : public EntityHandler
	{
	public:
		EntityHandlerType()
		{
			name = newstring("teleport");
		}

		~EntityHandlerType()
		{
			delete[] name;
		}
	};

	template <> class EntityHandlerType<POSITION> : public EntityHandler
	{
	public:
		EntityHandlerType()
		{
			name = newstring("position");
		}

		~EntityHandlerType()
		{
			delete[] name;
		}
	};

	static EntityHandler* handler[] =
	{
		new EntityHandlerType<NOTUSED>,
		new EntityHandlerType<LIGHT>,
		new EntityHandlerType<MAPMODEL>,
		new EntityHandlerType<PLAYERSTART>,
		new EntityHandlerType<ENVMAP>,
		new EntityHandlerType<PARTICLES>,
		new EntityHandlerType<MAPSOUND>,
		new EntityHandlerType<SPOTLIGHT>,
		new EntityHandlerType<TELEPORT>,
		new EntityHandlerType<POSITION>
	};

	EntityHandler& GetEntityHandler(int type)
	{
		return type && type < NUMHANDLER ? *handler[type] : *handler[NOTUSED];
	}
}
