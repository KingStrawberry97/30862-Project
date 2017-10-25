#include "Base.h"
#include "Item.h"
#ifndef CONTAINER_H_
#define CONTAINER_H_

class Container : public Base {

	Item* accepts;
	Item* items;

};

#endif