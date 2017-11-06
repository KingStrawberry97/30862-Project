/*
 * Item.cpp
 *
 *  Created on: Nov 5, 2017
 *      Author: jcbar
 */
#include"Item.h"

Item::Item(rapidxml::xml_node<> * item_node){
	item_node = item_node -> first_node();

	char* node_name;
	char* value;

	while(item_node != 0) {
		node_name = item_node -> name();
		value = item_node -> value();

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
	}
}

Item::~Item() {

}



