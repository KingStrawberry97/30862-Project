#include "Base.h"
#include "Border.h"
#include "Container.h"
#include "Item.h"
#include "Creature.h"

#ifndef ROOM_H_
#define ROOM_H_

class Room : public Base {

	char* type;
	Border* borders;
	Creature* creatures;
	Container* containers;
	Item* items;

};

#endif