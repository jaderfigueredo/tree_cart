/*
 * No.h
 *
 *  Created on: 23 de jan. de 2022
 *      Author: jader
 */

#ifndef NODE_H_
#define NODE_H_

#include <iostream>
#include <fstream>
#include <vector>
#include "Dataset.h"

using namespace std;


class Node {
public:
	Dataset* dataset;
	Node* left;
	Node* right;

	// Métodos construtores
	Node(Dataset* dataset = NULL);
	// Método destrutor
	virtual ~Node();

	void toSplit();

	void toSplitAll(int height = 0, int maxHeightParam = 4);

	bool setChildren(vector<Dataset*> datasets);

	unsigned int printInLevel(unsigned int i = 1, string side = "", unsigned int parent = 0, bool imprimirElementos = false, int hight = 0);

	unsigned int print(unsigned int i = 0, string side = "", unsigned int parent = 0, bool imprimirElementos = false, int hight = 0);

	void findBetterSplit();

	void draw(unsigned int tab = 0);

	void printStatistics(unsigned int i = 0);

	void preOrder(fstream &treeSettingsStream, int index = 0);

	void preOrderComIndices(ostringstream &results, int index = 0);

	void printContentForOverleafFormat(unsigned int index, string type = "");

	vector<Node*> getBranches();

	vector<Node*> getLeafs();

	int height();

	int nAcertos();
};


#endif /* NO_H_ */
