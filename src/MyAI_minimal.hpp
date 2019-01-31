// ======================================================================
// FILE:        MyAI.hpp
// ======================================================================

#ifndef MINE_SWEEPER_CPP_SHELL_MYAI_HPP
#define MINE_SWEEPER_CPP_SHELL_MYAI_HPP

#include "Agent.hpp"
#include <iostream> // temporary use
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include<queue>

using namespace std;

typedef pair<int,int> Key;          // The key to the maps are the x and y coords of the agent
typedef map<Key,int> map1;                  // The value of the maps are the pathCost

// Utility structures for implementing graph based
class node{
public:
    node(int _x=0,int _y=0,int z=0):x(_x),y(_y),cost(z) {}
    int x;
    int y;
    int cost;
    bool operator<(const node& a) const
    {
        return cost < a.cost;
    }
};

class MyAI : public Agent {
public:
    MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY );

    Action getAction ( int number ) override;


    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
    // Implementing graph based UCS
private:
    int pathCost;	//temp variable to store current pathCost

    // declaring frontier Q, frontierMap and exporedMap(don't need exporedQ)
    // frontier Q which includes nodes yet to be expanded
    priority_queue<node> frontierQ;
    map1 frontierMap;				// Which nodes are in frontierQ
    map1 exploredMap;				// Which nodes have been explore
    
    void printMap(map1);			//Helper function to print the maps
    bool isinBounds(int, int);
    bool inMap(map1,int,int);
    Key findMine();
    void insertNode(int x, int y, int pathCost);
    void removeNodeandUpdateAgent();
    void expandChildren(int pathCost);
    int numOfUncovered();
    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================
};

#endif //MINE_SWEEPER_CPP_SHELL_MYAI_HPP
