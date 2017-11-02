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
#include "Creature.h"
#include "Base.h"
#include "Room.h"
#include <sstream>
#include <itorator>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

class Map {
public:
	Map(char*filename);
	virtual ~Map();
	map<char*, Base*> all_objects;
	map<char*, Item*> items;
	map<char*, Container*> containers;
	map<char*, Creature*> creatures;
	map<char*, Room*> rooms;
	map<char*, char*> lookup;

private:
	void createZorkMap(char* filename);
	void fragmentXmlNodes(xml_node<>*)
};

#endif /* MAP_H_ */
