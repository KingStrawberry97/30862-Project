#include "Base.h"
#ifndef ITEM_H_
#define ITEM_H_

class Item : public Base {

private:
<<<<<<< HEAD
	char* writing;
	char* turn_on;
public:
	Item(rapidxml::xml_node<>*);
	virtual ~Item();
=======
	//std::string writing;
	//std::string turn_on;
public:
	Item(rapidxml::xml_node<>*);
	virtual ~Item();
	std::string writing;
	Trigger* turn_on;
>>>>>>> d3381c472fad9557d726ff9dd5c9b4d515ea62da
};

#endif
