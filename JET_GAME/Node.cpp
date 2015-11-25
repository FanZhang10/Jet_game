#ifndef _NODE_CPP
#define _NODE_CPP

#include <iostream>
#include <fstream>
#include <string>
#include "ArrayList.h"

#include "Node.h"



Node::Node(){
	locationX = 0;
	locationY = 0;
	locationZ = 0;
	isColloded = false;
}

int Node::getLocationX(){ return locationX; }

int Node::getLocationY(){ return locationY; }

int Node::getLocationZ(){ return locationZ; }

void Node::printOn(ostream & out) const {
	out << "Node[" << locationX << "," << locationY << "]";
}

void Node::parseDataFromFile(ifstream & input){

	string line; //line of text from input file
	/*
	while (getline(input, line))
	{
		line = trim(line);
		if (startsWith(line, nodeEndTag)){
			//cout << "End Node Parse\n";
			return;
		}
		else if (startsWith(line, nodeLocationStartTag) && endsWith(line, nodeLocationEndTag)){
			//parse node location data
			string datastring = line.substr(nodeLocationStartTag.length(),
				line.length() - nodeLocationStartTag.length() - nodeLocationEndTag.length());
			datastring = trim(datastring);

			string strX = datastring.substr(0, datastring.find_first_of(","));
			string strY = datastring.substr(datastring.find_first_of(",") + 1,
				datastring.length() - datastring.find_first_of(","));

			//set node location properties
			locationX = stoi(strX);
			locationY = stoi(strY);

			//cout << "parse node location: " << locationX << "X" << locationY << "\n";

		}
	}
	*/

}


ostream & operator<<(ostream & out, const Node& n){
	n.printOn(out);
	return out;
}

#endif