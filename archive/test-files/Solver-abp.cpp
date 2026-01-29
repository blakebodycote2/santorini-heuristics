// Solver.cpp
#include "Solver.hpp"
#include <array>

double Solver::minimax(const Position &pos, double alpha, double beta, int depth)
{
    searchCount++;
    // Check for Winning move first.
    std::array<int, 8> deltas = {-6, -5, -4, -1, 1, 4, 5, 6};
    for (int worker = 0; worker < 2; worker++)
    {
        int from = pos.getCurrentWorkerPos(worker);
        for (const auto& delta : deltas) 
        {
            int to = from + delta;
            if (pos.canMove(from, to) && pos.isWinningMove(to))
                return 10000 * depth;
        }
    }

    if (depth == 0) // Return evaluation if depth limit is reached
        return evaluate(pos);

    // Then start recursive evaluation
    double bestEval = -10000;

    for (int worker = 0; worker < 2; worker++)
    {
        int from = pos.getCurrentWorkerPos(worker);
        // Loop over every move position
        for (const auto& d1 : deltas) 
        {
            int to = from + d1;
            if (!pos.canMove(from, to)) continue;

            for (const auto& d2 : deltas) 
            {
                int build = to + d2;
                if (!pos.canBuild(from, to, build)) continue;

                Position nextPos(pos);
                nextPos.play(worker, from, to, build);
                // std::cout << from << " -> " << to << " -> " << build << std::endl;
                double eval = -minimax(nextPos, depth-1);
                if (eval > bestEval) bestEval = eval;
            }
        }
    }
    if (depth == 3) std::cout << searchCount << std::endl;
    return bestEval;
    // JUST NEED TO ADD SOME WAY OF KEEPING TRACK OF BEST MOVE.
}

    // recursive section
    

double Solver::evaluate(const Position &pos)
{
    double eval = 0;
    // (1 - 2 * (pos.turn % 2)) or (2 * (pos.turn % 2) - 1) for player scalar
    for (int i = 0; i < 2; i++)
    {
        eval +=  10 * pos.getLevel(pos.workerPos[0][i]);
        eval += -10 * pos.getLevel(pos.workerPos[1][i]);
    }
    return eval;
}