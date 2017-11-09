/*
 * Item.cpp
 *
 *  Created on: Nov 5, 2017
 *      Author: jcbar
 */
#include"Item.h"

Item::Item(rapidxml::xml_node<> * item_node){
	item_node = item_node -> first_node();

<<<<<<< HEAD
	char* node_name;
	char* value;
=======
	std::string node_name;
	std::string value;
	writing = "Item has no writing.";
	name = "NO NAME";
	description = "Item has no description.";
	status = "";
>>>>>>> d3381c472fad9557d726ff9dd5c9b4d515ea62da

	while(item_node != 0) {
		node_name = item_node -> name();
		value = item_node -> value();

<<<<<<< HEAD
		if(strcmp(node_name, "name") == 0) {
			name = node_name;
		}

		else if(strcmp(node_name, "writing") == 0) {
			writing = value;
		}
		else if(strcmp(node_name, "turnon") == 0) {
			//needs to have a function to set up the turn on actions
			turn_on = 0;
		}

		else if(strcmp(node_name, "status") == 0) {
			status = value;
		}
		else if(strcmp(node_name, "description") == 0) {
			description = value;
		}
		item_node = item_node -> next_sibling();
=======
		if(node_name == (std::string)"name") {
			name = value;
		}

		else if(node_name == (std::string)"writing") {
			writing = value;
		}
		else if(node_name == (std::string)"turnon") {
			Trigger* trigger = new Trigger(item_node);
			turn_on = trigger;
		}

		else if(node_name== (std::string)"status") {
			status = value;
		}
		else if(node_name == (std::string)"description") {
			description = value;
		}
		else if (node_name == (std::string)"trigger") {
			Trigger* trigger = new Trigger(item_node);
			triggers.push_back(trigger);
		}

		item_node = item_node -> next_sibling();

>>>>>>> d3381c472fad9557d726ff9dd5c9b4d515ea62da
	}
}

Item::~Item() {

}



