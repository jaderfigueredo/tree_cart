#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <math.h>
#include <bits/stdc++.h>
#include <cmath>
#include <limits>
#include <set>
#include <algorithm>
#include <string>
#include <unistd.h>

#include "Dataset.h"
#include "Node.h"
#include "Tree.h"
#include "Problem.h"


int DEBUG = 0;

int DISPLAY_OUTS = 1;

using namespace std;

//string datasets = "/home/jader/Documentos/ambiente-dev-mestrado/Datasets/Iris/";
string datasets = "csv/";
string fileName = datasets+"iris.norm.csv";
vector<string> problemClasses;

// Retorna o índice do filho esquerdo
int getLeftChild(int t) {
	return 2*t+1;
}

// Retorna o índice do filho direito
int getRightChild(int t) {
	return 2*t+2;
}

// Retorna o nó pai de um nó filho
int getParent(int t) {
	return ceil((float) t/2)-1;
}

// Retorna true se o a é <= b
bool doubleCompare(double a, double b) {
	return ((a < b+std::numeric_limits<double>::epsilon()));
}


// Encontrar uma string em um vector<string>
// Retorna o índice onde a string foi encontrada ou -1, caso contrário
int findStringInVector(string const _key, vector<string> list) {
	for(unsigned int i = 0; i < list.size(); i++) {
		if(_key.compare(list[i]) == 0) {
			return i;
		}
	}
	return -1;
}

string getTabs(int n = 1, string _char = "\t"){
	string tabs = "";
	while(n > 0) {
		tabs += _char;
		n--;
	}
	return tabs;
}

int getLevelFromIndex(int index) {
	// Caso base
	if(index == 0) {
		return 0;
	}

	// Passo recursivo
	return 1+getLevelFromIndex(getParent(index));
}

void printTreeInLevels(Node* root, int index = 0) {
	if(root == NULL) {
		return;
	}

	if(DISPLAY_OUTS && index == 0) {
		cout << endl << endl << endl;
	}

	int level = getLevelFromIndex(index);
	if(DISPLAY_OUTS) {
		cout << getTabs(level) << "Node " << index;
		//root->printContentForOverleafFormat();
		cout << endl;
	}

	printTreeInLevels(root->left, getLeftChild(index));
	printTreeInLevels(root->right, getRightChild(index));
}


int summary(Node* root, int index = 0, int totalAcertos = 0) {
	if(root == NULL) {
		return 0;
	}

	if(DISPLAY_OUTS && index == 0) {
		cout << endl << endl << endl;
	}

	int level = getLevelFromIndex(index);
	string nodeType = (root->left == NULL && root->right == NULL) ? "Folha" : "Galho";
	unsigned int acertos = 0;
	unsigned int erros = 0;

	if(DISPLAY_OUTS) {
		cout << nodeType << " " << index << endl;

		if(nodeType == "Folha") {
			vector<unsigned int> numberElementsByClass;
			numberElementsByClass.resize(Problem::JClasses);

			// Salva em 'elements' a classe na qual o elemento foi classificado no respectivo índice do elemento
			for(unsigned int i = 0; i < root->dataset->dataset.size(); i++) {
				vector<float> line = root->dataset->dataset[i];
				int _class = line[Problem::PAttributes];
				numberElementsByClass[_class]++;

				// Verifica se a classe do elemento é a mesma do nó folha que ele está inserido
				if(_class == root->dataset->majoritaryClass) {
					acertos++;
				}
			}
			erros = root->dataset->dataset.size()-acertos;

			cout << "Elementos: " << root->dataset->dataset.size()
					<< "\tRótulo: " << root->dataset->majoritaryClass
					<< "\tImpureza: " << std::setprecision(16) << root->dataset->impurity <<  endl;

			float acertosPercent = float(acertos)/root->dataset->dataset.size()*100;
			float errosPercent = float(erros)/root->dataset->dataset.size()*100;

			cout << std::fixed;
			cout << "Acertos: " << acertos << "\t(" << std::setprecision(2) << acertosPercent << "%)" << endl;
			cout << "Erros: " << erros << "\t(" << std::setprecision(2) << errosPercent << "%)" << endl;
			cout << "-----------" << endl;

			for(unsigned int _class = 0; _class < numberElementsByClass.size(); _class++) {
				cout << "Classe " << _class << ":  " << numberElementsByClass[_class] << endl;
			}

			totalAcertos += acertos;

		}
		else {
			cout << "Atributo[" << root->dataset->splitAttribute << "] = " << std::setprecision(16) << root->dataset->splitValue << endl;
		}
		cout << endl << endl;
	}

	int acertosR = summary(root->left, getLeftChild(index), totalAcertos);
	int acertosL = summary(root->right, getRightChild(index), totalAcertos);
	Problem::numberOfCorrects = acertos+acertosR+acertosL;
	Problem::numberOfWrongs = Problem::numberOfElemetsInProblem-Problem::numberOfCorrects;
	return acertos+acertosR+acertosL;

}

int main(int argc, char** argv){

	ostringstream results;

	if(argc > 1) {
		DISPLAY_OUTS = atoi(argv[1]);
	}

	if(DISPLAY_OUTS) {
		if(argc == 1) {
			cout << "Você também pode passar os seguintes parametros:" << endl;
			cout << "[1] Mostrar Saídas no terminal (1 - Sim, 0 - Não)" << endl;
			cout << "[2] Arquivo do dataset (padrão: csv/iris.norm.csv)" << endl;
			cout << "[3] Quantidade de atributos: (padrão: 4)" << endl;
			cout << "[4] Quantidade de classes: (padrão: 3)" << endl;
			cout << "[5] Classes separadas por vírgula ('0,1,2')" << endl;
			cout << "[6] Diferença mínima no melhoramento da impureza: (padrão: 0.00000001)" << endl;
			cout << "[7] Altura máxima da árvore: (padrão: 3)" << endl;
			cout << endl;

			cout << "Deseja continuar mesmo assim? [S ou n]: ";
			char continuar;
			cin >> continuar;
			if(continuar != 's' && continuar != 'S') {
				return 0;
			}
		}
	}

	//char caracter;
	int n; // = 150; // Total de elementos
	int PAttributes = 4; // Quantidade de atributos (deve ser)
	int JClasses = 3; // Quantidade de classes
	string classesInLine = "0,1,2"; // Todas as possíveis classes do problema
	float threshold = 0.00000001; // Mínima diferença de impureza de um nó pai para um nó filho
	int maxHeight = 3; // Altura máxima da árvore


	// Recebe o nome do arquivo por parâmetro quando é passado
	if(argc > 1) {
		if(DISPLAY_OUTS) { cout << "1 - Mostrar saídas:\t" << (DISPLAY_OUTS ? "Sim" : "Não") << endl; }
	}

	// Recebe o nome do arquivo por parâmetro quando é passado
	if(argc > 2) {
		fileName = string(argv[2]);
		if(DISPLAY_OUTS) { cout << "2 - fileName:\t" << fileName << endl; }
	}

	// Recebe o número de atributos
	if(argc > 3) {
		PAttributes = atoi(argv[3]);
	}
	results << "3 - N. atributos:\t" << PAttributes << endl;

	// Recebe o número de classes
	if(argc > 4) {
		JClasses = atoi(argv[4]);
	}
	results << "4 - N. classes:\t" << JClasses << endl;

	// Recebe o valor mínimo para a diferença do melhoramento da impureza
	if(argc > 5) {
		classesInLine = argv[5];
	}
	results << "5 - Classes:\t" << classesInLine << endl;

	// Recebe o valor mínimo para a diferença do melhoramento da impureza
	if(argc > 6) {
		threshold = stod(argv[6]);
		cout << std::fixed << std::setprecision(8);
	}
	results << "6 - Impureza Mínima:\t" << threshold << endl;

	// Recebe o valor mínimo para a diferença do melhoramento da impureza
	if(argc > 7) {
		maxHeight = stod(argv[7]);
	}
	results << "7 - Altura::\t" << maxHeight << endl;


	if(DISPLAY_OUTS){ cout << results.str(); }

	// Criando um dicionário de classes
	stringstream _lineClasses(classesInLine);
	string _class;

	while(getline(_lineClasses, _class, ',')) {
		problemClasses.push_back(_class);
		_class.clear();
	}

	if(DISPLAY_OUTS && DEBUG) {
		for(unsigned int i = 0; i < problemClasses.size(); i++) {
			cout << problemClasses[i] << ",\t";
		}
		cout << endl;
	}


	vector < vector < float > > x;
	fstream arquivo;
	arquivo.open(fileName, fstream::in);
	/* Testando leitura de arquivo */
	string aux;
	string line;
	string word;
	const char SEPARATOR = ',';

	// Descartando a primeira linha contendo as legendas
	arquivo >> aux;


	// Lendo os dados do arquivo
	//float dAux;
	string cl;


	// LENDO ARQUIVO CSV SEM ESPAÇAMENTO DEPOIS DA VÍRGULA
	if(!arquivo.is_open()) {
		if(DISPLAY_OUTS && DEBUG) { cout << "O arquivo não pode ser aberto." << endl; }
		return 0;
	}

	// A contagem de classes será feita em uma estrutura de map (chave-valor)
	// já durante a leitura do arquivo
	//set<string> setClasses;

	// Zerando a quantidade de elementos inseridos.
	n = 0;
	getline(arquivo, line); // descartando a primeira linha
	for(int i = 0; !arquivo.eof(); i++) {
		//x[i].resize(p+1);
		vector<float> attributes(PAttributes);

		// Lê uma linha do arquivo
		line.clear();
		getline(arquivo, line);

		if(DEBUG && DEBUG){ cout << "Linha do arquivo: " << line << endl; }

		// Se encontrar uma linha em branco, seja no meio ou no fim do arquivo, continua a na próxima linha
		// Até encontrar o fim do arquivo.
		if(!line.compare("")) {
			continue;
		}

		//transforma a string num fluxo de dados (stream)
		stringstream str(line);
		word.clear();
		int j = 0;

		while(getline(str, word, SEPARATOR)) {

			// Converte apenas os atributos numericos que estão nos índices 1,2,3,4;
			if(j > 0 && j <= PAttributes) {
				// Ao copiar para o novo vetor e descartar o ID, o dados vão de line[j] -> attributes[j-1].
				attributes[j-1] = stod(word);
			}
			// PAttributes+1 é o índice onde está a classe
			else if(j == PAttributes+1) {
				// DEPRECATED HARDCODE: Convertendo a classe de string para float
				//attributes.push_back(word.compare("Iris-setosa") == 0 ? 0 : (word.compare("Iris-versicolor") == 0 ? 1 : 2));


				// SOFTCODE:
				// Adiciona todos os valores de classes a um conjunto, desta forma
				// será possível tê-los associados à um índice
				////setClasses.insert(word);
				// Obtem o índice equivalente posição que a classe foi inserida na estrutura set
				////unsigned int index = std::distance(setClasses.begin(), setClasses.find(word));

				// Obtem o índice equivalente posição que a classe foi inserida na estrutura set
				unsigned int index = findStringInVector(word, problemClasses);

				// Se não encontra a classe no vetor, então ocorreu um erro
				unsigned int last = problemClasses.size()-1;
				if(index < 0 || index > last) {
					cerr << "Houve um erro ao tentar encontrar a classe no vetor: findStringInVector(word, problemClasses)" << endl;
					break;
				}

				// Associa o elemento à classe representada aqui equivalentemente pelo seu índice
				attributes.push_back(index);
			}

			word.clear();
			j++;
		}

		//if(DISPLAY_OUTS) { cout << endl; }
		// Colocando os atributos no índice do elemento
		x.push_back(attributes);

		// Contando a quantidade de elementos inseridos
		n++;
	}

	// FIM DA LEITURA DO ARQUIVO CSV COM ESPAÇAMENTO DEPOIS DA VÍRGULA

	if(DISPLAY_OUTS) { cout << "N. elementos encontr.:\t" << n << endl; }

	if(DISPLAY_OUTS && DEBUG) {
		cout << "Matriz de dados original:" << endl;
		for(unsigned int i = 0; i < x.size(); i++) {
			cout << i << "\t";
			for(unsigned int j = 0; j < x[i].size(); j++) {
				cout << x[i][j] << "\t";
			}
			cout << endl;
		}
	}

	// FIM DA LEITURA DO ARQUIVO CSV SEM ESPAÇAMENTO DEPOIS DA VÍRGULA

	if(DISPLAY_OUTS) { cout << endl << endl << endl << endl; }


	// VARIÁVEIS GLOBAIS DO PROBLEMA
	//unsigned int numberOfElemetsInProblem;
	vector<float> numberOfClassElementsInProblem;
	vector<float> estimateRateOfClassElemetsInProblem;



	// --- Fim das variáveis do problema ---
	Problem::init(x, PAttributes, JClasses, threshold);
	Problem::print(results);


	Dataset* dataset = new Dataset(x,PAttributes,JClasses);
	Node* root = new Node(dataset);

	root->toSplitAll(0, maxHeight);


	//cout << "não está chegando aqui" << endl;

	root->print(0, "root", -1, false);


	if(DISPLAY_OUTS && DEBUG) {
		cout << "Altura da árvore:\t" << Problem::hight << endl;
		cout << "Número de galhos:\t" << Problem::numberOfBranches << endl;
		cout << "Número de folhas:\t" << Problem::numberOfLeafs << endl;

		cout << endl;
		cout << endl;
	}
	//root->draw();

	string treeSettingsFile = "tree-settings.txt";
	fstream treeSettingsStream;
	treeSettingsStream.open(treeSettingsFile, fstream::out);
	root->preOrder(treeSettingsStream);
	if(DISPLAY_OUTS) {cout << endl << endl; }
	root->preOrderComIndices(results);

	//printTreeInLevels(root);

	summary(root);
	Problem::print(results);

	treeSettingsStream.close();


	if(DISPLAY_OUTS) { cout << results.str(); }

	if(DISPLAY_OUTS) { cout << endl << endl; }

	return 0;
}
