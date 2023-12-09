//Implementation File: container.cpp
#include "container.h"

//Constructor
Container::Container() {
	this->numBlocksContained = 0;
	this->currentSum = 0;
}

//Modifier
void Container::addContainedBlock(const int& r, const int& c) { 
	this->containedBlockIndexes.push_back(std::make_pair(r,c));
	this->numBlocksContained++;

	// std::string temp = b->getVal();
	// int toAdd;

	// if(temp != " " || "X")
	// 	toAdd = std::stoi(temp);

	// this->currentSum += toAdd;
}

void Container::updateExpectedSum(const int& es) { this->expectedSum = es; }
void Container::updateCurrentSum(const int& valToAdd) { this->currentSum += valToAdd; }

//Accessor
std::vector<std::pair<int,int>> Container::getCBI() { return this->containedBlockIndexes; }
int Container::getExpectedSum() { return this->expectedSum; }
int Container::getCurrentSum() { return this->currentSum; }

//Public Helper Function
int Container::getOverflow() {
	return expectedSum - currentSum;
}