//
// Created by Marce on 3/22/2024.
//

#ifndef T01_QUICKSORT_H
#define T01_QUICKSORT_H


#include <vector>

#include "WingedEdge.h"
#include "VulkanApp.cpp"

class QuickSort {
public:
	std::vector<Vertex> ValuesVec;

	QuickSort(std::vector<Vertex> randomVec);

	void Sort();

private:
	void qSort(std::vector<Vertex>& vToSort, int vBegin, int vEnd);
};


#endif //T01_QUICKSORT_H
