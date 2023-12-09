//Implementation File: grid.cpp
#include "grid.h"
#include <iostream>

//Constructor
Grid::Grid() {}

//Public Member Functions
void Grid::print() {
	//Top Bar
	std::cout << "-------------------------------\n";

	for(int r = 1; r <= 9; r++) {

		for(int t = 0; t <= 2; t++) {
			std::cout << "|";

			for(int b = 1; b <=9; b++) {
				this->data[r-1][b-1].printRow(t);

				if(b%3 == 0)
					std::cout << "|";
			}

			std::cout << std::endl;
		}

		if(r%3 == 0)
			std::cout << "-------------------------------\n";
	}
}

void Grid::print(bool b) {
	//Top Bar
	std::cout << "-------------------------------------------------------------------------------------\n";

	for(int r = 1; r <= 9; r++) {

		for(int t = 0; t <= 2; t++) {
			std::cout << "|";

			for(int b = 1; b <=9; b++) {
				this->data[r-1][b-1].printRow(t);

				if(b%3 == 0)
					std::cout << "|";
			}

			std::cout << std::endl;
		}

		if(r%3 == 0)
			std::cout << "-------------------------------------------------------------------------------------\n";
	}
}

std::string Grid::getFromAPV(const int& r, const int& c, const int& index) {
	return this->data[r][c].getFromAPV(index);
}

int Grid::getAPVSize(const int& r, const int& c) {
	return this->data[r][c].getAPVSize();
}

void Grid::updateBlockAPVs(const int& r, const int& c, const std::string& val) {
	int xINIT, xFIN, yINIT, yFIN;
	getBlockXYs(xINIT, xFIN, yINIT, yFIN, r, c);

	for(int x = xINIT; x <= xFIN; x++) {
		for(int y = yINIT; y <= yFIN; y++)
			this->data[x][y].deletePossibleValueOf(val);
	}

	for(int var = 0; var <= 8; var++) {
		this->data[r][var].deletePossibleValueOf(val);
		this->data[var][c].deletePossibleValueOf(val);
	}
}

bool Grid::isFilled() {

	for(int i = 0; i < 9; i++) {
		for(int j = 0; j < 9; j++) {
			if(data[i][j].getVal() == " ")
				return false;
		}
	}

	return true;
}

bool Grid::hasX() {

	for(int i = 0; i < 9; i++) {
		for(int j = 0; j < 9; j++) {
			if(data[i][j].getVal() == "X")
				return true;
		}
	}

	return false;
}

bool Grid::isEmptyAt(const int& r, const int& c) {
	if(this->data[r][c].getVal() == " ")
		return true;
	return false;
}

bool Grid::checkRowForDuplicates(const int& r, const int& c, const std::string& val) { //Returns true when it has duplicates
	for(int var = 0; var <= 8; var++) {
		if(this->data[r][var].getVal() == val && var != c)
			return true;
	}

	return false;
}

bool Grid::checkColumnForDuplicates(const int& r, const int& c, const std::string& val) { //Returns true when it has duplicates
	for(int var = 0; var <= 8; var++) {
		if(this->data[var][c].getVal() == val && var != r)
			return true;
	}
	
	return false;
}

bool Grid::checkBoxForDuplicates(const int& r, const int& c, const std::string& val) { //Returns true when it has duplicates
	int xINIT, xFIN, yINIT, yFIN;

	getBlockXYs(xINIT, xFIN, yINIT, yFIN, r, c);

	for(int x = xINIT; x <= xFIN; x++) {
		for(int y = yINIT; y <= yFIN; y++) {
			if(this->data[x][y].getVal() == val && !(x == r && y == c))
				return true;
		}
	}

	return false; //Only reaches here when no duplicates are found
}

bool Grid::checkInsertValidity(const int& r, const int& c, const std::string& val) { //Returns false when the insert is invalid
	bool b = this->checkBoxForDuplicates(r, c, val);

	if(b)
		return false;

	bool d = this->checkColumnForDuplicates(r, c, val);

	if(d)
		return false;

	bool e = this->checkRowForDuplicates(r, c, val);

	if(e)
		return false;

	return true;
}

void Grid::insert(const int& r, const int& c, const std::string& val) { 
	this->data[r][c].setVal(val);
	updateBlockAPVs(r, c, val);

	//Below lines in this function will not work when using our 'X' test case

	// bool a = this->checkInsertValidity(r, c, val);
	// if(!a)
	// 	this->data[r][c].setVal(" ");
}

// Block Grid::getBlock(const int& r, const int& c) {
// 	return data[r][c];
// }

std::string Grid::getValFromBlockAt(const int& r, const int& c) {
	return data[r][c].getVal();
}

void Grid::addContainer(Container c) {
	this->allContainersG.push_back(c);
}

std::vector<Container> Grid::getAllContainersG() {
	return this->allContainersG;
}

void Grid::updateContainerSum(std::pair<int, int> pair, int valToAdd) {
	for(Container c : allContainersG) {
		std::vector<std::pair<int,int>> vecPairs = c.getCBI();
		for(std::pair<int,int> somePair : vecPairs) {
			if(somePair == pair) 
				c.updateCurrentSum(valToAdd);
		}
	}
}

void Grid::surroundBlockAt(const int& r, const int& c, std::string surroundVal, bool b) {
	data[r][c].surround(surroundVal, b);
}

void Grid::emptyThisAtBlock(const int& r, const int& c, std::string str, bool b) {
	data[r][c].emptyThis(str, b);
}

void Grid::padAll() {
	for(int i = 0; i <= 8; i++) {
		for(int j = 0; j <=8; j++) {
			this->data[i][j].pad(true);
		}
	}
}

void Grid::placeInCorner(const int& r, const int& c, std::string val) {
	this->data[r][c].placeInCorner(val);
}

//Non Member Functions

void getBlockXYs(int& xINIT, int& xFIN, int& yINIT, int& yFIN, const int& r, const int& c) {
	if(r>=0 && r<=2) {
		xINIT = 0;

		if(c>=0 && c<=2)
			yINIT = 0;
		else if(c>=3 && c<=5)
			yINIT = 3;
		else
			yINIT = 6;
	} else if(r>=3 && r<=5) {
		xINIT = 3;

		if(c>=0 && c<=2)
			yINIT = 0;
		else if(c>=3 && c<=5)
			yINIT = 3;
		else
			yINIT = 6;
	} else {
		xINIT = 6;

		if(c>=0 && c<=2)
			yINIT = 0;
		else if(c>=3 && c<=5)
			yINIT = 3;
		else
			yINIT = 6;
	}

	xFIN = xINIT + 2;
	yFIN = yINIT + 2;
}

// //Overloaded Operator
// bool operator==(Grid& g1, Grid& g2) {
// 	bool same = true;

// 	for(int i = 0; i <= 8; i++) {
// 		for(int j = 0; j <= 8; j++) {

// 			if(g1.getValFromBlockAt(i,j) != g2.getValFromBlockAt(i,j))
// 				same = false;

// 		}
// 	}

// 	return same;
// }