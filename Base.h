#ifndef BASE_H_
#define BASE_H_
#include<vector>
#include"Trigger.h"
#include<stdio.h>
#include <string.h>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

class Base {
public:
<<<<<<< HEAD
	char* name;
	char* status;
	char* description;
	//std::vector<Tigger*>triggers;
	Base();
	virtual ~Base();
=======
	std::string name;
	std::string status;
	std::string description;
	std::vector<Trigger*>triggers;
	Base();
	~Base();

>>>>>>> d3381c472fad9557d726ff9dd5c9b4d515ea62da
};

#endif
