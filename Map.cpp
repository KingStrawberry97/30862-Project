/*
 * Map.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: jcbar
 */

#include "Map.h"

Map::Map(std::string filename) {
	createZorkMap(filename);
}

Map::~Map() {
}

void Map::createZorkMap(std::string filename) {
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> * map_node;
	// read xml file into vector
	std::ifstream theFile(filename);
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	// parse buffer using xml file parsing library into doc
	doc.parse<0>(&buffer[0]);

	map_node = doc.first_node("map");

	std::queue<rapidxml::xml_node<> *> rooms_nodes;
	std::queue<rapidxml::xml_node<> *> items_nodes;
	std::queue<rapidxml::xml_node<> *> containers_nodes;
	std::queue<rapidxml::xml_node<> *> creatures_nodes;

	fragmentXmlNodes(map_node, rooms_nodes, items_nodes, containers_nodes, creatures_nodes);

	Room* newRoom;
	while(rooms_nodes.size() != 0){
		newRoom = new Room(rooms_nodes.front());
		all_objects[newRoom -> name] = newRoom;
		rooms[newRoom -> name] = newRoom;
		rooms_nodes.pop();
	}

	Item* newItem;
	while(items_nodes.size() != 0){
		newItem = new Item(items_nodes.front());
		all_objects[newItem -> name] = newItem;
		items[newItem -> name] = newItem;
		items_nodes.pop();
	}

	Container * newContainer;
	while(containers_nodes.size() != 0){
		newContainer = new Container(containers_nodes.front());
		all_objects[newContainer -> name] = newContainer;
		containers[newContainer -> name] = newContainer;
		containers_nodes.pop();
	}

	// set item owner field to container or room

	setItemOwners();

	Creature * newCreature;
	while(creatures_nodes.size() != 0){
		newCreature = new Creature(creatures_nodes.front());
		all_objects[newCreature -> name] = newCreature;
		creatures[newCreature -> name] = newCreature;
		creatures_nodes.pop();
	}
}

void Map::fragmentXmlNodes(rapidxml::xml_node<>* map_node, std::queue<rapidxml::xml_node<>*> &rooms, std::queue<rapidxml::xml_node<>*>&items, std::queue<rapidxml::xml_node<>*> &containers, std::queue<rapidxml::xml_node<>*> &creatures) {

	rapidxml::xml_node<>* node = map_node -> first_node();
	while(node != NULL){
		if((std::string)node->name() == (std::string)"room"){
			rooms.push(node);
		}
		else if((std::string)node -> name() == (std::string)"item") {
			items.push(node);
		}
		else if((std::string)node -> name() == (std::string)"container") {
			containers.push(node);
		}
		else if((std::string)node -> name() == (std::string)"creature") {
			creatures.push(node);
		}
		node = node -> next_sibling();
	}

}

void Map::run() {  // main loop for functionality of program
	std::string startRoom = "Entrance";
	std::string currRoom = startRoom;
	std::string input = "";
	std::vector<std::string> valid_direction = { "n", "s", "e", "w" };
	Trigger* trigger = NULL;
	

	bool game_over = false;
	bool trigger_override = false;

	std::cout << rooms.find(startRoom)->second->description << std::endl;
	
	while (!game_over) {
		std::getline(std::cin, input);
		std::string first_word = input.substr(0, input.find(" "));

		if (checkInput(input)) {
			trigger_override = false;
			std::cout << "Input valid." << std::endl;
			trigger = checkRoomTriggers(findRoom(currRoom), input);

			// if trigger != NULL, there is a trigger that overrides the given command

			if (trigger != NULL) {
				std::cout << "TRIGGER FOUND for command: " << input << std::endl;

				// check if trigger conditions are met
				// if so, override command and execute trigger

				checkTriggerConditions(trigger);
				if (trigger->conditions_met && (!(trigger->completed))) {
					executeTrigger(trigger);
					trigger_override = true;
				}
			}

			// if no trigger overrides command, execute command

			// move to different room 

			if (trigger_override == false) {
				for (std::vector<std::string>::iterator p = valid_direction.begin(); p != valid_direction.end(); ++p) {
					if ((*p) == input) {
						currRoom = changeRoom(input, currRoom);
					}
				}

				// print inventory

				if (input == (std::string)"i") {
					printInventory();
				}

				// open exit

				else if (input == (std::string)"open exit") {
					if (findRoom(currRoom)->type == (std::string)"exit") {
						std::cout << "VICTORY" << std::endl;
						game_over = true;
					}
				}

				// take [item]

				else if (first_word == (std::string)"take" && (countWords(input) == 2)) {
					std::vector<std::string> words = tokenizeString(input);
					Item* item = findItem(words.at(1));
					if (item != NULL) {
						if (item->owner == (std::string)"inventory") {
							std::cout << "You already have that." << std::endl;
						}
						else if (item->owner != rooms.find(currRoom)->second->name) {
							std::cout << "You can't take that." << std::endl;
						}
						else {
							std::cout << "Item " << item->name << " added to inventory." << std::endl;
							item->owner = "inventory";
						}
					}
					else {
						std::cout << "You can't take that." << std::endl;
					}
				}

				// open [container], might need testing

				else if (first_word == (std::string)"open" && input != (std::string)"open exit" && (countWords(input) == 2)) {
					std::vector<std::string> words = tokenizeString(input);
					Container* container = findContainer(words.at(1));
					if (container != NULL) {
						if (container->open == true) {
							//std::cout << container->name << " is already open." << std::endl;
						}
						else {
							openContainer(container, findRoom(currRoom));
							container->open = true;
						}
					}
					else {
						std::cout << "You can't open that." << std::endl;
					}
				}

				// read [item]

				else if (first_word == (std::string)"read" && (countWords(input) == 2)) {
					std::vector<std::string> words = tokenizeString(input);
					Item* item = findItem(words.at(1));
					if (item != NULL) {
						if (item->owner != (std::string)"inventory") {
							std::cout << "You don't have that item." << std::endl;
						}
						else {
							std::cout << item->writing << std::endl;
						}
					}
					else {
						std::cout << "You don't have that item." << std::endl;
					}

				}

				// drop [item]

				else if (first_word == (std::string)"drop" && (countWords(input) == 2)) {
					std::vector<std::string> words = tokenizeString(input);
					Item* item = findItem(words.at(1));
					if (item != NULL) {
						if (item->owner != (std::string)"inventory") {
							std::cout << "You don't have that item." << std::endl;
						}
						else {
							item->owner = currRoom;
							std::cout << "You drop the " << item->name << "." << std::endl;
						}
					}
					else {
						std::cout << "You don't have that item." << std::endl;
					}
				}

				// put [item] in [container], might need testing

				else if (first_word == (std::string)"put" && (countWords(input) == 4) && input.find(" in ") != std::string::npos) {
					std::vector<std::string> words = tokenizeString(input);
					Item* item = findItem(words.at(1));
					Container* container = findContainer(words.at(3));
					if (item != NULL && container != NULL){
						if (item->owner != (std::string)"inventory") {
							std::cout << "You don't have that item." << std::endl;
						}
						else if (container->open == false) {
							std::cout << "The " << container->name << " isn't open." << std::endl;
						}
						else if (std::find(findRoom(currRoom)->containers.begin(), findRoom(currRoom)->containers.end(), container->name) != findRoom(currRoom)->containers.end()) {
							item->owner = currRoom;
							container->items.push_back(item->name);
							std::cout << "You put the " << item->name << " in the " << container->name << "." << std::endl;
						}
						else {
							std::cout << "You can't put that there." << std::endl;
						}
					}
					else {
						std::cout << "You can't put that there." << std::endl;
					}
					
				}

				// turn on [item]

				else if (input.find("turn on") != std::string::npos && first_word == (std::string)"turn" && (countWords(input) == 3)) {
					std::vector<std::string> words = tokenizeString(input);
					Item* item = findItem(words.at(2));
					if (item != NULL) {
						if (item->owner != (std::string)"inventory") {
							std::cout << "You don't have that item." << std::endl;
						}
						else {
							checkTriggerConditions(item->turn_on);
							if (item->turn_on->conditions_met == true) {
								if (item->turn_on->completed) {
									std::cout << item->name << " has already been turned on." << std::endl;
								}
								else {
									std::cout << "You activate the " << item -> name << std::endl;
									executeTrigger(item->turn_on);
								}
							}
						}
					}
					else {
						std::cout << "You don't have that item." << std::endl;
					}
				}

				// attack [creature] with [item]

				else if (first_word == (std::string)"attack" && (countWords(input) == 4) && input.find(" with ") != std::string::npos) {
					std::vector<std::string> words = tokenizeString(input);
					Creature* creature = findCreature(words.at(1));
					Item* item = findItem(words.at(3));
					if (item != NULL && creature != NULL) {
						if (item->owner != (std::string)"inventory") {
							std::cout << "You don't have that item." << std::endl;
						}
						else if (std::find(findRoom(currRoom)->creatures.begin(), findRoom(currRoom)->creatures.end(), creature->name) != findRoom(currRoom)->creatures.end()) {
							std::cout << "That creature isn't here." << std::endl;
						}
						else {
							attackCreature(creature, item);
						}
					}
					else {
						std::cout << "You can't do that." << std::endl;
					}

				}

			}
		}
		else {
			std::cout << "Invalid input." << std::endl;
		}
		// to do: check if other triggers need to be activated
		checkAllTriggers(findRoom(currRoom));
	}

}

void Map::checkAllTriggers(Room* current_room) {
	Item* item = NULL;
	Container* container = NULL;
	Creature* creature = NULL;
	Trigger* found_trigger = NULL;

	for (std::vector<std::string>::iterator q = current_room->items.begin(); q != current_room->items.end(); ++q) {
		item = findItem(*q);
		found_trigger = item->checkOtherTriggers();
		if (found_trigger != NULL) {
			checkTriggerConditions(found_trigger);
			if (found_trigger->conditions_met && (!(found_trigger->completed))) {
				executeTrigger(found_trigger);
			}
		}
	}
	for (std::vector<std::string>::iterator q = current_room->containers.begin(); q != current_room->containers.end(); ++q) {
		container = findContainer(*q);
		found_trigger = container->checkOtherTriggers();
		if (found_trigger != NULL) {
			checkTriggerConditions(found_trigger);
			if (found_trigger->conditions_met && (!(found_trigger->completed))) {
				executeTrigger(found_trigger);
			}
		}
	}
	//some sort of issue here in trying to iterate through creatures in a room. Its not finding any
	//for (std::vector<std::string>::iterator q = current_room->creatures.begin(); q != current_room->creatures.end(); ++q) {
		creature = findCreature("gnome");
		found_trigger = creature->checkOtherTriggers();
		if (found_trigger != NULL) {
			checkTriggerConditions(found_trigger);
			if (found_trigger->conditions_met && (!(found_trigger->completed))) {
				executeTrigger(found_trigger);
			}
		}
//	}
}


void Map::attackCreature(Creature* creature, Item* item) {
	if (std::find(creature->vulnerabilities.begin(), creature->vulnerabilities.end(), item->name) != creature->vulnerabilities.end()) {
		checkTriggerConditions(creature->attack);
		if (creature->attack->conditions_met) {
			executeTrigger(creature->attack);
		}
		else {
			std::cout << "Conditions for attack were not met." << std::endl;
		}

	}
	else {
		std::cout << "The " << creature->name << " is not weak to " << item->name << "." << std::endl;
	}
}

bool Map::checkInput(std::string input) {
	std::vector<std::string> valid_direction = {"n", "s", "e", "w"};
	std::string first_word = input.substr(0, input.find(" "));
	
	// check if directional input

	for (std::vector<std::string>::iterator p = valid_direction.begin(); p != valid_direction.end(); ++p) {
		if ((*p) == input) {
			return true;
		}
	}

	// check if "check inventory"

	if (input == (std::string)"i") {
		return true;
	}

	// check if "open exit"

	else if (input == (std::string)"open exit") {
		return true;
	}

	// check if "take"

	else if (first_word == (std::string)"take" && (countWords(input) == 2)) {
		return true;
	}

	// check if "open"

	else if (first_word == (std::string)"open" && input != (std::string)"open exit" && (countWords(input) == 2)) {
		return true;
	}

	// check if "read"

	else if (first_word == (std::string)"read" && (countWords(input) == 2)) {
		return true;
	}

	// check if "drop"

	else if (first_word == (std::string)"drop" && (countWords(input) == 2)) {
		return true;
	}

	// check if "put"

	else if (first_word == (std::string)"put" && (countWords(input) == 4) && input.find(" in ") != std::string::npos) {
		return true;
	}

	// check if "turn on"

	else if (input.find("turn on") != std::string::npos && first_word == (std::string)"turn" && (countWords(input) == 3)) {
		return true;
	}

	// check if "attack"

	else if (first_word == (std::string)"attack" && (countWords(input) == 4) && input.find(" with ") != std::string::npos) {
		return true;
	}

	return false;
}

int Map::countWords(std::string input)
{
	int number_of_words = 1;
	for (int i = 0; i < (int)input.length(); i++)
		if (input[i] == ' ')
			number_of_words++;
	return number_of_words;
}

void Map::printInventory() {
	bool inv_empty = true;
	std::cout << "Inventory: ";
	for (std::map<std::string, Item*>::iterator p = items.begin(); p != items.end(); ++p) {
		if (p->second->owner == (std::string)"inventory") {
			if (inv_empty != true) {
				std::cout << ", ";
			}
			std::cout << p->second->name;
			inv_empty = false;
		}
	}

	if (inv_empty == true) {
		std::cout << "empty";
	}
	std::cout << std::endl;

}

void Map::openContainer(Container* container, Room* currRoom) {
	bool container_empty = true;
	std::cout << container->name << " contains: ";
	for (std::vector<std::string>::iterator p = container->items.begin(); p != container->items.end(); ++p) {
		if (container_empty != true) {
			std::cout << ", ";
		}
		std::cout << (*p);
		findItem(*p)->owner = currRoom->name;
		container_empty = false;
	}
	std::cout << std::endl;

	if (container_empty == true) {
		std::cout << container->name << " is empty." << std::endl;
	}

}

void Map::printItems() {
	std::map<std::string, Item*>::iterator p = items.begin();
	std::cout << "///// ITEMS /////" << std::endl;
	while(p != items.end()) {
		std::cout<< "Item name: " << p->second -> name << std::endl;
		std::cout << "Item writing:  " << p->second->writing << std::endl;
		std::cout << "Item status: " << p->second->status << std::endl;
		if (p->second->turn_on != NULL) {
			std::cout << "Item turnon: " << std::endl;
			printTriggers(p->second->turn_on);
		}
		// print triggers vector
		std::cout << "Triggers: " << std::endl;
		for (std::vector<Trigger*>::iterator q = p->second->triggers.begin(); q != p->second->triggers.end(); ++q) {
			printTriggers(*q);
		}
		std::cout << std::endl;

		++p;
	}
}

void Map::printCreatures() {
	std::map<std::string, Creature*>::iterator p = creatures.begin();
	std::cout << "\n///// CREATURES /////" << std::endl;
	while (p != creatures.end()) {
		std::cout << "Creature name: " << p->second->name << std::endl;
		// print vulnerabilities vector
		std::cout << "Vulnerability: ";
		for (std::vector<std::string>::iterator q = p->second->vulnerabilities.begin(); q != p->second->vulnerabilities.end(); ++q) {
			std::cout << *q;
		}
		std::cout << std::endl;
		// print triggers vector
		std::cout << "Triggers: " << std::endl;
		for (std::vector<Trigger*>::iterator q = p->second->triggers.begin(); q != p->second->triggers.end(); ++q) {
			printTriggers(*q);
		}
		// print attack
		std::cout << "Attack: " << std::endl;
		printTriggers(p->second->attack);
		std::cout << std::endl;
		++p;
	}
}

void Map::printRooms() {
	std::map<std::string, Room*>::iterator p = rooms.begin();
	std::cout << "\n///// ROOMS /////" << std::endl;
	while (p != rooms.end()) {
		std::cout << "Room name:" << p->second->name << std::endl;
		std::cout << "Type: " << p->second->type << std::endl;
		// print items vector
		std::cout << "Items: ";
		for (std::vector<std::string>::iterator q = p->second->items.begin(); q != p->second->items.end(); ++q) {
			std::cout << *q << " ";
		}
		std::cout << std::endl;
		// print containers vector
		std::cout << "Containers: ";
		for (std::vector<std::string>::iterator q = p->second->containers.begin(); q != p->second->containers.end(); ++q) {
			std::cout << *q << " ";
		}
		std::cout << std::endl;
		// print creatures vector
		std::cout << "Creatures: ";
		for (std::vector<std::string>::iterator q = p->second->creatures.begin(); q != p->second->creatures.end(); ++q) {
			std::cout << *q << " ";
		}
		std::cout << std::endl;
		// print borders vector
		std::cout << "Borders: " << std::endl;
		std::string name;
		for (std::vector<Border*>::iterator q = p->second->borders.begin(); q != p->second->borders.end(); ++q) {
			std::cout << (*q)->name << " to the " << (*q)->direction << std::endl;		
		}
		// print triggers vector
		std::cout << "Triggers: " << std::endl;
		for (std::vector<Trigger*>::iterator q = p->second->triggers.begin(); q != p->second->triggers.end(); ++q) {
			printTriggers(*q);
		}
		std::cout << std::endl;
		p++;
	}
}

void Map::printContainers() {
	std::map<std::string, Container*>::iterator p = containers.begin();
	std::cout << "///// CONTAINERS /////" << std::endl;
	while (p != containers.end()) {
		std::cout << "Container name: " << p->second->name << std::endl;
		std::cout << "Items: ";
		std::cout << "Container status: " << p->second->status << std::endl;
		// print items vector
		for (std::vector<std::string>::iterator q = p->second->items.begin(); q != p->second->items.end(); ++q) {
			std::cout << *q << " ";
		}
		// print accept vector
		std::cout << "\nAccepts: ";
		for (std::vector<std::string>::iterator q = p->second->accepts.begin(); q != p->second->accepts.end(); ++q) {
			std::cout << *q << " ";
		}
		std::cout << "\n";
		// print triggers vector
		std::cout << "Triggers: " << std::endl;
		for (std::vector<Trigger*>::iterator q = p->second->triggers.begin(); q != p->second->triggers.end(); ++q) {
			printTriggers(*q);
		}
		p++;
	}
}

void Map::printTriggers(Trigger* trigger) {
	if (trigger->command != "") {
		std::cout << "Command: " << trigger->command << std::endl;
	}
	if (trigger->type != "") {
		std::cout << "Type: " << trigger->type << std::endl;
	}
	std::cout << "Print: ";
	for (std::vector<std::string>::iterator q = trigger->print.begin(); q != trigger->print.end(); ++q) {
		std::cout << *q << "; ";
	}
	std::cout << std::endl;
	std::cout << "Actions: ";
	for (std::vector<std::string>::iterator q = trigger->action.begin(); q != trigger->action.end(); ++q) {
		std::cout << *q << "; ";
	}
	std::cout << std::endl;
	std::cout << "CONDITIONS: " << std::endl;
	for (std::vector<Condition*>::iterator q = trigger->conditions.begin(); q != trigger->conditions.end(); ++q) {
		if ((*q)->owner != "") {
			std::cout << "owner: " << (*q)->owner << std::endl;
		}
		if ((*q)->has != "") {
			std::cout << "has: " << (*q)->has << std::endl;
		}
		if ((*q)->object != "") {
			std::cout << "object: " << (*q)->object << std::endl;
		}
		if ((*q)->status != "") {
			std::cout << "status: " << (*q)->status << std::endl;
		}
	}
	std::cout << std::endl;

}

Item* Map::findItem(std::string name) {
	std::map<std::string, Item*>::iterator p = items.begin();
	while (p != items.end()) {
		if (p->second->name == name) {
			return (p->second);
		}
		p++;
	}
	return NULL;
}

Container* Map::findContainer(std::string name) {
	std::map<std::string, Container*>::iterator p = containers.begin();
	while (p != containers.end()) {
		if (p->second->name == name) {
			return (p->second);
		}
		p++;
	}
	return NULL;
}

Creature* Map::findCreature(std::string name) {
	std::map<std::string, Creature*>::iterator p = creatures.begin();
	while (p != creatures.end()) {
		if (p->second->name == name) {
			return (p->second);
		}
		p++;
	}
	return NULL;
}

Room* Map::findRoom(std::string name) {
	std::map<std::string, Room*>::iterator p = rooms.begin();
	while (p != rooms.end()) {
		if (p->second->name == name) {
			return (p->second);
		}
		p++;
	}
	return NULL;
}

Trigger* Map::checkRoomTriggers(Room* current_room, std::string command) {
	Item* item = NULL;
	Container* container = NULL;
	Creature* creature = NULL;
	Trigger* found_trigger = NULL;

	// if function finds trigger that matches the command, it returns the trigger
	// otherwise, returns NULL

	found_trigger = current_room->checkTriggers(command);
	if (found_trigger != NULL) {
		return found_trigger;
	}

	for (std::vector<std::string>::iterator q = current_room->items.begin(); q != current_room->items.end(); ++q) {
		item = findItem(*q);
		found_trigger = item->checkTriggers(command);
		if (found_trigger != NULL) {
			return found_trigger;
		}
	}
	for (std::vector<std::string>::iterator q = current_room->containers.begin(); q != current_room->containers.end(); ++q) {
		container = findContainer(*q);
		found_trigger = container->checkTriggers(command);
		if (found_trigger != NULL) {
			return found_trigger;
		}
	}
	for (std::vector<std::string>::iterator q = current_room->creatures.begin(); q != current_room->creatures.end(); ++q) {
		creature = findCreature(*q);
		found_trigger = creature->checkTriggers(command);
		if (found_trigger != NULL) {
			return found_trigger;
		}
	}
	return NULL;
}

void Map::setItemOwners() {
	Item* item = NULL;
	for (std::map<std::string, Room*>::iterator p = rooms.begin(); p != rooms.end(); ++p) {
		for (std::vector<std::string>::iterator q = p->second->items.begin(); q != p->second->items.end(); ++q) {
			item = findItem((*q));
			item->owner = p->second->name;
		}
	}
	for (std::map<std::string, Container*>::iterator p = containers.begin(); p != containers.end(); ++p) {
		for (std::vector<std::string>::iterator q = p->second->items.begin(); q != p->second->items.end(); ++q) {
			item = findItem((*q));
			item->owner = p->second->name;
		}
	}

}

void Map::checkTriggerConditions(Trigger* trigger) {
	if (trigger == NULL) {
		return;
	}
	if (trigger->conditions.size() == 0) {
		trigger->conditions_met = true;
		return;
	}
	for (std::vector<Condition*>::iterator p = trigger->conditions.begin(); p != trigger->conditions.end(); ++p ) {
		// Case 1: [Object] is [status]
		if ((*p)->status != (std::string)"") {
			Base* object = all_objects.find((*p)->object)->second;
			if (object == NULL) {
				std::cout << "ERROR: OBJECT NOT FOUND" << std::endl;
				return;
			}
			if (object->status != (*p)->status) {
				trigger->conditions_met = false;
				return;
			}
			else {
				trigger->conditions_met = true;
			}
		}

		// Case 2: [Container/Inventory] has [Object]
		// I'm assuming here that the only two things that can "own" an object are the inventory or a container
		// If this isn't true, then this will have to be changed
		else if ((*p)->owner != (std::string)"") {
			Item* item = findItem((*p)->object);
			if (item == NULL) {
				std::cout << "ERROR: ITEM NOT FOUND" << std::endl;
				return;
			}
			if ((*p)->owner == (std::string)"inventory") {
				if (item->owner == (std::string)"inventory") {
					if ((*p)->has == (std::string)"no") {
						trigger->conditions_met = false;
						return;
					}
					else if ((*p)->has == (std::string)"yes") {
						trigger->conditions_met = true;
					}

				}
				else {
					if ((*p)->has == (std::string)"no") {
						trigger->conditions_met = true;
					}
					else if ((*p)->has == (std::string)"yes") {
						trigger->conditions_met = false;
						return;
					}
				}
			}
			else {
				Container* owner = findContainer((*p)->owner);
				if (owner == NULL) {
					std::cout << "ERROR: OWNER NOT FOUND" << std::endl;
					return;
				}
				if (item->owner == owner->name) {
					if ((*p)->has == (std::string)"no") {
						trigger->conditions_met = false;
						return;
					}
					else {
						trigger->conditions_met = true;
					}
				}
				else {
					if ((*p)->has == (std::string)"no") {
						trigger->conditions_met = true;
					}
					else {
						trigger->conditions_met = false;
						return;
					}
				}
			}

		}
	}
}

std::vector<std::string> Map::tokenizeString(std::string string) {
	std::stringstream ss(string);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

void Map::executeTrigger(Trigger* trigger) {

	if (trigger->completed == true && (trigger->type == (std::string)"single")) {
		return;
	}

	for (std::vector<std::string>::iterator p = trigger->print.begin(); p != trigger->print.end(); ++p) {
		std::cout << (*p) << std::endl;
	}

	for (std::vector<std::string>::iterator p = trigger->action.begin(); p != trigger->action.end(); ++p) {
		std::vector<std::string> words = tokenizeString(*p);
		// Case 1: Update [item] to [status]
		if (words.at(0) == (std::string)"Update") {
			updateObject(words.at(1), words.at(3));
		}
		// Case 2: Add [object] to [room]
		else if (words.at(0) == (std::string)"Add") {
			addObject(words.at(1), words.at(3));
		}
		// Case 3: Delete [object]
		else if (words.at(0) == (std::string)"Delete") {
			deleteObject(words.at(1));
		}
		else {
			std::cout << "ERROR: UNKNOWN ACTION" << std::endl;
		}
	}

	if (trigger->type == "single") {
		trigger->completed = true;
	}
}

void Map::updateObject(std::string object_name, std::string status) {
	Base* object = all_objects.find(object_name)->second;
	if (object != NULL) {
		object->status = status;
	}
	else {
		std::cout << "ERROR: OBJECT NOT FOUND" << std::endl;
	}
	return;
}

void Map::deleteObject(std::string object_name) {
	// d e f i n i t e l y needs testing, not sure if works
	Item* item = findItem(object_name);
	Creature* creature = findCreature(object_name);
	Container* container = findContainer(object_name);
	Room* room = findRoom(object_name);

	if (creature != NULL) {
		for (std::map<std::string, Room*>::iterator p = rooms.begin(); p != rooms.end(); ++p) {
			for (std::vector<std::string>::iterator q = p->second->creatures.begin(); q != p->second->creatures.end(); ++q) {
				if ((*q) == object_name) {
					p->second->creatures.erase(std::remove(p->second->creatures.begin(), p->second->creatures.end(), object_name), p->second->creatures.end());
				}
			}
		}
	}
	else if (container != NULL) {
		for (std::map<std::string, Room*>::iterator p = rooms.begin(); p != rooms.end(); ++p) {
			for (std::vector<std::string>::iterator q = p->second->containers.begin(); q != p->second->containers.end(); ++q) {
				if ((*q) == object_name) {
					p->second->containers.erase(std::remove(p->second->containers.begin(), p->second->containers.end(), object_name), p->second->containers.end());
				}
			}
		}
	}
	else if (room != NULL) {
		for (std::map<std::string, Room*>::iterator p = rooms.begin(); p != rooms.end(); ++p) {
			for (std::vector<Border*>::iterator q = p->second->borders.begin(); q != p->second->borders.end(); ++q) {
				if ((*q)->name == object_name) {
					(*q)->name = "";
				}
			}
		}
	}
	else if (item != NULL) {
		for (std::map<std::string, Room*>::iterator p = rooms.begin(); p != rooms.end(); ++p) {
			for (std::vector<std::string>::iterator q = p->second->items.begin(); q != p->second->items.end(); ++q) {
				if ((*q) == object_name) {
					p->second->items.erase(std::remove(p->second->items.begin(), p->second->items.end(), object_name), p->second->items.end());
				}
			}
		}
		for (std::map<std::string, Container*>::iterator p = containers.begin(); p != containers.end(); ++p) {
			for (std::vector<std::string>::iterator q = p->second->items.begin(); q != p->second->items.end(); ++q) {
				if ((*q) == object_name) {
					p->second->items.erase(std::remove(p->second->items.begin(), p->second->items.end(), object_name), p->second->items.end());
				}
			}
		}
	}
	else {
		std::cout << "ERROR: OBJECT NOT FOUND FOR DELETION" << std::endl;
	}
}

void Map::addObject(std::string object_name, std::string location_name) { // doesn't work yet

	Container* container = findContainer(location_name);
	if (container != NULL) {
		Item* item = findItem(object_name);

		container->items.push_back(object_name);
		item->owner = location_name;

	}

	else {
		Room* room = findRoom(location_name);

		if (room == NULL) {
			std::cout << "ERROR: LOCATION NOT FOUND" << std::endl;
			return;
		}

		// the reason I created three pointers is because we don't know what type of object is being added to the room
		Item* item = findItem(object_name);
		Container* container = findContainer(object_name);
		Creature* creature = findCreature(object_name);

		if (item != NULL) {
			room->items.push_back(item->name);
			item->owner = room->name;
		}
		else if (container != NULL) {
			room->containers.push_back(container->name);
		}
		else if (creature != NULL) {
			room->creatures.push_back(creature->name);
		}
		else {
			std::cout << "ERROR IN ADD: OBJECT NOT FOUND" << std::endl;
		}

	}

	std::cout << "DEBUG: " << object_name << " added to " << location_name << std::endl;

}

std::string Map::changeRoom(std::string command, std::string currRoom) {
	Room* currRoomP = findRoom(currRoom);
	std::vector<Border*>::iterator p = currRoomP->borders.begin();
	while(p != currRoomP->borders.end()) {
		if ((*p)->direction == command) {
			if ((*p)->name == (std::string)"") {
				std::cout << "That room no longer exists." << std::endl;
				return currRoom;
			}
			std::cout << findRoom((*p)->name)->description << std::endl;
			return (*p)->name;
		}
		++p;
	}
	std::cout << "Can't go that way." << std::endl;
	return currRoom;
}
