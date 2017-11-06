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
	char* name;
	char* status;
	char* description;
	//std::vector<Tigger*>triggers;
	Base();
	virtual ~Base();
};

#endif
