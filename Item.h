#include "Base.h"
#ifndef ITEM_H_
#define ITEM_H_

class Item : public Base {

private:
	char* writing;
	char* turn_on;
public:
	Item(rapidxml::xml_node<>*);
	virtual ~Item();
};

#endif
