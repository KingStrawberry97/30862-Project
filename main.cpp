// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <vector>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include <fstream>
#include <sstream>
#include "Base.h"
#include "Item.h"
#include "Map.h"


int main(int argc, char* argv[]) {
<<<<<<< HEAD
	Map * game = new Map("xml_test.xml");
	game -> printItems();
=======
	std::string input;
	bool valid = false;

	while (!valid) {
		std::cout << "Input file name: ";
		std::getline(std::cin, input);

		if (input == "xml_test.xml") {
			std::cout << "Running..." << std::endl;
			valid = true;
		}
	}

	Map * game = new Map(input);
	game->run();

	game -> printItems();
	game->printContainers();
	game->printCreatures();
	game->printRooms();
>>>>>>> d3381c472fad9557d726ff9dd5c9b4d515ea62da
    return 0;
}

