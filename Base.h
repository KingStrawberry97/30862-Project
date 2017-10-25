#include "Trigger.h"
#ifndef BASE_H_
#define BASE_H_

class Base {

protected:
	char* name;
	char* status;
	char* description;
	Trigger* triggers;
};

#endif
