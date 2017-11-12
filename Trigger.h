#include "Condition.h"
#ifndef TRIGGER_H_
#define TRIGGER_H_

class Trigger {
public:
	std::string command;
	std::vector<std::string> print;
	std::vector<std::string> action;
	std::string type;
	std::vector<Condition*> conditions;
	bool conditions_met;
	bool completed;

	Trigger(rapidxml::xml_node<>*);
	~Trigger();
};

#endif
