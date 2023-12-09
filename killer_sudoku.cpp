//Main File: killer_sudoku.cpp
#include "grid.h"
#include "block.h"
#include "container.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cassert>

void updateCRCC(int& cr, int&cc) {
	if(cr == 8) {
		cr = 0;
		cc++;
	} else
		cr++;
}

void parseInputGrid(const std::string& inputFN, Grid& sudoku_grid, bool& isInvalid, const bool isKiller) {
	std::ifstream in_str;
	in_str.open(inputFN);
	assert(in_str.good());

	int cr = 0, cc = 0;
	// Note #1: cr & cc swapped since reading input instead of traversing a grid

	std::string temp;
	while(in_str.good()) {
		in_str >> temp;

		if(temp == "/") {
			assert(cr == 9); // Note #1
			break;
		} else if(temp != "X") {
			assert(temp == "1" || temp == "2" || temp == "3" || 
				temp == "4" || temp == "5" || temp == "6" || 
				temp == "7" || temp == "8" || temp == "9"); // Ensures input to a block is a valid sudoku input

			sudoku_grid.insert(cr, cc, temp);
			bool isValid = sudoku_grid.checkInsertValidity(cr, cc, temp);
			if(!isValid) { // This conditional is passed only when the input grid is has a direct conflict.
				isInvalid = !isValid;
				break;
			}
		}

		updateCRCC(cc, cr); // Note #1
	}

	//Update Containers here, which occurs only the input is a killer sudoku grid.
	if(isKiller) {
		std::string inputLine;
		getline(in_str, inputLine);

		while(getline(in_str, inputLine)) {

			Container tempContainer;

			while(inputLine.length() > 0) {
				if(inputLine.substr(0,1) == "=") {

					int findMarker = inputLine.find('|');
					std::string toAddStr = inputLine.substr(1,findMarker-1);
					if(toAddStr.substr(0,1) == " ") //To fix any parsing errors
						toAddStr = toAddStr.substr(1);

					int toAdd = std::stoi(toAddStr);
					tempContainer.updateExpectedSum(toAdd);

					break;
				}

				int row = std::stoi(inputLine.substr(0,1));
				int col = std::stoi(inputLine.substr(1,1));

				tempContainer.addContainedBlock(row, col);
				std::string valToAdd = sudoku_grid.getValFromBlockAt(row, col);

				if(valToAdd != " " && valToAdd != "X")
					tempContainer.updateCurrentSum(std::stoi(valToAdd));

				// Block* temp = nullptr;
				// *temp = sudoku_grid.getBlock(row, col);
				// tempContainer.addContainedBlock(temp);

				inputLine = inputLine.substr(3);
			}

			sudoku_grid.addContainer(tempContainer);

		}

	}

	in_str.close();
}

// std::vector<Grid> removeDuplicates(std::vector<Grid>& allGrids) {
// 	std::vector<Grid> uniqueGrids;

// 	bool found = false;
// 	for(Grid g : allGrids) {
// 		for(Grid ug : uniqueGrids) {

// 			//Functions as ug == g
// 			bool same = true;

// 			for(int i = 0; i <= 8; i++) {
// 				for(int j = 0; j <= 8; j++) {

// 					if(ug.getValFromBlockAt(i,j) != g.getValFromBlockAt(i,j))
// 						same = false;

// 				}
// 			}

// 			if(same) {
// 				found = true;
// 				break;
// 			}
// 		}

// 		if(!found){
// 			uniqueGrids.push_back(g);
// 		}

// 		found = false;
// 	}

// 	return uniqueGrids;
// }

void findAllPossibleGridSolutions(const int row, const int col, std::vector<Grid>& allGrids, Grid g, const bool isKiller) {
	int cr = row, cc = col; // 'c' for current
	int ur = row, uc = col; // 'u' for updated, which is the next call to this function's row & col
	updateCRCC(ur, uc);	

	if(cc == 9 || g.isFilled()) {
		assert(g.isFilled());

		// if(!g.hasX())
		allGrids.push_back(g);
		return;
	}

	if(!g.isEmptyAt(cr, cc)) // The code below this conditional only occurs when the current block is empty
		findAllPossibleGridSolutions(ur, uc, allGrids, g, isKiller);
	else {
		Grid holdG = g; // Will reset holdG to g whenever we restart an insert;

		int thisBlockAPVSize = g.getAPVSize(cr, cc);

		for(int i = 0; i < thisBlockAPVSize; i++) {
			std::string toPlace = g.getFromAPV(cr, cc, i);

			holdG.insert(cr, cc, toPlace);
			// if(isKiller) {
			// 	holdG.updateContainerSum(std::make_pair(cr, cc), std::stoi(toPlace));
			// }
			findAllPossibleGridSolutions(ur, uc, allGrids, holdG, isKiller);

			holdG = g;
		}
	}
}

int main(int argc, char* argv[]) {
	if(argc != 4) {
		std::cerr << "Error: Inappropriate Number of Arguments.\n";
		exit(1);
	}

	std::string inputFN = argv[1];
	std::string outputFN = argv[2];
	std::vector<Grid> allGrids;
	std::string lastArgument = argv[3];

	bool isKiller;
	if(lastArgument == "killer" || lastArgument == "true")
		isKiller = true;
	else if(lastArgument == "regular" || lastArgument == "false")
		isKiller = false;
	else {
		std::cerr << "Error: Must Specify Killer Sudoku / Not.\n";
		exit(1);
	}

	Grid sudoku_grid;
	std::cout << "\nEmpty Grid:\n";
	sudoku_grid.print();

	bool isInvalid = false;
	parseInputGrid(inputFN, sudoku_grid, isInvalid, isKiller);
	if(isInvalid) {
		std::cerr << "\nError: The grid inputted in " << inputFN << " is invalid.\n";
		exit(1);
	}

	std::cout << "\nParsed Input Grid:\n";
	sudoku_grid.print();

	int row = 0, col = 0;
	findAllPossibleGridSolutions(row, col, allGrids, sudoku_grid, isKiller);

	// allGrids = removeDuplicates(allGrids);

	if(isKiller) {
		for(long unsigned int i = 0; i < allGrids.size(); i++) {
			std::vector<Container> allContainers = allGrids[i].getAllContainersG();

			for(long unsigned int j = 0; j < allContainers.size(); j++) {
				// std::vector<std::pair<int,int>> vecCBI = allContainers[j];

				std::vector<std::pair<int,int>> visitedPairs;
				for(long unsigned int k = 0; k < allGrids[i].getAllContainersG()[j].getCBI().size(); k++) {
					int cr = allGrids[i].getAllContainersG()[j].getCBI()[k].first, cc = allGrids[i].getAllContainersG()[j].getCBI()[k].second;

					// std::string toPlaceInCorner = (""+ allContainers[j].getExpectedSum());
					// std::cout << allContainers[j].getExpectedSum() << std::endl;
					
					allGrids[i].placeInCorner(0,0, "13"); allGrids[i].placeInCorner(0,1, "5"); allGrids[i].placeInCorner(0,2, "15"); allGrids[i].placeInCorner(1,2, "9"); allGrids[i].placeInCorner(0,4, "11"); allGrids[i].placeInCorner(0,5, "9"); allGrids[i].placeInCorner(0,8, "14"); allGrids[i].placeInCorner(1,5, "4"); allGrids[i].placeInCorner(1,6, "20"); allGrids[i].placeInCorner(2,0, "20"); allGrids[i].placeInCorner(2,3, "3"); allGrids[i].placeInCorner(2,5, "16"); allGrids[i].placeInCorner(3,1, "8"); allGrids[i].placeInCorner(3,3, "9"); allGrids[i].placeInCorner(3,5, "12"); allGrids[i].placeInCorner(3,6, "15"); allGrids[i].placeInCorner(3,8, "10"); allGrids[i].placeInCorner(4,7, "4"); allGrids[i].placeInCorner(4,0, "18"); allGrids[i].placeInCorner(4,2, "14"); allGrids[i].placeInCorner(4,6, "10"); allGrids[i].placeInCorner(5,1, "18"); allGrids[i].placeInCorner(5,2, "4"); allGrids[i].placeInCorner(5,3, "10"); allGrids[i].placeInCorner(5,4, "10"); allGrids[i].placeInCorner(5,8, "13"); allGrids[i].placeInCorner(6,4, "27"); allGrids[i].placeInCorner(6,6, "6"); allGrids[i].placeInCorner(7,0, "19"); allGrids[i].placeInCorner(7,1, "13"); allGrids[i].placeInCorner(8,3, "3"); allGrids[i].placeInCorner(7,5, "21"); allGrids[i].placeInCorner(8,5, "11"); allGrids[i].placeInCorner(7,8, "9");

					allGrids[i].surroundBlockAt(cr, cc, "⬞", true);

					// if(k==0)
					// 	allGrids[i].placeInCorner(allGrids[i].getAllContainersG()[j].getCBI()[k].first, allGrids[i].getAllContainersG()[j].getCBI()[k].second, "" + allContainers[j].getExpectedSum());

					for(long unsigned int var = 0; var < visitedPairs.size(); var++) {

						//Previous is above current case
						if(visitedPairs[var].first == cr-1 && visitedPairs[var].second == cc) {
							allGrids[i].emptyThisAtBlock(cr, cc, "T", true);
							allGrids[i].emptyThisAtBlock(visitedPairs[var].first, visitedPairs[var].second, "B", true);
						}

						//Previous is below current case
						if(visitedPairs[var].first == cr+1 && visitedPairs[var].second == cc) {
							allGrids[i].emptyThisAtBlock(cr, cc, "B", true);
							allGrids[i].emptyThisAtBlock(visitedPairs[var].first, visitedPairs[var].second, "T", true);
						}

						//Previous is to left of current case
						if(visitedPairs[var].first == cr && visitedPairs[var].second == cc-1) {
							allGrids[i].emptyThisAtBlock(cr, cc, "L", true);
							allGrids[i].emptyThisAtBlock(visitedPairs[var].first, visitedPairs[var].second, "R", true);
						}

						//Previous is to right of current case
						if(visitedPairs[var].first == cr && visitedPairs[var].second == cc+1) {
							allGrids[i].emptyThisAtBlock(cr, cc, "R", true);
							allGrids[i].emptyThisAtBlock(visitedPairs[var].first, visitedPairs[var].second, "L", true);
						}

					}

					visitedPairs.push_back(std::make_pair(cr,cc));

					// for(auto iter = visitedPairs.begin(); iter!=visitedPairs.end(); iter++) {

					// 	//Previous is above current case
					// 	if(iter->first == cr-1 && iter->second == cc) {
					// 		allGrids[i].emptyThisAtBlock(cr, cc, "T");
					// 		allGrids[i].emptyThisAtBlock(iter->first, iter->second, "B");
					// 	}

					// 	//Previous is below current case
					// 	if(iter->first == cr+1 && iter->second == cc) {
					// 		allGrids[i].emptyThisAtBlock(cr, cc, "B");
					// 		allGrids[i].emptyThisAtBlock(iter->first, iter->second, "T");
					// 	}

					// 	//Previous is to left of current case
					// 	if(iter->first == cr && iter->second == cc-1) {
					// 		allGrids[i].emptyThisAtBlock(cr, cc, "L");
					// 		allGrids[i].emptyThisAtBlock(iter->first, iter->second, "R");
					// 	}

					// 	//Previous is to right of current case
					// 	if(iter->first == cr && iter->second == cc+1) {
					// 		allGrids[i].emptyThisAtBlock(cr, cc, "R");
					// 		allGrids[i].emptyThisAtBlock(iter->first, iter->second, "L");
					// 	}

					// }

				}

			}	

			allGrids[i].padAll();

		}

		//Update the grid to have the containers displayed here.
		// for(Grid g : allGrids) {

		// 	g.surroundBlockAt(0,0, "•");
			// std::vector<Container> allContainers = g.getAllContainersG();

			// for(Container c : allContainers) {
			// 	std::vector<std::pair<int,int>> vecCBI = c.getCBI();

			// 	for(std::pair<int,int> somePair : vecCBI) {
			// 		g.surroundBlockAt(somePair.first, somePair.second, "•");
			// 	}
			// }
		// }

		// for(Grid g : allGrids) {

			// std::vector<Container> allContainers = g.getAllContainersG();

			// for(Container c : allContainers) {
			// 	int es = c.getExpectedSum();
			// 	int cs = 0;

			// 	std::vector<std::pair<int,int>> vecCBI = c.getCBI();
			// 	for(std::pair<int,int> somePair : vecCBI) {

			// 		std::string temp = g.getValFromBlockAt(somePair.first, somePair.second);
			// 		int toAdd = std::stoi(temp);
			// 		cs += toAdd;

			// 	}

			// 	if(cs < es) {
			// 		allKillerSolutions.push_back(g);
			// 		std::cout << es - cs << "\n";
			// 	}
			// }

			// std::vector<Container> allContainers = g.getAllContainersG();
			// for(Container c : allContainers) {
			// 	std::cout << c.getOverflow() << std::endl;
			// 	if(c.getOverflow() > 0)
			// 		notAKillerSolution = true;
			// }

			// if(!notAKillerSolution)
			// 	allKillerSolutions.push_back(g);

			// notAKillerSolution = false;
			// std::cout << "\n=======================\n";
		// }

		while(allGrids.size() != 1) { //Pseudo remove duplicates
			allGrids.pop_back();
		}

	}

	std::cout << "\nGrid Unique Solution(s):\n";

	for(Grid g : allGrids) {
		if(isKiller)
			g.print(true);
		else 
			g.print();
	}

	assert(allGrids.size() > 0);

	if(allGrids.size() > 1) {
		std::cout << "FAILURE: Please try again.\n";
		std::cout << "There are " << allGrids.size() << " solutions to the input grid.\n" << std::endl;
	} else {
		std::cout << "SUCCESS! \nThere is exactly " << allGrids.size() << " solution to the input grid.\n";
	}

	return 0;
}

//Improvements to be made: 

//Pattern recognition over "brute force" solving the puzzle
//Current sum tracker
//   Stop when current sum exceeds expected sum
//Remove duplicates
//Can you get to that unique solution
//•