#ifndef _GRAPH_CPP
#define _GRAPH_CPP

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>       /* time */

#include "ArrayList.h"
#include "Node.h"

#include "Graph.h"

static int numOfCube = 3; //chage for how many cube be showed on screen

Graph::Graph(){ level = 1; }
	Graph::~Graph(){

		cout << "Destructing Graph" << "\n";


		//delete the nodes nodes of the graph
		for (int i = 0; i < nodes.size(); i++){
			Node * nodePtr = nodes.elementAt(i);
			delete nodePtr;
		}

	}



	void Graph::addNode(Node * nodePtr){
		nodes.add(nodePtr);

	}

	int Graph::getNumber()
	{
		int numCube = 3;
		if (level == 1) {
			numCube = 3;
		}
		else if (level == 2) {
			numCube = 4;
		}
		else{		
			numCube += 1;
		}

		return numCube;
	}

	void Graph::updateNode()
	{	
		
		float z = this->nodes.elementAt(0)->getLocationZ();
		
		numOfCube = getNumber();


		for (int i = 0; i < numOfCube; i++){
			z = this->nodes.elementAt(i)->getLocationZ();

			if (z < 0.1f) {
				this->nodes.remove(this->nodes.elementAt(i));

				Node * node = new Node();
				int test = rand();
				/* generate secret number between 1 and 10: */
				float xPos = rand() % 100 - 50;
				float yPos = rand() % 100 - 50;
				float zPos = rand() % 500 + 400;

				node->locationX = xPos;
				node->locationY = yPos;
				node->locationZ = zPos;

				addNode(node);
			}
		
		}
	}

	void Graph::buildGraph(int thelevel) {
		level = thelevel;
		numOfCube = getNumber();
		/* initialize random seed: */
		srand(time(NULL));


		//for (int i = 0; i < 3; i++)
		for (int i = 0; i < numOfCube; i++) {
			Node * node = new Node();
			int test = rand();
			/* generate secret number between 1 and 10: */
			float xPos = rand() % 100 - 50;
			float yPos = rand() % 100 - 50;
			float zPos = rand() % 500 + 400;

			node->locationX = xPos;
			node->locationY = yPos;
			node->locationZ = zPos;

			addNode(node);
		}
	}



	Node * Graph::nodeWithLocation(int x, int y){
		//answer the node with location co-ordinates x,y or null if none exists
		//delete the nodes nodes of the graph
		for (int i = 0; i < nodes.size(); i++){
			Node * nodePtr = nodes.elementAt(i);
			if (nodePtr->getLocationX() == x && nodePtr->getLocationY() == y) return nodePtr;
		}

		return NULL;

	}





#endif