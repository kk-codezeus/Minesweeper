// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Jian Li
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#ifndef MINE_SWEEPER_CPP_SHELL_MYAI_HPP
#define MINE_SWEEPER_CPP_SHELL_MYAI_HPP

#include "Agent.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <list>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef pair<int,int> Key;
typedef pair<int,double> Probability;

class MyAI : public Agent {
public:
    MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY);
    ~MyAI();

    Action getAction ( int number ) override;
    
private:
    int **state;
    list<Key> tilesOnBorder;
    list<Key> tilesToFlag;
    list<Key> tilesToUncover;
    list<Probability> probList;
    
    bool isinBounds(int, int);
    void insertKey(int, int, list<Key>&);
    void updateAgent(list<Key>&);
    void addToBorder(int number);
    void removeNonBorders();
    void expandChildren();
    void addToList(list<Key>&);
    bool inBorder(int, int);
    int numberOfCovered(int, int, int);
    void chooseNeighbor();
    void chooseRandom();
    void printMatrix(int**);
    void printList(list<Key>);
};

#endif //MINE_SWEEPER_CPP_SHELL_MYAI_HPP
