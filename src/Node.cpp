/*
 * No.cpp
 *
 *  Created on: 23 de jan. de 2022
 *      Author: jader
 */
#include "Node.h"
#include "Problem.h"

/*
#ifndef DISPLAY_OUTS
	#define DISPLAY_OUTS 1
	// Outros arquivos com a mesma DEFINITION:
	// Dataset.cpp
	// Node.cpp
	// Problem.cpp
#endif
*/
extern int DISPLAY_OUTS;
extern int DEBUG;
extern vector<string> problemClasses;


// Retorna o índice do filho esquerdo
extern int getLeftChild(int t);
// Retorna o índice do filho direito
extern int getRightChild(int t);

extern string getTabs(int n = 1);
extern int getLevelFromIndex(int index);

using namespace std;

// Construtor
Node::Node(Dataset* dataset) {
	this->left = NULL;
	this->right = NULL;
	this->dataset = dataset;
}
// Destrutor
Node::~Node() {
	delete this->dataset;
	delete this->left;
	delete this->right;
}

/*
unsigned int Node::printInLevel(unsigned int i, string side, unsigned int parent, bool imprimirElementos, int hight) {

	// Se não existe um nó raiz
		// Informar árvore vazia e encerrar o método

	// Se não
	// Adicionar o nó na fila

	// Enquanto a fila não estiver vazia
		// Remove e guarda o primeiro elemento da fila
		// Imprime o elemento
		// Se houver um filho esquerdo
			//Adiciona o filho esquerdo na fila
		// Se houver um filho direito
			// Adiciona o filho direito na fila




/ *

	if(DISPLAY_OUTS) {
	// Imprimir os dados do nó atual
		cout << endl;
		cout << "Node " << ": " << i << "\t\tFilho " << side << " do " << (parent == 0 ? "--" : to_string(parent));
	}

	if(this->dataset == NULL) {
		if(DISPLAY_OUTS) { cout << " não possui dataset." << endl; }
		return i;
	}

	// Calcular a altura da árvore
	// A altura só aumenta, mas ela não pode ser acumulativa
	if(hight > Problem::hight) {
		Problem::hight = hight;
	}

	// Se for para imprimir os elementos, mas este não for um nó folha, então não tentar imprimir seus elementos
	bool imprimirElementosDaFolha = (imprimirElementos && this->left == NULL && this->right == NULL);
	// Imprimir os detalhes do nó.
	this->dataset->print("", false, imprimirElementosDaFolha);

	vector<Dataset*> datasets(2);
	datasets[0] = this->left != NULL ? this->left->dataset : NULL;
	datasets[1] = this->right != NULL ? this->right->dataset : NULL;
	string tab = (this->left == NULL && this->right == NULL) ? "\t" : "";

	if(DISPLAY_OUTS) {
		cout << "Left:  " << this->left << tab << "\tdataset: " << datasets[0] << endl;
		cout << "Right: " << this->right << tab << "\tdataset: " << datasets[1]  << endl;
	}

	parent = i;

	// Imprimir os dados do nós filho esquerdo
	if(this->left != NULL) {
		i = this->left->print(++i, "esquerdo", parent, imprimirElementos, ++hight);
		hight--;
	}

	// Imprimir os dados do nós filho direito
	if(this->right != NULL) {
		i = this->right->print(++i, "direito", parent, imprimirElementos, ++hight);
		hight--;
	}

	if(this->left == NULL && this->right == NULL) {
		Problem::numberOfLeafs++;
	}
	else {
		Problem::numberOfBranches++;
	}

	if(DISPLAY_OUTS) { cout << endl; }
	return i;

}
*/

unsigned int Node::print(unsigned int i, string side, unsigned int parent, bool imprimirElementos, int hight) {

	if(DISPLAY_OUTS) {
	// Imprimir os dados do nó atual
		cout << endl;
		cout << "Node " << ": " << i << "\t\tFilho " << side << " do " << (parent == -1 ? "--" : to_string(parent));
	}

	if(this->dataset == NULL) {
		if(DISPLAY_OUTS) { cout << " não possui dataset." << endl; }
		return i;
	}

	// Calcular a altura da árvore
	// A altura só aumenta, mas ela não pode ser acumulativa
	if(hight > Problem::hight) {
		Problem::hight = hight;
	}

	// Se for para imprimir os elementos, mas este não for um nó folha, então não tentar imprimir seus elementos
	bool imprimirElementosDaFolha = (imprimirElementos && this->left == NULL && this->right == NULL);
	// Imprimir os detalhes do nó.
	this->dataset->print("", false, imprimirElementosDaFolha);

	vector<Dataset*> datasets(2);
	datasets[0] = this->left != NULL ? this->left->dataset : NULL;
	datasets[1] = this->right != NULL ? this->right->dataset : NULL;
	string tab = (this->left == NULL && this->right == NULL) ? "\t" : "";

	if(DISPLAY_OUTS && DEBUG) {
		cout << "Left:  " << this->left << tab << "\tdataset: " << datasets[0] << endl;
		cout << "Right: " << this->right << tab << "\tdataset: " << datasets[1]  << endl;
	}

	parent = i;

	// Imprimir os dados do nós filho esquerdo
	if(this->left != NULL) {
		//i = this->left->print(++i, "esquerdo", parent, imprimirElementos, ++hight);
		this->left->print((2*i+1), "esquerdo", parent, imprimirElementos, ++hight);
		hight--;
	}

	// Imprimir os dados do nós filho direito
	if(this->right != NULL) {
		//i = this->right->print(++i, "direito", parent, imprimirElementos, ++hight);
		this->right->print((2*i+2), "direito", parent, imprimirElementos, ++hight);
		hight--;
	}

	if(this->left == NULL && this->right == NULL) {
		Problem::numberOfLeafs++;
	}
	else {
		Problem::numberOfBranches++;
	}

	if(DISPLAY_OUTS) { cout << endl; }
	return i;
}

void Node::toSplit() {
	// Se for um nó vazio, não há o que fazer
	if(this->dataset == NULL) {
		return;
	}

	// Colocar condiçaõ para não deixar a árvore crescer mais que um limite de altura

	vector<Dataset*> subsets = this->dataset->findBetterSplitProfessor();

	this->left = (subsets[0] != NULL) ? new Node(subsets[0]) : NULL;

	this->right = (subsets[1] != NULL) ? new Node(subsets[1]) : NULL;
}

void Node::toSplitAll(int height, int maxHeightParam) {
	// Impedir que a árvore cresça além do limite máximo (maxHeightParam)
	if(height >= maxHeightParam) {

		// Encontrar todos os possíveis splits; Pode não ter utilidade, mas parece evitar algum bug na hora de imprimir a árvore
		//this->dataset->findSplits();
		// Determinar a Classe majoritária do conjunto
		this->dataset->defineMajoritaryClass();
		// Calcular estatísticas do nó, pois elas são usadas para encontrar a melhor partição dos dados
		this->dataset->calcStatistics();

		this->left = NULL;
		this->right = NULL;
		return;
	}

	this->toSplit();

	if(this->left != NULL) {
		height++;
		this->left->toSplitAll(height, maxHeightParam);
	}

	height--;

	if(this->right != NULL) {
		height++;
		this->right->toSplitAll(height, maxHeightParam);
	}

	// height--; // Inútil
}

void Node::printContentForOverleafFormat(unsigned int index, string type) {
	if(DISPLAY_OUTS) {
		cout << "[.\\node[draw]{" << index << " " << type << "\\\\ ";
		for(unsigned int j = 0; j < Problem::JClasses; j++) {
			cout << "cl[" << problemClasses[j] << "] = " << this->dataset->countElementsClasses[j] << "\\\\\t";
		}
		cout << "Impur:" << std::setprecision(5) << this->dataset->impurity;
		cout << " }; ";
	}
}

void Node::draw(unsigned int tab) {
	//Imprimindo as tabulações
	string concat = "";
	for(unsigned int i = 0; i < tab; i++) {
		concat += "    ";
	}

	if(DISPLAY_OUTS) {
		cout << concat;
	}
	this->printContentForOverleafFormat(tab);

	tab++;
	bool flagHasChild = false;

	if(this->left != NULL) {
		if(DISPLAY_OUTS) { cout << endl; }
		this->left->draw(tab);
		flagHasChild = true;
	}

	if(this->right != NULL) {
		if(DISPLAY_OUTS) { cout << endl; }
		this->right->draw(tab);
		flagHasChild = true;
	}

	if(flagHasChild) {
		if(DISPLAY_OUTS) { cout << endl << concat; }
	}

	if(DISPLAY_OUTS) { cout << "] "; }

	if(DISPLAY_OUTS) {
		// Se for o fim da execução (nó raiz), quebrar mais uma linha
		if(tab == 1) { cout << endl; }
	}
}


void Node::printStatistics(unsigned int i) {
	if(this->dataset == NULL) {
		return;
	}

	string label = "Node " + std::to_string(i);
	this->dataset->printStatistics(label);

	if(this->left != NULL) {
		this->left->printStatistics(++i);
	}

	if(this->right != NULL) {
		this->right->printStatistics(++i);
	}
}

void Node::preOrder(fstream &treeSettingsStream, int index) {
	string separator = " ";
	// Imprime o identificador de início de nó e o seu índice
	if(DISPLAY_OUTS) { cout << "(" << separator << index << separator; }
	treeSettingsStream << "(" << separator << index << separator;

	// Verifica e imprime o tipo do nó
	if(this->left == NULL || this->right == NULL) {
		if(DISPLAY_OUTS) { cout << "folha" << separator << "Impur:" << this->dataset->majoritaryClass << "_"<< problemClasses[this->dataset->majoritaryClass] << separator; }
		treeSettingsStream << "folha" << separator << this->dataset->majoritaryClass << separator;
	}
	else {
		if(DISPLAY_OUTS) {
			cout << "galho" << separator;
			cout << this->dataset->splitAttribute << separator;
			cout << setprecision(7) << this->dataset->splitValue << separator;
		}

		treeSettingsStream << "galho" << separator;
		treeSettingsStream << this->dataset->splitAttribute << separator;
		treeSettingsStream << setprecision(7) << this->dataset->splitValue << separator;

		// Passos recursivos: Chamar a impressão do filho esquerdo e filho direito
		this->left->preOrder(treeSettingsStream, (index+1));
		this->right->preOrder(treeSettingsStream, (index+2));
	}

	// Imprime o caracter de fechamento
	if(DISPLAY_OUTS) { cout << ")" << separator; }
	treeSettingsStream << ")" << separator;
}


void Node::preOrderComIndices(fstream &treeSettingsStream, int index, bool writeTreeSettings) {

	if(DISPLAY_OUTS) {
		if(writeTreeSettings) {
			//cout << endl << "NÃO escrevendo o arquivo tree-settings.txt, apenas no terminal." << endl << endl;
		}
		else {
			//cout << endl << "Escrevendo o arquivo tree-settings.txt" << endl << endl;
		}
	}

	string separator = " ";
	string tabs = getTabs(getLevelFromIndex(index));
	bool isLeaf = false;

	// Imprime o identificador de início de nó e o seu índice
	if(DISPLAY_OUTS) { cout << endl << tabs << "(" << separator << index << separator; }
	if(writeTreeSettings) { treeSettingsStream << "(" << separator << index << separator; }

	// Verifica e imprime o tipo do nó
	if(this->left == NULL || this->right == NULL) {
		if(DISPLAY_OUTS) { cout << "folha" << separator << "\tclass: " << this->dataset->majoritaryClass << "_"<< problemClasses[this->dataset->majoritaryClass] << separator; }
		cout << "\tImpur:" << std::setprecision(7) << this->dataset->impurity;

		if(writeTreeSettings) { treeSettingsStream << "folha" << separator << this->dataset->majoritaryClass << separator; }
		isLeaf = true;
	}
	else {
		if(DISPLAY_OUTS) {
			cout << "galho" << separator;
			cout << "\tImpur: " << std::setprecision(7) << this->dataset->impurity;
			cout << "\tAttr: " << this->dataset->splitAttribute << separator;
			cout << "\tSplitVal: " << setprecision(7) << this->dataset->splitValue << separator;
		}

		if(writeTreeSettings) {
			treeSettingsStream << "galho" << separator;
			treeSettingsStream << this->dataset->splitAttribute << separator;
			treeSettingsStream << setprecision(7) << this->dataset->splitValue << separator;
		}

		// Passos recursivos: Chamar a impressão do filho esquerdo e filho direito
		this->left->preOrderComIndices(treeSettingsStream, getLeftChild(index));
		this->right->preOrderComIndices(treeSettingsStream, getRightChild(index));
	}

	// Imprime o caracter de fechamento
	string closer = isLeaf ? ")" : "\n"+tabs+")";
	if(DISPLAY_OUTS) { cout << closer << separator; }
	if(writeTreeSettings) { treeSettingsStream << ")" << separator; }
}



