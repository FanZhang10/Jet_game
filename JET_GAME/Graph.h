#ifndef _GRAPH_H
#define _GRAPH_H

#include <iostream>
#include <string>

#include "ArrayList.h"
#include "Node.h"




//Edge XML tags

const string graphStartTag = "<graph>";
const string graphEndTag = "</graph>";
const string imageStartTag = "<image>"; 
const string imageEndTag = "</image>";
const string filenameStartTag = "<filename>";
const string filenameEndTag = "</filename>";
const string dimensionsStartTag = "<dimensions>";
const string dimensionsEndTag = "</dimensions>";


class Graph{

private:
	//information about a background image on which the graph is based.
	string backgroundImageFileName;
	int backgroundImageWidth;
	int backgroundImageHeight;

public: 

	ArrayList<Node> nodes; //nodes of the graph


	Graph();
	~Graph();

	int level;
	int getNumber();
	void buildGraph(int thelevel);


	void addNode(Node * nodePtr);
	void updateNode();

	Node * nodeWithLocation(int x, int y);

};

ostream & operator<<(ostream & out, const Graph & g);

#endif