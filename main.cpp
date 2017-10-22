// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include <fstream>
#include <sstream>


int main() {
	std::cout << "parsing xml file..." << std::endl;
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> * map;
	// read xml file into vector
	std::ifstream theFile("xml_test.xml");
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	// parse buffer using xml file parsing library into doc
	doc.parse<0>(&buffer[0]);

	map = doc.first_node("map");

	// load room data
	std::cout << "Map data: " << std::endl;

	for (rapidxml::xml_node<> * room = map->first_node("room"); room; room = room -> next_sibling("room")) {
		rapidxml::xml_node<> * room_name = room->first_node("name");
		std::cout << "Room: " << room_name->value() << std::endl;
		rapidxml::xml_node<> * description = room->first_node("description");
		if (description != NULL) {
			std::cout << "Description: " << description->value() << std::endl;
		}

		// load items
		std::cout << "Items in this room: ";
		for (rapidxml::xml_node<> * item = room->first_node("item"); item; item = item -> next_sibling("item")) {
			std::cout << item->value() << std::endl;
		}
		std::cout << std::endl;

		// load containers
		std::cout << "Containers in this room: ";
		for (rapidxml::xml_node<> * container = room->first_node("container"); container; container = container->next_sibling("container")) {
			std::cout << container->value() << std::endl;
		}
		std::cout << std::endl;

		// load creatures
		std::cout << "Creatures in this room: ";
		for (rapidxml::xml_node<> * creature = room->first_node("creature"); creature; creature = creature->next_sibling("creature")) {
			std::cout << creature->value() << std::endl;
		}
		std::cout << std::endl;

		// load triggers, needs work
		std::cout << "Triggers in this room: " << std::endl;
		for (rapidxml::xml_node<> * trigger = room->first_node("trigger"); trigger; trigger = room->next_sibling("trigger")) {
			std::cout << "Command: " << trigger->first_node("command")->value() << " type:" << trigger->first_node("type")->value() << std::endl;
			rapidxml::xml_node<> * condition = trigger->first_node("condition");
		}

		// load borders
		std::cout << "Borders: " << std::endl;
		for (rapidxml::xml_node<> * border = room->first_node("border"); border; border = border->next_sibling("border")) {
			std::cout << border->first_node("direction")->value();
			std::cout << ": " << border->first_node("name")->value() << std::endl;
		}
		std::cout << std::endl;


	}

	// load item data

	for (rapidxml::xml_node<> * item_loop; item_loop; item_loop = item_loop->next_sibling("item")) {
		std::cout << "Name: " << item_loop->first_node("name")->value() << std::endl;
		if (item_loop -> first_node("writing")) {
			std::cout << item_loop->first_node("writing")->value();
		}
		else {
			std::cout << "No writing" << std::endl;
		}
	}

	

    return 0;
}

