//
//  Puzzle.h
//  Sudoku
//
//  Created by Brian Mar on 1/27/14.
//  Copyright (c) 2014 Brian Mar. All rights reserved.
//

#ifndef __Sudoku__Puzzle__
#define __Sudoku__Puzzle__

#include <iostream>
#include "Cell.h"

class Puzzle{
public:
    //constructors
    Puzzle();
    ~Puzzle(); 
    void initializePuzzle();
    Puzzle(std::string inputString);
    
    //accessor methods
    std::vector<int> getPossibleValues(int row, int column);
    bool moreThanOnePossibleValue(int row, int column);
    int getCellNumber(int row, int column);
    
    //mutator methods
    void setCellNumber(int row, int column, int numberToBeSet);
    
    void updateRowColumnRegion(int row, int column);
    void generatePossibleValues();
    void printPuzzle();
    
    void printPossibleValues();
    bool isValid(int currentRow, int currentColumn, int numberToCheck);
    bool inRow(int currentRow, int numberToCheck);
    bool inColumn(int currentColumn, int numberToCheck);
    bool inRegion(int currentRow, int currentColumn, int numberToCheck);
    
    bool posInRow(int currentRow, int currentColumn, int posToCheck);
    bool posInColumn(int currentRow, int currentColumn, int posToCheck);
    bool posInRegion(int currentRow, int currentColumn, int posToCheck);
    
    bool solved();
    bool findEmptyCell();
    void decideCells();
    void erasePossibleValue(int i, int j, int k);

private:
    Cell sudokuGrid[9][9];
    int sizeOfPuzzle = 81;
    
};

#endif /* defined(__Sudoku__Puzzle__) */





