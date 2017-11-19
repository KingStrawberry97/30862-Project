#ifndef BASE_H_
#define BASE_H_
#include<vector>
#include"Trigger.h"
#include<stdio.h>
#include <algorithm>
#include <string.h>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

class Base {
public:
	std::string name;
	std::string status;
	std::string description;
	std::vector<Trigger*>triggers;
	Base();
	~Base();

	Trigger* checkTriggers(std::string command);

};

#endif
