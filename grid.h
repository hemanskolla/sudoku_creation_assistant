//Header File: grid.h
#ifndef __grid_h
#define __grid_h
#include "block.h"
#include "container.h"
#include <vector>

class Grid {
public:
	//Constructor
	Grid();

	//Public Member Functions
	void print();
	std::string getFromAPV(const int& r, const int& c, const int& index);
	int getAPVSize(const int& r, const int& c);
	void updateBlockAPVs(const int& r, const int& c, const std::string& val);

	bool isFilled();
	bool hasX();
	bool isEmptyAt(const int& r, const int& c);

	bool checkRowForDuplicates(const int& r, const int& c, const std::string& val);
	bool checkColumnForDuplicates(const int& r, const int& c, const std::string& val);
	bool checkBoxForDuplicates(const int& r, const int& c, const std::string& val);

	bool checkInsertValidity(const int& r, const int& c, const std::string& val);
	void insert(const int& r, const int& c, const std::string& val);

	std::string getValFromBlockAt(const int& r, const int& c);
	void addContainer(Container c);
	std::vector<Container> getAllContainersG();
	void updateContainerSum(std::pair<int, int> pair, int valToAdd);

	//Killer Sudoku Specific
	void print(bool b);
	void surroundBlockAt(const int& r, const int& c, std::string surroundVal, bool b);
	void emptyThisAtBlock(const int& r, const int& c, std::string str, bool b);
	void padAll();
	void placeInCorner(const int& r, const int& c, std::string val);

	// Block getBlock(const int& r, const int& c);

private:
	//Private Member Variables
	Block data[9][9];
	std::vector<Container> allContainersG;

};

//Non Member Functions
void getBlockXYs(int& xINIT, int& xFIN, int& yINIT, int& yFIN, const int& r, const int& c);

//Overloaded Operator
// bool operator==(const Grid& g1, const Grid& g2);

#endif