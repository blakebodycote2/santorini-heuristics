// Position.cpp
#include "Position.hpp"
#include <random>

Position::Position() 
{
    turn = 0;

    board[0] = Bitboard((1ULL << 25) - 1); // All squares start at level 0
    for (int i = 1; i < 5; ++i)
        board[i] = Bitboard(0U);
    
    for (int i = 0; i < 2; ++i)
        workers[i] = Bitboard(0U);

    // specific position
    /*
    board[0] = 0b1111111011010011000110111;
    board[1] = 0b0000000100000000110000000;
    board[2] = 0b0000000000001100001000000;
    board[3] = 0b0000000000000000000000000;
    board[4] = 0b0000000000100000000001000;*/
    
    // Define random
    std::random_device rd;
    std::mt19937 gen(rd()); // Random seed
    std::uniform_int_distribution<int> index(0, 8);
    // Set worker starting positions
    int startingChoices[9] = {b2, b3, b4, 
                              c2, c3, c4, 
                              d2, d3, d4};
    int startingSpot;
    for (int player = 0; player < 2; player++) 
        for (int i = 0; i < 2; i++)
        {
            do {
                startingSpot = startingChoices[index(gen)];
            } while (workers[0].getBit(startingSpot) || workers[1].getBit(startingSpot));
            
            workerPos[player][i] = startingSpot;
            workers[player].setBit(startingSpot);
        }
}

Position::Position(const Position& other)
{
    this->turn = other.turn;
    for (int i = 0; i < 2; ++i) 
        for (int j = 0; j < 2; ++j) 
            workerPos[i][j] = other.workerPos[i][j];
    // Copy bitboards
    for (int i = 0; i < 2; ++i) 
        this->workers[i] = other.workers[i];
    for (int i = 0; i < 5; ++i) 
        board[i]= other.board[i];
}

int Position::getCurrentWorkerPos(int worker) const
{
    return workerPos[turn % 2][worker];
}

int Position::getLevel(int square) const 
{
    for (int i = 0; i < 4; i++)
        if (board[i].getBit(square)) return i;
    return 4; // If none match, it must be level 4.
}

void Position::setLevel(int square, int level) 
{
    board[level].removeBit(square);
    board[level+1].setBit(square);
}

bool Position::canMove(int from, int to) const
{
    // Check if move spot is on the board.
    if (!(to >= 0 && to <= 24)) return false;

    // Prevent illegal wrapping on horizontal bounds
    if (abs((from % 5) - (to % 5)) > 1) return false;

    // Check no workers at move spot.
    if (workers[0].getBit(to) || workers[1].getBit(to)) return false;

    // Ensure the current level is higher or a single level lower than the new spot.
    return (getLevel(from) + 1) >= getLevel(to);
}

bool Position::canBuild(int from, int to, int build) const 
{
    // Check if build spot is on the board.
    if (build < 0 || build > 24) return false;

    // Prevent illegal wrapping on horizontal bounds
    if (abs((to % 5) - (build % 5)) > 1) return false;


    // Prevent building on workers or domes, except when the worker
    // was previously standing on the square (from == build)
    if ((workers[0].getBit(build) && (from != build)) ||
        (workers[1].getBit(build) && (from != build)) || 
        board[4].getBit(build)) 
        return false;

    return true;
}

void Position::play(int worker, int from, int to, int build)
{
    // Move
    workerPos[turn % 2][worker] = to;
    workers[turn % 2].removeBit(from);
    workers[turn % 2].setBit(to);

    // Build
    int buildLevel = getLevel(build);
    setLevel(build, buildLevel);

    turn++;
}

bool Position::isWinningMove(int square) const
{
    return (board[3].getBit(square));
}

