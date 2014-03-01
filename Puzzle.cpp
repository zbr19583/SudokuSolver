//
//  Puzzle.cpp
//  Sudoku
//
//  Created by Brian Mar on 1/27/14.
//  Copyright (c) 2014 Brian Mar. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include "Puzzle.h"
#include "Cell.h"
#include <sstream>
#include <string.h>
#include <algorithm>

//default constructor
Puzzle::Puzzle()
{
    initializePuzzle();
}


Puzzle::~Puzzle(void)
{
}


Puzzle::Puzzle(std::string inputString)
{
    int pos = 0;
    int id = 1;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            //assign id for reference
            sudokuGrid[i][j].iD = id;
            id++;
            int intValue = inputString[pos] - '0';
            if ((inputString[pos] == '.') || (intValue >= 0))
            {
                if (intValue >= 1)
                {
                    sudokuGrid[i][j].cellNumber = intValue;
                    pos++;
                } //fill in each cell's cellNumber with known values
                else
                {
                    sudokuGrid[i][j].cellNumber = 0;
                    pos++;
                }
            }
        } //for each cell...
    }
} //end Puzzle::puzzle


//initialize every cell with '.'
void Puzzle::initializePuzzle()
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            sudokuGrid[i][j].cellNumber = 0;    //initialize each number in cell
        }
    }
}


//generates possible values for each empty Cell
void Puzzle::generatePossibleValues()
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if (sudokuGrid[i][j].cellNumber == 0)
            {
                for (int x = 1; x <= 9; ++x)
                {
                    if (isValid(i, j, x))
                        sudokuGrid[i][j].possibleValues.push_back(x);
                }
            }
}


//get possible values of a Cell
std::vector<int> Puzzle::getPossibleValues(int row, int column)
{
    return sudokuGrid[row][column].possibleValues;
}

//true if the cell has more than one possible value
bool Puzzle::moreThanOnePossibleValue(int row, int column)
{
    return (sudokuGrid[row][column].possibleValues.size() > 1);
}


void Puzzle::updateRowColumnRegion(int row, int column)
{
    //update row
    for (int j = 0; j < 9; ++j)
    {
        for (int x = 0; x < sudokuGrid[row][j].possibleValues.size(); ++x)
            if (sudokuGrid[row][j].possibleValues[x] == sudokuGrid[row][column].cellNumber)
                sudokuGrid[row][j].possibleValues.erase(sudokuGrid[row][j].possibleValues.begin() +  x);
    }
    
    //update column
    for (int i = 0; i < 9; ++i)
    {
        for (int y = 0; y < sudokuGrid[i][column].possibleValues.size(); ++y)
            if (sudokuGrid[i][column].possibleValues[y] == sudokuGrid[row][column].cellNumber)
                sudokuGrid[i][column].possibleValues.erase(sudokuGrid[i][column].possibleValues.begin() +  y);
    }
    
    
    //update region TEST THIS CODE
    int startRow = (row/3)*3;
    int startColumn = (column/3)*3;
    for (int i = 0; i < 3; ++i, ++startRow)
    {
        int j = 0;
        for (int col = startColumn; j < 3; ++j, ++col)
            for (int z = 0; z < sudokuGrid[startRow][col].possibleValues.size(); ++z)
                if (sudokuGrid[startRow][col].possibleValues[z] == sudokuGrid[row][column].cellNumber)
                    sudokuGrid[startRow][col].possibleValues.erase(sudokuGrid[startRow][col].possibleValues.begin() + z);
    }
}


//get the cellNumber of a Cell
int Puzzle::getCellNumber(int row, int column)
{
    return sudokuGrid[row][column].cellNumber;
}


//set the cellNumber of a Cell
void Puzzle::setCellNumber(int row, int column, int numberToBeSet)
{
    sudokuGrid[row][column].cellNumber = numberToBeSet;
    sudokuGrid[row][column].possibleValues.clear();
}


//check if numberToCheck is valid for the cell
bool Puzzle::isValid(int currentRow, int currentColumn, int numberToCheck)
{
    return (!inRow(currentRow, numberToCheck) && !inColumn(currentColumn, numberToCheck) && !inRegion(currentRow, currentColumn, numberToCheck));
}


bool Puzzle::inRow(int currentRow, int numberToCheck)
{
    int i = currentRow;
    for (int j = 0; j < 9; ++j) //i represents the list of numbers to compare numberToCheck with
    {
        if (numberToCheck == sudokuGrid[i][j].cellNumber) //if the current traversed cell is not equal to the current number
            return true;
    } //traverse column for matches to numberToCheck
    return false; // should this return statement be here
}


bool Puzzle::inColumn(int currentColumn, int numberToCheck)
{
    int j = currentColumn;
    for (int i = 0; i < 9; ++i) //i represents the list of numbers to compare numberToCheck with
    {
        if (numberToCheck == sudokuGrid[i][j].cellNumber)
            return true;
    } //traverse row for matches to numberToCheck
    return false; // should this return statement be here
}


bool Puzzle::inRegion(int currentRow, int currentColumn, int numberToCheck)
{
    int startRow = (currentRow/3)*3;
    int startColumn = (currentColumn/3)*3;
    bool outcome = false;
    for (int i = 0; i < 3; ++i, ++startRow)
    {
        int j = 0;
        for (int column = startColumn; j < 3; ++j, ++column)
        {
            if (numberToCheck == sudokuGrid[startRow][column].cellNumber)
                outcome = true;
        }
    }
    return outcome;
}


bool Puzzle::posInRow(int currentRow, int currentColumn, int posToCheck)
{
    int i = currentRow;
    for (int j = 0; j < 9; ++j) //i represents the list of numbers to compare numberToCheck with
    {
        if (sudokuGrid[i][j].cellNumber == 0 && sudokuGrid[i][j].iD != sudokuGrid[currentRow][currentColumn].iD)
        {
            for (int k = 0; k < sudokuGrid[i][j].possibleValues.size(); ++k)
            {
                if (posToCheck == sudokuGrid[i][j].possibleValues[k])
                    return true;
            }
        }
    } //traverse column for matches to posToCheck
    return false; // should this return statement be here
}


bool Puzzle::posInColumn(int currentRow, int currentColumn, int posToCheck)
{
    int j = currentColumn;
    for (int i = 0; i < 9; ++i) //i represents the list of numbers to compare numberToCheck with
    {
        if (sudokuGrid[i][j].cellNumber == 0 && sudokuGrid[i][j].iD != sudokuGrid[currentRow][currentColumn].iD)
        {
            for (int k = 0; k < sudokuGrid[i][j].possibleValues.size(); ++k)
            {
                if (posToCheck == sudokuGrid[i][j].possibleValues[k])
                    return true;
            }
        }
    } //traverse row for matches to numberToCheck
    return false; // should this return statement be here
}


bool Puzzle::posInRegion(int currentRow, int currentColumn, int posToCheck)
{
    int startRow = (currentRow/3)*3;
    int startColumn = (currentColumn/3)*3;
    for (int i = 0; i < 3; ++i, ++startRow)
    {
        int j = 0;
        for (int column = startColumn; j < 3; ++j, ++column)
        {
            if (sudokuGrid[startRow][column].cellNumber == 0 && sudokuGrid[startRow][column].iD != sudokuGrid[currentRow][currentColumn].iD)
            {
                for (int k = 0; k < sudokuGrid[startRow][column].possibleValues.size(); ++k)
                {
                    if (posToCheck == sudokuGrid[startRow][column].possibleValues[k])
                        return true;
                }
            }
        }
    }
    return false;
}


//this did not work...
//bool Puzzle::posInRegion(int currentRow, int currentColumn, int posToCheck)
//{
//    int startRow = (currentRow/3)*3;
//    int startColumn = (currentColumn/3)*3;
//    for (int i = 0; i < 3; ++i, ++startRow)
//    {
//        for (int j = 0; j < 3; ++j, ++startColumn)
//        {
//            if (sudokuGrid[startRow][startColumn].cellNumber == 0 && sudokuGrid[startRow][startColumn].iD != sudokuGrid[currentRow][currentColumn].iD)
//            {
//                for (int k = 0; k < sudokuGrid[startRow][startColumn].possibleValues.size(); ++k)
//                {
//                    if (posToCheck == sudokuGrid[startRow][startColumn].possibleValues[k])
//                        return true;
//                }
//            }
//        }
//    }
//    return false;
//}

//


//return true if puzzle is solved
bool Puzzle::solved(){
    return (!findEmptyCell());
}


bool Puzzle::findEmptyCell()
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (sudokuGrid[i][j].cellNumber == 0)
                return true;
        }
    }
    return false;
} //returns true if there is at least one empty cell



void Puzzle::decideCells()
{
    //cell is the reference to the Cell with 1 possibleValue
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if (sudokuGrid[i][j].cellNumber == 0 && sudokuGrid[i][j].possibleValues.size() == 1)
            {
                sudokuGrid[i][j].cellNumber = sudokuGrid[i][j].possibleValues.front();
                sudokuGrid[i][j].possibleValues.pop_back();
                updateRowColumnRegion(i, j);
            }
}


//erase possibleValue
void Puzzle::erasePossibleValue(int i, int j, int k)
{
    sudokuGrid[i][j].possibleValues.erase(std::remove(sudokuGrid[i][j].possibleValues.begin(), sudokuGrid[i][j].possibleValues.end(), k), sudokuGrid[i][j].possibleValues.end());
}


//bool Puzzle::nakedPair()
//{
//    for (int i = 0; i < 9; ++i)
//        for (int j = 0; j < 9; ++j)
//        {
//            //traverse row
//            int currentRow = i;
//            for (int k = 0; k < 9; ++k)
//            {
//                if ( (k != j) && (sudokuGrid[i][j].possibleValues.size() == 2
//                                  && sudokuGrid[currentRow][k].possibleValues.size() == 2)
//                    && (sudokuGrid[i][j].possibleValues == sudokuGrid[currentRow][k].possibleValues))   //if we find a naked pair,
//                {
//                    for (int currentColumn = 0; currentColumn < 9; ++currentColumn)
//                    {
//                        for (int m = 0; m < sudokuGrid[currentRow][currentColumn].possibleValues.size(); ++m)
//                        {
//                            if (currentColumn != (k | j) && sudokuGrid[currentRow][currentColumn].possibleValues[m]
//                                == (sudokuGrid[i][j].possibleValues[0] | sudokuGrid[i][j].possibleValues[1]))
//                            {
//                                //remove the naked pairs from the row
//                                sudokuGrid[currentRow][currentColumn].possibleValues.erase(sudokuGrid[currentRow][currentColumn].possibleValues.begin() + m);
//                                return true;
//                            }
//                        }
//                    }
//                }
//            }
//            
//            //traverse column
//            int currentColumn = j;
//            for (int k = 0; k < 9; ++k)
//            {
//                if ( (k != i) && (sudokuGrid[i][j].possibleValues.size() == 2
//                                  && sudokuGrid[k][currentColumn].possibleValues.size() == 2)
//                    && (sudokuGrid[i][j].possibleValues == sudokuGrid[k][currentColumn].possibleValues))   //if we find a naked pair,
//                {
//                    for (int currentRow = 0; currentRow < 9; ++currentRow)
//                    {
//                        for (int m = 0; m < sudokuGrid[currentRow][currentColumn].possibleValues.size(); ++m)
//                        {
//                            if (currentRow != (k | i) && sudokuGrid[currentRow][currentColumn].possibleValues[m]
//                                == (sudokuGrid[i][j].possibleValues[0] | sudokuGrid[i][j].possibleValues[1]))
//                            {
//                                //remove the naked pairs from the row
//                                sudokuGrid[currentRow][currentColumn].possibleValues.erase(sudokuGrid[currentRow][currentColumn].possibleValues.begin() + m);
//                                return true;
//                            }
//                        }
//                    }
//                }
//            }
//            
//            //traverse region
////            
////            int startRow = (i/3)*3;
////            int startColumn = (j/3)*3;
////            bool outcome = false;
////            for (int m = 0; m < 3; ++m, ++startRow)
////            {
////                int n = 0;
////                for (int column = startColumn; n < 3; ++n, ++column)
////                {
////                    if ( (m != i) && (sudokuGrid[i][j].possibleValues.size() == 2
////                                      && sudokuGrid[k][currentColumn].possibleValues.size() == 2)
////                        && (sudokuGrid[i][j].possibleValues == sudokuGrid[k][currentColumn].possibleValues))   //if we find a naked pair,
////                        outcome = true;
////                }
////            }
////            return outcome;
//            
//            //traverse region
//            //add code
//        }
//    return false;
//}


void Puzzle::printPossibleValues()
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
        {
            for (int k = 0; k < sudokuGrid[i][j].possibleValues.size(); ++k)
            {
                std::cout << sudokuGrid[i][j].possibleValues[k];
            }
            std::cout << std::endl;
        }
}

void Puzzle::printPuzzle()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; ++j)
        {
            std::cout << sudokuGrid[i][j].cellNumber;
        }
    }
    std::cout << std::endl;
}

