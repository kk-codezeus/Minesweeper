// ======================================================================
// FILE:        MyAI.cpp
//
// ======================================================================

#include "MyAI.hpp"

MyAI::MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY ) : Agent()
{
    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
    rowDimension = _rowDimension;
    colDimension = _colDimension;
    totalMines = _totalMines;
    agentX = _agentX;
    agentY = _agentY;
    pathCost = 1000;
    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================
};

Agent::Action MyAI::getAction( int number )
{
    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
    if(number == -1)
    {
	//printMap(frontierMap);
	removeNodeandUpdateAgent();
	if(pathCost < -25)
	{
		return {LEAVE,-1,-1};
	}
	else
        return {UNCOVER, agentX, agentY};
        // update agents, expand children, pop child, then uncover child
    }
    if(number == 0)
    {
        exploredMap.insert(make_pair(make_pair(agentX,agentY),pathCost));
        
        expandChildren(pathCost-1);
        
        // get a node from frontierQ and also remove it from the frontierMap
        
        removeNodeandUpdateAgent();
        
//        printMap(exploredMap);
//        printMap(frontierMap);
        return {UNCOVER,agentX,agentY};
    }
    if(number == 1)
    {
	    //if node has pathCost > 0 set its pathCost = 0 and reinsert in frontierQ and frontierMap
	if(pathCost > 0)
        {
            insertNode(agentX, agentY, -50);
            removeNodeandUpdateAgent();
            //return Action
            if(pathCost > 0) 
            return {UNCOVER,agentX,agentY};
        }
	    //else if pathCost == 0, find the mine around it and Flag that mine
	    if(totalMines !=0)
	    {
    		int uncoveredNeighbors = numOfUncovered();		
            	while(uncoveredNeighbors > 1)
            	{
                	insertNode(agentX, agentY, -100);
                	removeNodeandUpdateAgent();
			uncoveredNeighbors = numOfUncovered();
            	}
		if(uncoveredNeighbors == 1)
		{
			exploredMap.insert(make_pair(make_pair(agentX,agentY),pathCost); //Insert the node with only mine as its neigh		
			totalMines--;
			Key mine = findMine();
			agentX = mine.first;
			agentY = mine.second;
			pathcost = -25;


			exploredMap.insert(make_pair(make_pair(agentX.agentY),pathCost)); //Insert the mine tile in exploredMap
			expandChildren(-25); 		//Expanding the children of the mine and assigning them lower priority than other 
               	        return{FLAG,agentX, agentY};
		}
            }
	 
            else if(totalMines == 0)
            {
		removeNodeandUpdateAgent();
		if(pathCost == -25)
			return {UNCOVER,agentX,agentY};
		else
		{
			
		}
                // update agents, expand children, (if no child, then LEAVE), pop child, then uncover child
            }
            
        
    }
    return {LEAVE, -1, -1};
    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================

}

Key MyAI::findMine()
{
    for(int x = agentX - 1;x<=agentX + 1;x++)
    {
        for(int y = agentY - 1;y<=agentY + 1;y++)
        {
            if(isinBounds(x,y) && !inMap(frontierMap,x,y) &&!inMap(exploredMap,x,y))
            {
                Key mine(x,y);
                return mine;
            }
        }
    }
    return make_pair(-1,-1);
}

bool MyAI::isinBounds(int x,int y)
{
   return  (0<= x && x < rowDimension && 0<= y && y < colDimension);
}

bool MyAI::inMap(map1 checkMap, int x,int y)
{
	Key check(x,y);
	map1::iterator it = checkMap.begin();
	it = checkMap.find(check);
	if(it != checkMap.end())
		return true;
	return false;
}	

void MyAI::printMap(map1 printMap)
{

	for(map1::iterator it=printMap.begin();it!=printMap.end();++it)
		cout<<it->first.first<<" "<<it->first.second<<" "<<it->second<<endl;
}

void MyAI::insertNode(int x, int y, int pathCost) {
    node toBeInserted(x, y, pathCost);
    frontierQ.push(toBeInserted);
    Key newPos(x,y);
    frontierMap.insert(make_pair(newPos,pathCost));
}

void MyAI::removeNodeandUpdateAgent() {
    node child = frontierQ.top();
    frontierQ.pop();
    frontierMap.erase(make_pair(child.x,child.y));
    pathCost = child.cost;
    agentX = child.x;
    agentY = child.y;
}

void MyAI::expandChildren(int pathCost) {
    for(int x=agentX-1;x<=agentX+1;x++)
    {
        for(int y=agentY-1;y<=agentY+1;y++)
        {
            if(isinBounds(x,y) && !inMap(frontierMap,x,y) &&!inMap(exploredMap,x,y))
            {
		if(x == agentX && y == agentY)
		    continue;	
                insertNode(x, y, pathCost);
            }
        }
    }
}

int MyAI::numOfUncovered() {
    int count = 0;
    for(int x = agentX - 1;x<=agentX + 1;x++)
    {
        for(int y = agentY - 1;y<=agentY + 1;y++)
        {
            if(isinBounds(x,y) && !inMap(frontierMap,x,y) &&!inMap(exploredMap,x,y))
            {
		if(x == agentX && y == agentY)
		    continue;
                count++;
            }
        }
    }
    return count;
}
