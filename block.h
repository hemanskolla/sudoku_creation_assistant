//Header File: block.h
#ifndef __block_h
#define __block_h
#include <string>
#include <set>

class Block {
public:
	//Constructors
	Block();
	Block(const std::string& val);
	Block(const std::string& val, bool b);

	//Accessors
	const std::string getVal();
	int getAPVSize();
	const std::string getFromAPV(const int& index);

	//Modifiers
	void setVal(const std::string val);
	void deletePossibleValueOf(const std::string val);
	void surround(std::string surroundVal);
	void emptyThis(std::string str);

	//Public Member Functions
	void printRow(const int rowIndex);

	//Killer Sudoku Specific
	void surround(std::string surroundVal, bool b);
	void setVal(const std::string val, bool b);
	void emptyThis(std::string str, bool b);
	void pad(bool b);
	void placeInCorner(std::string val);

private:
	//Private Member Variables
	std::string data[3][3];
	std::set<std::string> allPossibleValues;

};

#endif