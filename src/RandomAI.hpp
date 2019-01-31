// ======================================================================
// FILE:        RandomAI.hpp
//
// ======================================================================

#ifndef MINE_SWEEPER_CPP_SHELL_RANDOMAI_HPP
#define MINE_SWEEPER_CPP_SHELL_RANDOMAI_HPP

#include <cstdlib>
#include "Agent.hpp"
#include<iostream>

class RandomAI : public Agent
{
public:

    RandomAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY){
        rowDimension = _rowDimension;
        colDimension = _colDimension;
    };

    Action getAction( int number) override
    {
        return{actions[rand() % 4], rand() % rowDimension, rand() % colDimension};
    }

private:

    const Action_type actions[4] =
            {
                    LEAVE,
                    UNCOVER,
                    FLAG,
                    UNFLAG,
            };

};
#endif //MINE_SWEEPER_CPP_SHELL_RANDOMAI_HPP
