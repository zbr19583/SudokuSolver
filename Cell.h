//
//  Cell.h
//  Sudoku
//
//  Created by Brian Mar on 1/27/14.
//  Copyright (c) 2014 Brian Mar. All rights reserved.
//

#ifndef __Sudoku__Cell__
#define __Sudoku__Cell__

#include <iostream>
#include <vector>
#include <set>

class Cell{
public:
    Cell();
    
    int iD;
    int cellNumber;
    std::vector<int> possibleValues;
};


#endif /* defined(__Sudoku__Cell__) */



