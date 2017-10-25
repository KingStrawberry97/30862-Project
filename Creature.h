#include "Base.h"
#include "Item.h"
#ifndef CREATURE_H_
#define CREATURE_H_

class Creature : public Base {

	Item* vulnerability;
	char* attack;

};

#endif