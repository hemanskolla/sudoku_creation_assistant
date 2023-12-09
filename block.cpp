//Implementation File: block.cpp
#include "block.h"
#include <iostream>

//Constructors
Block::Block() { // Fills block entirely with empty spaces

	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++)
			this->data[i][j] = " ";
	}

	//Populates allPossibleValues
	for(int k = 1; k <=9; k++)
		this->allPossibleValues.insert(std::to_string(k));

}

Block::Block(const std::string& val) { // Fills center with val and all borders with empty spaces

	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			if(i==1 && j==1)
				this->data[i][j] = val;
			else
				this->data[i][j] = " ";
		}
	}

	//Populates allPossibleValues
	for(int k = 1; k <=9; k++)
		this->allPossibleValues.insert(std::to_string(k));

}


Block::Block(const std::string& val, bool b) { // Fills center with val and all borders with empty spaces

	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			if(i==1 && j==1)
				this->data[i][j] = " " + val + " ";
			else
				this->data[i][j] = " ";
		}
	}

	//Populates allPossibleValues
	for(int k = 1; k <=9; k++)
		this->allPossibleValues.insert(std::to_string(k));

}

//Accessors
const std::string Block::getVal() { return this->data[1][1]; }
int Block::getAPVSize() { return allPossibleValues.size(); }
const std::string Block::getFromAPV(const int& index) { 
	std::set<std::string>::iterator iter = allPossibleValues.begin();

	for(int start = 0; start < index; start++)
		iter++;

	return *iter;
}

//Modifiers
void Block::setVal(const std::string val) { this->data[1][1] = val; }

void Block::setVal(const std::string val, bool b) { 
	this->data[1][1] = " " + val + " ";
}

void Block::deletePossibleValueOf(const std::string val) {
	std::set<std::string>::iterator iter = allPossibleValues.find(val);

	if(iter != allPossibleValues.end())
		allPossibleValues.erase(iter);
}

//Public Member Functions
void Block::printRow(const int rowIndex) {

	// std::string row[] = this->getRow(rowIndex);

	for(std::string s : this->data[rowIndex])
		std:: cout << s;

}

void Block::surround(std::string surroundVal, bool b) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			if(!(i==1 && j==1)) {
				this->data[i][j] = surroundVal;
				this->data[i][j] += surroundVal;
				this->data[i][j] += surroundVal;
			}
		}
	}
}

void Block::surround(std::string surroundVal) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			if(!(i==1 && j==1)) {
				this->data[i][j] = surroundVal;
			}
		}
	}
}
	
void Block::emptyThis(std::string str, bool b) {
	if(str == "R")
		data[1][2] = "   ";
	if(str == "T")
		data[0][1] = "   ";
	if(str == "B")
		data[2][1] = "   ";
	if(str == "L")
		data[1][0] = "   ";
}

void Block::emptyThis(std::string str) {
	if(str == "R")
		data[1][2] = " ";
	if(str == "T")
		data[0][1] = " ";
	if(str == "B")
		data[2][1] = " ";
	if(str == "L")
		data[1][0] = " ";
}

void Block::pad(bool b) {
	std::string dVal = this->data[1][1];
	this->data[1][1] = " " + dVal + " ";
}

void Block::placeInCorner(std::string val) {
	if(val.length() == 1) {
		this->data[0][0] = " " + val + " ";
	} else if(val.length() == 2) {
		this->data[0][0] = val + " ";
	} else if(val.length() == 3) {
		this->data[0][0] = val + "  ";
	}
}
