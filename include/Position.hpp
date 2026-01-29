#ifndef POSITION_HPP
#define POSITION_HPP

#include "Bitboard.hpp"
#include <iostream>


class Position 
{
public:
    int turn;
    int workerPos[2][2];
    Bitboard workers[2];  
    Bitboard board[5];

    Position();
    Position(const Position&);

    int getCurrentWorkerPos(int worker) const;

    int getLevel(int square) const;
    void setLevel(int square, int level);

    bool canMove(int from, int to) const;
    bool canBuild(int from, int to, int buildSpot) const;
    
    void play(int worker, int from, int to, int build);
    bool isWinningMove(int square) const;
    unsigned int nbMoves() const;
};

#endif  // POSITION_HPP

