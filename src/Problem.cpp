/*
 * Problem.cpp
 *
 *  Created on: 24 de fev. de 2022
 *      Author: jader
 */

#include "Problem.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <iomanip>

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

using namespace std;

// === ATRIBUTOS DO PROBLEMA ===
unsigned int Problem::numberOfElemetsInProblem;
unsigned int Problem::JClasses;
unsigned int Problem::PAttributes;
vector<vector<float>> Problem::data;
vector<float> Problem::numberOfClassElementsInProblem;
vector<float> Problem::estimateRatioOfClassElemetsInProblem;
int Problem::numberOfBranches;
int Problem::numberOfLeafs;
int Problem::hight;
float Problem::threshold;
int Problem::numberOfCorrects;
int Problem::numberOfWrongs;

Problem::Problem(){

}

Problem::~Problem() {
	// TODO Auto-generated destructor stub
}

void Problem::init(vector<vector<float>> data,
				unsigned int PAttributes,
				unsigned int JClasses,
				float threshold
		){
	Problem::data = data;
	Problem::numberOfElemetsInProblem = data.size();
	Problem::PAttributes = PAttributes;
	Problem::JClasses = JClasses;
	Problem::calcRatio();
	Problem::numberOfBranches = 0;
	Problem::numberOfLeafs = 0;
	Problem::hight = 0;
	Problem::threshold = threshold;
	Problem::numberOfCorrects = 0;
	Problem::numberOfWrongs = 0;
}

// Calcular quantos elementos são de cada classe e suas proporções.
bool Problem::calcRatio() {
	vector<string> messages;

	// Verificar inconsistências
	if(Problem::numberOfElemetsInProblem < 2) {
		messages.push_back("O número de elementos do conjunto não pode ser menor que 2.");
	}
	if(Problem::PAttributes < 1) {
		messages.push_back("O número de atributos do conjunto não pode ser menor que 1.");
	}
	if(Problem::JClasses < 2) {
		messages.push_back("O número de classes do conjunto não pode ser menor que 2.");
	}
	if(Problem::numberOfElemetsInProblem != Problem::data.size()) {
		messages.push_back("O número de elementos informado é diferente do que realmente há nos dados.");
	}
	if(Problem::PAttributes != Problem::data[0].size()) {
		messages.push_back("É possível que o número informado de atributos seja diferente dos existentes nos dados");
	}

	// Contar total de elementos de cada classe
	Problem::numberOfClassElementsInProblem.resize(Problem::JClasses);
	// Zerando contadores
	for(unsigned int j = 0; j < Problem::JClasses; j++) {
		Problem::numberOfClassElementsInProblem[j] = 0;
	}
	// Contanto quantos elementos há em cada classe
	for(unsigned int i = 0; i < Problem::numberOfElemetsInProblem; i++) {
		int classe = (int) Problem::data[i][Problem::PAttributes];
		Problem::numberOfClassElementsInProblem[classe]++;
	}

	// Calcular a proporção de elementos de cada classe
	Problem::estimateRatioOfClassElemetsInProblem.resize(Problem::JClasses);
	for(unsigned int j = 0; j < Problem::JClasses; j++) {
		Problem::estimateRatioOfClassElemetsInProblem[j] = (float) Problem::numberOfClassElementsInProblem[j] / Problem::numberOfElemetsInProblem;
	}

	return (messages.size() == 0) ? true : false;
}

// Imprime o objeto Problem
void Problem::print(ostringstream &results) {

	/*
	 * === FIT ==========================================

		N. elementos trein.:	150
		N. galhos:		2
		N. folhas:		3
		Altura real/max:	2 / 2
		Total acertos:		144	(96.000%)
		Total erros:		6	(4.000%)
		Tempo gasto:		0h0min6s882ms

		-----------------------------

		Solução:		Optimal
		Valor objetivo:		0.720
		N. Variáveis:		654
		GAP:			0.00%

		-----------------------------

		Parametros de entrada:
		CSV:			csv/iris.norm.csv
		N. Elementos:		150
		Atributos:		4
		Classes:		3 (0,1,2)
		Altura max.:		2
		Alpha:			0.30
		Min. elem. folha:	1
		Tempo Limite:		0h2m

		==================================================
	 */

	if(DISPLAY_OUTS) {
		results << "=== FIT ==========================================";

		cout << "Número de elementos no problema:\t" << Problem::numberOfElemetsInProblem << endl;
		cout << "Número de classes no problema:\t\t" << Problem::JClasses << endl;
		cout << "Número de atributos no problema:\t" << Problem::PAttributes << endl;

		// Evitando violação de memória, caso não haja elementos no vetor
		int nAttributes = (Problem::data.size() > 0) ? Problem::data[0].size() : 0;
		cout << "Dados:\t" << Problem::data.size() << "x" << nAttributes << endl;

		cout << "Número de elementos de cada classe:\t" << endl;
		for(unsigned int j = 0; j < Problem::numberOfClassElementsInProblem.size(); j++) {
			cout << "[" << j << "]\t" << Problem::numberOfClassElementsInProblem[j] << '\t' << setprecision(7) << Problem::estimateRatioOfClassElemetsInProblem[j]*100 << "%" << endl;
		}

		cout << "Número de galhos:\t" << Problem::numberOfBranches << endl;
		cout << "Número de folhas:\t" << Problem::numberOfLeafs << endl;


		cout << std::fixed << std::setprecision(2);

		float percent;
		percent= float(Problem::numberOfWrongs)/Problem::numberOfElemetsInProblem*100;
		cout << "Erros: " << Problem::numberOfWrongs << "\t(" << percent << "%)" << endl;

		percent = float(Problem::numberOfCorrects)/Problem::numberOfElemetsInProblem*100;
		cout << "Acertos: " << Problem::numberOfCorrects << "\t(" << percent << "%)" << endl;
	}
}






















