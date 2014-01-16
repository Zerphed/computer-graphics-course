/*
 * clothSystem.h
 *
 *  Created on: Nov 1, 2011
 *      Author: pboyer
 */

#ifndef CLOTHSYSTEM_H_
#define CLOTHSYSTEM_H_

#include "pendulumSystem.h"

class ClothSystem: public PendulumSystem {
	public:
		ClothSystem(unsigned rows, unsigned columns);
		virtual ~ClothSystem();

		void draw();

		void shiftSystem(bool forward);

	private:
		// Mapping 2D index -> 1D index
		// 2D: [i][j] -> 1D: i*cols + j
		inline int getLinearIndex(int rowIndex, int colIndex) {
			return (rowIndex*c) + colIndex;
		}

		void initClothSystem();

		unsigned r,c;

		// This is the list of faces (indices into vecv and vecn)
		vector<vector<unsigned> > vecf;
};

#endif /* CLOTHSYSTEM_H_ */
