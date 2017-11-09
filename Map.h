/*
 * Map.h
 *
 *  Created on: Nov 2, 2017
 *      Author: jcbar
 */

#ifndef MAP_H_
#define MAP_H_
#include <iostream>
#include <stdio.h>
#include <vector>
#include <map>
#include <queue>
#include "Item.h"
<<<<<<< HEAD
//#include "Creature.h"
//#include "Container.h"
#include "Base.h"
//#include "Room.h"
=======
#include "Creature.h"
#include "Base.h"
#include "Room.h"
>>>>>>> d3381c472fad9557d726ff9dd5c9b4d515ea62da
#include "Trigger.h"
#include <sstream>
#include <iterator>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
<<<<<<< HEAD
=======
#include "Container.h"

>>>>>>> d3381c472fad9557d726ff9dd5c9b4d515ea62da

class Map {
public:
	Map(std::string filename);
	virtual ~Map();
<<<<<<< HEAD
	std::map<char*, Base*> all_objects;
	std::map<char*, Item*> items;
	//std::map<char*, Container*> containers;
	//std::map<char*, Creature*> creatures;
	//std::map<char*, Room*> rooms;
	//map<char*, char*> lookup;
	void printItems();
=======
	std::map<std::string, Base*> all_objects;
	std::map<std::string, Item*> items;
	std::map<std::string, Container*> containers;
	std::map<std::string, Creature*> creatures;
	std::map<std::string, Room*> rooms;
	//map<char*, char*> lookup;
	void printItems();
	void printContainers();
	void printCreatures();
	void printRooms();
	void printTriggers(Trigger* trigger);
	void run();
>>>>>>> d3381c472fad9557d726ff9dd5c9b4d515ea62da

private:
	void createZorkMap(std::string filename);
	void fragmentXmlNodes(rapidxml::xml_node<>*, std::queue<rapidxml::xml_node<>*>&, std::queue<rapidxml::xml_node<> *>&, std::queue<rapidxml::xml_node<> *>&,std::queue<rapidxml::xml_node<> *>&);
};

#endif /* MAP_H_ */
