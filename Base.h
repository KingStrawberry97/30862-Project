#ifndef BASE_H_
#define BASE_H_
#include "Trigger.h"
#include <vector>
#include<stdio.h>

class Base {

protected:
	char* name;
	char* status;
	char* description;
	vector<Tigger*>triggers;
public:
	Base();
	virutal ~Base();
};

#endif
