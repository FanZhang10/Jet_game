#ifndef _NODE_H
#define _NODE_H

#include <iostream>
#include <string>
#include "ArrayList.h"

//Models nodes of a graph

//Node XML tags
const string nodeStartTag = "<node>";
const string nodeEndTag = "</node>";
const string nodeLocationStartTag = "<location>";
const string nodeLocationEndTag = "</location>";

class Edge; //forward declaration for Edge class

class Node{

public:
	Node();

	int locationX;
	int locationY;
	int locationZ;

	int getLocationX();
	int getLocationY();
	int getLocationZ();

	bool isColloded;

	void printOn(ostream & out) const;

	void parseDataFromFile(ifstream & input);

};

ostream & operator<<(ostream & out, const Node& n);



#endif