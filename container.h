//Header File: container.h
#ifndef __container_h
#define __container_h
#include "block.h"
#include <vector>

class Container {
public:
	//Constructor
	Container();

	//Modifier
	void addContainedBlock(const int& r, const int& c);
	void updateExpectedSum(const int& es);
	void updateCurrentSum(const int& valToAdd);

	//Accessor
	std::vector<std::pair<int,int>> getCBI();
	int getExpectedSum();
	int getCurrentSum();

	//Public Helper Function
	int getOverflow();

private:
	//Private Member Variables
	int numBlocksContained;
	// std::vector<Block*> containedBlocks;
	std::vector<std::pair<int,int>> containedBlockIndexes;

	int expectedSum;
	int currentSum; //Does not seem to be wokring
};
#endif