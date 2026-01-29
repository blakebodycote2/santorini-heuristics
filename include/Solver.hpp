// Solver.hpp
#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <iostream>
#include <algorithm>
#include <random>
#include "Position.hpp"

struct Move {
    int worker;
    int from;
    int to;
    int level;

    Move() : worker(0), from(0), to(0), level(0) {}
};

struct MoveResult 
{
    Position pos;
    int winner;

    MoveResult(const Position& position, int winId) 
        : pos(position), winner(winId) {}
};

class Solver 
{
public:
    int rootDepth = 3;
    int searchCount = 0; // for debug
    double w[9];
    
    Solver();

    MoveResult playMove(Position pos);

    double getWeight(int index) const;
    void setWeight(int index, double weight);

    void generateRandomWeights();

    MoveResult negamaxMain(const Position &pos, double alpha, double beta, int depth);
    double negamaxRecur(const Position &pos, double alpha, double beta, int depth);

    int checkForWinner(const int turn, const int eval, const Position& pos);
    
    double evaluate(const Position &pos);

    double currHeight(const Position &pos);
    double oppHeight(const Position &pos);

    double currMobility(const Position &pos);
    double oppMobility(const Position &pos);

    double currCoverage(const Position &pos);
    double oppCoverage(const Position &pos);

    double currCentrality(const Position &pos);
    double oppCentrality(const Position &pos);

    double proximity(const Position &pos);
};

#endif // SOLVER_HPP

