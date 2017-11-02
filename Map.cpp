/*
 * Map.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: jcbar
 */

#include "Map.h"

Map::Map(char* filename) {
	createZorkMap(filename);
}

Map::~Map() {
}

void Map::createZorkMap(char* filename) {
	xml_document<> doc;
	xml_node<> * map_node;

	std::ifstream theFile(filename);
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);
	map_node = doc.first_node();
	fragmentXmlNodes(map_node)

}
