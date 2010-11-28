#ifndef CUBEJ_ENTITIES_H_
#define CUBEJ_ENTITIES_H_

#include "cube.h"

enum				          	// static entity types
{
	NOTUSED = ET_EMPTY,         // entity slot not in use in map
	LIGHT = ET_LIGHT,           // lightsource, attr1 = radius, attr2 = intensity
	MAPMODEL = ET_MAPMODEL,     // attr1 = angle, attr2 = idx
	PLAYERSTART = ET_PLAYERSTART,// attr1 = angle, attr2 = team
	ENVMAP = ET_ENVMAP,         // attr1 = radius
	PARTICLES = ET_PARTICLES,
	MAPSOUND = ET_SOUND,
	SPOTLIGHT = ET_SPOTLIGHT,
	TELEPORT = ET_GAMESPECIFIC,  // attr1 = idx
	POSITION,
	NUMHANDLER
};

namespace CubeJ
{
	class EntityHandler
	{
	public:

		EntityHandler();
		virtual ~EntityHandler() = 0;
		virtual const char *entnameinfo(entity &e);
		virtual const char *entname();
		virtual void writeent(entity &e, char *buf);
		virtual void readent(entity &e, char *buf);
		virtual float dropheight(entity &e);
		virtual void fixentity(extentity &e);
		virtual void entradius(extentity &e, bool color);
		virtual bool mayattach(extentity &e);
		virtual bool attachent(extentity &e, extentity &a);
		virtual bool printent(extentity &e, char *buf);
		virtual void deleteentity(extentity *e);
		virtual const char *entmodel(const entity &e);
		virtual void animatemapmodel(const extentity &e, int &anim, int &basetime);

	protected:
		//name for entity type, used in newent command
		const char* name;

		//additional name
		const char* nameinfo;

		//path to model used to visualize ent (pickups in sauer)
		const char* model;

		//dropheight of an model
		float drop;

		//attach or group entities (red eclipse uses this alot)
		bool canattach;
		bool attached;

		//print additional info's when using entget command
		bool print;
	};

	EntityHandler& GetEntityHandler(int type);

    extentity *NewEntity();

    void ClearEnts();

    vector<extentity *> &GetEnts();

    int EntityInfosize();

    void EditEntity(int i, bool local);
}

#endif
