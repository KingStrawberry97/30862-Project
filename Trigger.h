#include "Condition.h"
#ifndef TRIGGER_H_
#define TRIGGER_H_

class Trigger {
<<<<<<< HEAD
	// put more stuff here later

=======
public:
	std::string command; // make this vector<> of commands? I don't know if we need to consider multiple commands
	std::vector<std::string> print;
	std::vector<std::string> action;
	std::string type;
	std::vector<Condition*> conditions;

	Trigger(rapidxml::xml_node<>*);
	~Trigger();
>>>>>>> d3381c472fad9557d726ff9dd5c9b4d515ea62da
};

#endif
