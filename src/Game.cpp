//Game.cpp
#include "Game.hpp"

Game::Game(Solver p0, Solver p1)
{
    player0 = p0;
    player1 = p1;
}

Solver Game::play()
{
    Position pos = Position();

    MoveResult result = player0.playMove(pos); // Play the first move.

    // Play the rest of the game.
    while (true)
    {
        result = player1.playMove(result.pos);
        if (result.winner != -1)
            break;
        result = player0.playMove(result.pos);
        if (result.winner != -1)
            break;
    }
    /*
    result.pos.workers[0].printBitboard();
    result.pos.workers[1].printBitboard();
    result.pos.board[0].printBitboard();
    result.pos.board[1].printBitboard();
    result.pos.board[2].printBitboard();
    result.pos.board[3].printBitboard();
    result.pos.board[4].printBitboard();
    */
    // std::cout << " Winner : " << result.winner << std::endl;
    
    return (result.winner == 0) ? player0 : player1;
}