/*
 * DatasetGiniIndex.h
 *
 *  Created on: 20 de fev. de 2022
 *      Author: jader
 */

#ifndef DATASETGINIINDEX_H_
#define DATASETGINIINDEX_H_

#include "Dataset.h"

class DatasetGiniIndex: public Dataset {
public:

	// METHODS ***
	DatasetGiniIndex();
	virtual ~DatasetGiniIndex();

	// Retorna a impureza do nó baseado no Índice Gini
	double calcImpurityByGiniIndex();

};

#endif /* DATASETGINIINDEX_H_ */
