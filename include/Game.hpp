#ifndef GAME_HPP
#define GAME_HPP

#include "Solver.hpp"
#include <iostream>

class Game 
{
private:
    Solver player0;
    Solver player1;

public:
    Game(Solver p0, Solver p1);

    Solver play();
};

#endif // GAME_HPP

