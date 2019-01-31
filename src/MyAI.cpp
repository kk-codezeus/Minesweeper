// ======================================================================
// FILE:        MyAI.cpp
//
// ======================================================================

#include "MyAI.hpp"

MyAI::MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY ) : Agent()
{
    rowDimension = _rowDimension;
    colDimension = _colDimension;
    totalMines = _totalMines;
    agentX = _agentX;
    agentY = _agentY;
    
    state = new int*[_colDimension];
    for(int i = 0;i<_colDimension;i++)
    {
        state[i] = new int[_rowDimension];
    }
    for(int i = 0;i < _colDimension;i++)
    {
        for(int j = 0;j < _rowDimension;j++)
        {
            state[i][j] = 10;
        }
    }
    addToBorder(0);
};

MyAI::~MyAI () {
    for (int i = 0; i < colDimension; i++) {
        delete [] state[i];
    }
    delete [] state;

}

Agent::Action MyAI::getAction( int number )
{
    state[agentX][agentY] = number;
//    printMatrix(state);
    
    addToBorder(number);
    //remove any tiles that were previously added to tilesOnBorder that are no longer border tiles
    removeNonBorders();
    
    //for debugging purposes!!!
//    cout << "tilesToFlag: ";
//    printList(tilesToFlag);
//    cout << endl << "tilesToUncover: ";
//    printList(tilesToUncover);
//    cout << endl << "tilesOnBorder: ";
//    printList(tilesOnBorder);
//    cout << endl;
    
    //empty out any tiles that need to be flagged as indicated by rule #1
    if (!tilesToFlag.empty())
    {
        updateAgent(tilesToFlag);
        totalMines--;
        return {FLAG,agentX,agentY};
    }
    //empty out any tiles that need to be uncovered as indicated by rule #2
    if (!tilesToUncover.empty())
    {
        updateAgent(tilesToUncover);
        return {UNCOVER,agentX,agentY};
    }
    
    if (totalMines > 0) {
        for (list<Key>::iterator it=tilesOnBorder.begin(); it != tilesOnBorder.end(); ++it) {
            agentX = it->first;
            agentY = it->second;
            number = state[agentX][agentY];
            
            //if percept is 0, add all of current tile's children to tilesToUncover
            if (number == 0) {
                expandChildren();
                updateAgent(tilesToUncover);
                return {UNCOVER, agentX, agentY};
            }
            
            //apply RULE #1 or RULE #2 to find new times to flag or uncover, or randomly choose one to uncover
            int numOfCovered = numberOfCovered(10, agentX, agentY);
            int numOfFlag = numberOfCovered(-1, agentX, agentY);
            //RULE #1
            if (numOfCovered == number - numOfFlag)
            {
                addToList(tilesToFlag);
                updateAgent(tilesToFlag);
                totalMines--;
                return {FLAG,agentX,agentY};
            }
            //#RULE 2
            if (number == numOfFlag)
            {
                addToList(tilesToUncover);
                updateAgent(tilesToUncover);
                return {UNCOVER,agentX,agentY};
            }
        }
        //chooseRandom();
        chooseNeighbor();
        return {UNCOVER, agentX, agentY};
    }
    
    //already found all the mines, but still need to uncover all other tiles
    else if (totalMines == 0) {
        //uncover all the rest where state[x][y] == 10
        for (int i = 0; i < colDimension; i++) {
            for (int j = 0; j < rowDimension; j++) {
                if (state[i][j] == 10){
                    Key current(i, j);
                    tilesToUncover.push_front(current);
                }
            }
        }
        if (!tilesToUncover.empty()) {
            updateAgent(tilesToUncover);
            return {UNCOVER, agentX, agentY};
        }
        //if there are no more mines and the tilesToUncover list is empty, then we must be done with the game!
        else
            return {LEAVE, -1, -1};
    }
    return {LEAVE, -1, -1};
}

bool MyAI::isinBounds(int x,int y)
{
    return (0 <=  x && x < colDimension && 0 <= y && y < rowDimension);
}

void MyAI::insertKey(int x, int y, list<Key>& l1) {
    Key newPos(x,y);
    l1.push_front(newPos);
}

void MyAI::updateAgent(list<Key>& l1)
{
    Key toReturn = l1.front();
    l1.pop_front();
    agentX = toReturn.first;
    agentY = toReturn.second;
}

void MyAI::addToBorder(int number) {
    Key border(agentX, agentY);
    if (number == 0)
        tilesOnBorder.push_front(border);
    else
        tilesOnBorder.push_back(border);
}

void MyAI::removeNonBorders() {
    int coveredNeighbors, x, y;
    for (list<Key>::iterator it = tilesOnBorder.begin(); it != tilesOnBorder.end(); ++it) {
        x = it->first;
        y = it->second;
        coveredNeighbors = numberOfCovered(10, x, y);
        if (coveredNeighbors == 0 || state[x][y] == -1) {
            it = tilesOnBorder.erase(it);
            it--;
        }
    }
}

void MyAI::expandChildren() {
    for (int x=agentX-1; x<=agentX+1; x++)
    {
        for (int y=agentY-1; y<=agentY+1; y++)
        {
            if (isinBounds(x,y) && !inBorder(x,y) && state[x][y] == 10)
            {
                if (x == agentX && y == agentY)
                    continue;
                insertKey(x, y, tilesToUncover);
            }
        }
    }
}

void MyAI::addToList(list<Key>& l1) {
    for (int x = agentX - 1; x<=agentX + 1; x++)
    {
        for (int y = agentY - 1; y<=agentY + 1; y++)
        {
            if (isinBounds(x,y) && !inBorder(x,y) && state[x][y] == 10)
            {
                if (x == agentX && y == agentY)
                    continue;
                Key toAdd(x, y);
                l1.push_front(toAdd);
            }
        }
    }
}

bool MyAI::inBorder(int x, int y) {
    Key border(x, y);
    for (list<Key>::iterator it=tilesOnBorder.begin(); it != tilesOnBorder.end(); ++it) {
        if (*it == border)
            return true;
    }
    return false;
}

int MyAI::numberOfCovered(int number, int _x, int _y) {
    int count = 0;
    for (int x = _x - 1; x<=_x + 1; x++)
    {
        for (int y = _y - 1; y<=_y + 1; y++)
        {
            if (isinBounds(x,y) && !inBorder(x,y) && state[x][y] == number) {
                if (x == _x && y == _y)
                    continue;
                count++;
            }
        }
    }
    return count;
}

void MyAI::chooseRandom() {
    srand(time(NULL));
    int random_x, random_y, random;
    
    while (true) {
        random = rand();
        random_x = random % colDimension;
        random_y = random % rowDimension;
        if (state[random_x][random_y] == 10) {
            agentX = random_x;
            agentY = random_y;
            break;
        }
    }
}

void MyAI::chooseNeighbor() {
    int x, y, percept, totalCovered, totalFlagged, combination;
    double currentProb, leastLikelyProb = 1.0;
    Key leastLikely(0,0);
    bool found = false;
    
    for (list<Key>::iterator it=tilesOnBorder.begin(); it != tilesOnBorder.end(); ++it) {
        x = it->first;
        y = it->second;
        percept = state[x][y];
        totalCovered = numberOfCovered(10, x, y);
        totalFlagged = numberOfCovered(-1, x, y);
        
        for (int i = x-1; i <= x+1; i++) {
            for (int j = y-1; j <= y+1; j++) {
                if (isinBounds(i,j) && !inBorder(i,j) && state[i][j] == 10) {
                    found = false;
                    currentProb = static_cast<double>(percept - totalFlagged) / totalCovered;
                    for (list<Probability>::iterator itt= probList.begin(); itt != probList.end(); ++itt) {
                        if (itt->first == (i*100+j)){
                            found = true;
                            if (currentProb > itt->second){
                                itt->second = currentProb;
                            }
                        }
                    }
                    if (!found) {
                        Probability toAdd(i*100+j, currentProb);
                        probList.push_back(toAdd);
                    }
                }
            }
        }
    }
    for (list<Probability>::iterator it= probList.begin(); it != probList.end(); ++it) {
        if (it->second < leastLikelyProb) {
            leastLikelyProb = it->second;
            leastLikely.first = it->first / 100;
            leastLikely.second = it->first - (leastLikely.first * 100);
        }
    }
    while (!probList.empty()){
        probList.pop_back();
    }
    agentX = leastLikely.first;
    agentY = leastLikely.second;
}

void MyAI::printMatrix(int** state) {
    for (int i = 0; i < colDimension; i++) {
        for (int j = 0; j < rowDimension; j++) {
            cout << state[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void MyAI::printList(list<Key> l1) {
    for (list<Key>::iterator it = l1.begin(); it != l1.end(); ++it) {
        cout << it->first << ", " << it->second << "\t";
    }
}
