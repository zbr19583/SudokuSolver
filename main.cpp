//
//  main.cpp
//  Sudoku
//
//  Created by Brian Mar on 1/27/14.
//  Copyright (c) 2014 Brian Mar. All rights reserved.
//
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string>
#include "Puzzle.h"
#include "Cell.h"
#include <list>
#include <stack>
#include <sstream>
#include <iomanip>

void readPuzzle(std::string &puzzleString);
bool guess(Puzzle &puzzle, Puzzle &alternative);
bool hiddenSingles(Puzzle &puzzle);
std::string readAndVerify(char input[81]);
bool isPrintable(char character);
std::string charToString(char character);
std::string intToString(int integer);
std::string charToHex(unsigned char character);
int stringToInt(std::string string);
std::string printHex(std::string value);
void verifyInputString(std::string inputString);



//template that can clone ANY object
template <class T>
T *clone(T orig)
{
    T *copy = new T();
    
    *copy = orig;
    return copy;
}


int main()
{
    std::list<Puzzle> solutions;
    
    static std::stack<Puzzle> alternatives;
    
    char inputString[81];
    
    std::string inputPuzzle = readAndVerify(inputString);
    
    Puzzle puzzle(inputPuzzle);
    
    puzzle.generatePossibleValues();
    
    alternatives.push(puzzle);
    
    while (!alternatives.empty())
    {
        puzzle = alternatives.top();
        alternatives.pop();
        
        //decide all immediately decideable cells
        puzzle.decideCells();
        
        //try simplification strats
        bool simplificationFound = true;
        while (!puzzle.solved() && simplificationFound)
        {
            simplificationFound = false;
            do
            {
                simplificationFound = hiddenSingles(puzzle);
            }
            while (simplificationFound == true);
            
            //fall back to guessing
            if (!simplificationFound)
            {
                Puzzle alternative;
                alternative = *clone(puzzle);
                if ((simplificationFound = guess(puzzle, alternative)))
                {
                    //record alternative if guess is wrong
                    alternatives.push(alternative);
                }
            }
            
            //decide all immediately decidable cells before looking for further simplifications
            if (simplificationFound)
            {
                puzzle.decideCells();
            }
        }
        
        
        //if solution is found or contradiction is found(no simplifications)
        if (puzzle.solved())
        {
            solutions.push_back(puzzle);
        }
    }
    
    if (solutions.empty())
    {
        std::cout << "No solutions.\n";
    }
    
    if (!solutions. empty())
    {
        while (!solutions.empty())
        {
            solutions.front().printPuzzle();
            solutions.pop_front();
        }
    }
    
    std::exit(0);
    
}//end main

//*****************************************************************************************



//return true if guess was correct
bool guess(Puzzle &puzzle, Puzzle &alternative)
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if ((puzzle.getCellNumber(i, j) == 0) && !(puzzle.getPossibleValues(i, j).empty()))
            {
                for (int k = 0; k < puzzle.getPossibleValues(i, j).size(); ++k)
                {
                    //make an alternative puzzle that assumes the puzzle is false
                    //MUST MAKE A DEEP COPY
                    alternative.erasePossibleValue(i, j, puzzle.getPossibleValues(i, j)[k]);
                    
                    //set number as guess
                    puzzle.setCellNumber(i, j, puzzle.getPossibleValues(i, j)[k]);
                    puzzle.getPossibleValues(i, j).clear(); //clear possible values after setting cell number
                    puzzle.updateRowColumnRegion(i, j);
                    return true;
                }
            }
            else if (puzzle.getCellNumber(i, j) == 0 && puzzle.getPossibleValues(i, j).empty())
                return false;
            //contradiction occured...
        }
    }
    return 0;
} //must return false for contradictions


bool hiddenSingles(Puzzle &puzzle)
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if (puzzle.getCellNumber(i, j) == 0)
            {
                for (int k = 0; k < puzzle.getPossibleValues(i, j).size(); ++k)
                {
                    if (!puzzle.posInRegion(i, j, puzzle.getPossibleValues(i, j)[k]) || !puzzle.posInColumn(i, j, puzzle.getPossibleValues(i, j)[k]) || !puzzle.posInRow(i, j, puzzle.getPossibleValues(i, j)[k]))
                    {
                        puzzle.setCellNumber(i, j, puzzle.getPossibleValues(i, j)[k]);
                        puzzle.getPossibleValues(i, j).clear();
                        puzzle.updateRowColumnRegion(i, j);
                        return true;
                    }
                }
            }
    return false;
}


std::string readAndVerify(char input[81])
{
    unsigned char unexpectedChar;
    std::string savedString;
    for (int index = 0; index < 81; ++index)
    {
        char input = std::cin.get();
        //eof
        if (std::cin.eof())
        {
            std::cout << "ERROR: expected <value> saw <eof>" << std::endl;
            std::exit(0);
        }
        
        if (input != '3' && input != '2' && input != '1' &&
            input != '4' && input != '5' && input != '6' &&
            input != '9' && input != '8' && input != '7' &&
            input != '.' )
        {
            unexpectedChar = input;
            if (unexpectedChar == ' ')
            {
                std::cout << "ERROR: expected <value> saw " << " " << std::endl;
                std::exit(0);
            }
            else if (unexpectedChar == '\n')
            {
                std::cout << "ERROR: expected <value> saw \\n" << std::endl;
                std::exit(0);
            }
            else if (isprint(unexpectedChar))
            {
                std::cout << "ERROR: expected <value> saw " << unexpectedChar << std::endl;
                std::exit(0);
            }
            else
            {
                std::cout << "ERROR: expected <value> saw \\x" << std::setw(2) << std::setfill('0') << std::hex << (int)unexpectedChar<< std::endl;
                std::exit(0);
            }
        }
        //CHECKPOINT
        
        if (input == '\n')
        {
            std::cout << "ERROR: expected <value> saw \\n" << std::endl;
            std::exit(0);
        }
        
        //append to string
        savedString += input;
    }
    
    char input2 = std::cin.get();
    if (input2 != '\n')
    {
        if (input2 == ' ')
        {
            std::cout << "ERROR: expected <value> saw " << " " << std::endl;
            std::exit(0);
        }
        else if (isprint(input2))
        {
            std::cout << "ERROR: expected \\n saw " << input2 << std::endl;
            std::exit(0);
        }
        else
        {
            std::cout << "ERROR: expected \\n saw \\x" << std::setw(2) << std::setfill('0') << std::hex << (int)input2 << std::endl;
            std::exit(0);
        }
    }
    
    char input3 = std::cin.get();
    if (!std::cin.eof())
    {
        if (input3 == ' ')
        {
            std::cout << "ERROR: expected <value> saw " << " " << std::endl;
            std::exit(0);
        }
        else if (input3 == '\n')
        {
            std::cout << "ERROR: expected <eof> saw \\n" << std::endl;
            std::exit(0);
        }
        else if (isprint(input3))
        {
            std::cout << "ERROR: expected \\n saw " << input3 << std::endl;
            std::exit(0);
        }
        else
        {
            std::cout << "ERROR: expected \\n saw \\x" << std::setw(2) << std::setfill('0') << std::hex << (int)input3<< std::endl;
            std::exit(0);
        }
    }
    return savedString;
}


bool isPrintable(char character)
{
	if (character == '\n')
	{
		return true;
	}
    
	return (bool) isprint(character);
}


std::string charToString(char character)
{
    //if the character is \n, return \\n
    if (character == '\n')
    {
        return "\\n";
    }
    else
    {
        std::stringstream ss;
        ss << character;
        return ss.str();
    }
}


std::string intToString(int integer)
{
    std::stringstream ss;
    ss << integer;
    return ss.str();
}


std::string charToHex(unsigned char character)
{
    std::stringstream ss;
    ss << std::hex << std::setw(2) << (int)character;
    return ss.str();
}

int stringToInt(std::string string)
{
    int integer;
    std::istringstream iss (string);
    iss >> integer;
    return integer;
}


std::string printHex(std::string value)
{
    char fill = '0';
    int width = 2;
    std::stringstream ss;
	ss << std::setfill(fill) << std::setw(width) << value;
	return ss.str();
}

//bool nakedPair(Puzzle &puzzle)
//{
//    for (int i = 0; i < 9; ++i)
//        for (int j = 0; j < 9; ++j)
//        {
//            //traverse row
//            currentRow = i;
//            for (int k = 0; k < 9; ++k)
//            {
//                if ( k != j && (puzzle.getPossibleValues(i, j).size() == puzzle.getPossibleValues(currentRow, k).size()) && (puzzle.getPossibleValues(i, j) == puzzle.getPossibleValues(currentRow, k))
//                {
//                    std::cout << "All elements are equal each other" << std::endl;
//                }
//            }  
//        }
//
//}
