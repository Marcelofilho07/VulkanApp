//
// Created by Marce on 3/22/2024.
//

#include <chrono>
#include <iostream>
#include "QuickSort.h"

QuickSort::QuickSort(std::vector<Vertex> randomVec)
{
	ValuesVec = std::move(randomVec);
}

void QuickSort::qSort(std::vector<Vertex>& vToSort, int vBegin, int vEnd)
{

	/*const int pivot = vToSort[(vBegin + vEnd) / 2];
	int i = vBegin;
	int j = vEnd - 1;
	while(i <= j)
	{
		while (vToSort[i] < pivot && i < vEnd)
		{
			++i;
		}
		while (vToSort[j] > pivot && j > vBegin)
		{
			--j;
		}
		if(i <= j)
		{
			int aux = vToSort[i];
			vToSort[i] = vToSort[j];
			vToSort[j] = aux;
			i++;
			j--;
		}
	}

	if(j > vBegin)
	{
		qSort(vToSort, vBegin, j + 1);
	}
	if(i < vEnd)
	{
		qSort(vToSort, i, vEnd);
	}*/

}

void QuickSort::Sort()
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	qSort(ValuesVec, 0, ValuesVec.size());

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "QUICK SORT SIZE: " << ValuesVec.size() << " - TIME: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[µs] , " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs], "<< std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl << std::endl;
}
